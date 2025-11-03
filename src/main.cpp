#include "rock.hpp"
#include "scene.hpp"
#include "rockfallGameController.hpp"
#include "interfaces.hxx"
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
    InitUISystems();

    // Resource loading
    LoadCosmetics();
    InitShaders();
    background_texture = LoadTexture((TEXTURES_PATH/"grass.png").u8string().c_str());

    // Variable declarations
    Rock *rock = new Rock(Vector2{0,0});
    camera.target = rock->GetTransform().position;
    rock->SetCosmetic(COSMETICS::RED_BOWTIE);

    // ========================
    // === SCENE SHENNIGANS ===
    // ========================
    Scene *main_scene = new Scene;
    main_scene->AddUi("main_ui", main_ui);
    main_scene->AddUi("games_ui", games_ui);
    main_scene->AddObject("rock", rock);

    Scene *scene_rockfall_game = new Scene;
    RockfallGameController *rf_game_controller = new RockfallGameController();
    scene_rockfall_game->AddObject("game_controller", rf_game_controller);

    SceneManager scene_manager;
    scene_manager.AddScene("main_scene", main_scene);
    scene_manager.AddScene("rockfall_game", scene_rockfall_game);
    scene_manager.LoadScene("main_scene");
    
    //scene_manager.LoadScene("rockfall_game");


    //rf_game_controller->StartGame();
    while (!WindowShouldClose() && !closeGame){
        // --- Game logic ---
        scene_manager.Update();
        rock->SetHighlight(highlight_rock);
        // --- Drawing logic ---
        scene_manager.Draw();
    }

    return 0;
}