#include "globals.hxx"
#include "rock.hpp"
#include "UI.hpp"
#include "callbacks.hpp"
#include "scene.hpp"
#include <iostream>

int main(){
    // Camera setup
    camera.target   = {0,0};
    camera.offset   = {WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 2};
    camera.rotation = 0;
    camera.zoom     = 5.0f;

    // Window initialization
    InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Rocky the pet rock");
    SetTargetFPS(gameSettings.targetFPS);
    SetWindowMonitor(0);
    Image img = LoadImage((TEXTURES_PATH/"rocky.png").u8string().c_str());
    SetWindowIcon(img);
    UnloadImage(img);

    // Resource loading
    LoadCosmetics();
    InitShaders();
    background_texture = LoadTexture((TEXTURES_PATH/"grass.png").u8string().c_str());

    // Variable declarations
    Rock *rock = new Rock(Vector2{0,0});
    camera.target = rock->GetTransform().position;
    UIContainer *ui = new UIContainer;
    rock->SetCosmetic(COSMETICS::NONE);

    // =========================
    // === BUTTON SHENNIGANS ===
    // =========================
    float button_spacing = 85;
    float button_offset  = 50;
    const float BUTTON_Y = 580;
    // -----------------
    // Buttons panel
    Transform2D panel_transform;
    panel_transform.position = {0, BUTTON_Y - 50};
    panel_transform.rotation = 0;
    panel_transform.scale    = 1;
    UI::Panel* buttons_panel = new UI::Panel(panel_transform, Vector2{WINDOW_SIZE.x, WINDOW_SIZE.y - (BUTTON_Y - 50)}, {240, 240, 255, 150}, {220, 220, 255, 200}, 5);
    buttons_panel->SetDrawOrder(-1);
    ui->AddElement("buttons_panel", buttons_panel);
    // Exit button
    UI::Button* exit_btn =  new UI::Button(LoadTexture((TEXTURES_PATH/"exit_button.png").u8string().c_str()), {Vector2{35, 35}, 0, 3});
    exit_btn->DefineOnPressCallback(ExitCallback);
    ui->AddElement("button_exit", exit_btn);
    // -----------------
    // Shop button
    UI::Button* shop_btn = new UI::Button(LoadTexture((TEXTURES_PATH/"shop_button.png").u8string().c_str()), {Vector2{button_offset, BUTTON_Y}, 0, 4});
    ui->AddElement("button_shop", shop_btn);
    // -----------------
    // Cosmetics button
    UI::Button* cosmetics_btn = new UI::Button(LoadTexture((TEXTURES_PATH/"cosmetics_button.png").u8string().c_str()), {Vector2{button_offset + button_spacing * 1, BUTTON_Y}, 0, 4});
    ui->AddElement("button_cosmetics", cosmetics_btn);
    // -----------------
    // Games button
    UI::Button* games_btn = new UI::Button(LoadTexture((TEXTURES_PATH/"games_button.png").u8string().c_str()), {Vector2{button_offset + button_spacing * 2, BUTTON_Y}, 0, 4});
    ui->AddElement("button_games", games_btn);
    // -----------------
    // Settings button
    UI::Button* settings_btn = new UI::Button(LoadTexture((TEXTURES_PATH/"settings_button.png").u8string().c_str()), {Vector2{button_offset + button_spacing * 3, BUTTON_Y}, 0, 4});
    ui->AddElement("button_settings", settings_btn);

    // ========================
    // === SCENE SHENNIGANS ===
    // ========================
    Scene *main_scene = new Scene;
    main_scene->AddUi(ui);
    main_scene->AddObject("rock", rock);

    SceneManager scene_manager;
    scene_manager.AddScene("main_scene", main_scene);
    scene_manager.LoadScene("main_scene");


    while (!WindowShouldClose() && !closeGame){
        // --- Game logic ---
        scene_manager.Update();
        // --- Drawing logic ---
        scene_manager.Draw();
    }

    return 0;
}