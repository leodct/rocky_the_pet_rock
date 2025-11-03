#include "interfaces.hxx"


UIContainer *main_ui = new UIContainer;
void InitMainUI()
{
    // ===============
    // === MAIN UI ===
    // ===============

    // -------------------------
    // --- BUTTON SHENNIGANS ---
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
    main_ui->AddElement("buttons_panel", buttons_panel);
    // Exit button
    UI::Button* exit_btn =  new UI::Button(LoadTexture((TEXTURES_PATH/"button_exit.png").u8string().c_str()), {Vector2{35, 35}, 0, 3});
    exit_btn->DefineOnPressCallback(ExitCallback);
    main_ui->AddElement("button_exit", exit_btn);
    // -----------------
    // Shop button
    UI::Button* shop_btn = new UI::Button(LoadTexture((TEXTURES_PATH/"button_shop.png").u8string().c_str()), {Vector2{button_offset, BUTTON_Y}, 0, 4});
    main_ui->AddElement("button_shop", shop_btn);
    // -----------------
    // Cosmetics button
    UI::Button* cosmetics_btn = new UI::Button(LoadTexture((TEXTURES_PATH/"button_cosmetics.png").u8string().c_str()), {Vector2{button_offset + button_spacing * 1, BUTTON_Y}, 0, 4});
    main_ui->AddElement("button_cosmetics", cosmetics_btn);
    // -----------------
    // Games button
    UI::Button* games_btn = new UI::Button(LoadTexture((TEXTURES_PATH/"button_games.png").u8string().c_str()), {Vector2{button_offset + button_spacing * 2, BUTTON_Y}, 0, 4});
    games_btn->DefineOnPressCallback(OpenGameSelectionCallback);
    main_ui->AddElement("button_games", games_btn);
    // -----------------
    // Settings button
    UI::Button* settings_btn = new UI::Button(LoadTexture((TEXTURES_PATH/"button_settings.png").u8string().c_str()), {Vector2{button_offset + button_spacing * 3, BUTTON_Y}, 0, 4});
    main_ui->AddElement("button_settings", settings_btn);
    
}

UIContainer *games_ui = new UIContainer;
void InitGamesUI()
{
    games_ui->SetDrawOrder(main_ui->GetDrawOrder() + 1);

    Transform2D panel_transform  = {{25, 25}, 0, 1};
    Vector2     panel_size       = {WINDOW_SIZE.x - panel_transform.position.x * 2, WINDOW_SIZE.y - 150};
    Color       panel_color      = { 175, 175, 100, 235 };
    Color       panel_edge_color = { 200, 200, 125, 255 };
    UI::Panel *selection_panel = new UI::Panel(panel_transform, panel_size, panel_color, panel_edge_color, 5);
    selection_panel->SetDisplayState(false);
    selection_panel->SetDrawOrder(-1);
    games_ui->AddElement("panel", selection_panel);

    // -------------------
    // --- GAMES LABEL ---
    Transform2D label_games_transform = {{66, 100}, 0, 1};
    UI::Label *label_games = new UI::Label(label_games_transform, "- GAMES -", 50, BLACK);
    games_ui->AddElement("label_games", label_games);

    // ------------------------
    // --- EXIT MENU BUTTON ---
    Transform2D return_button_transform = {{33, 33}, 0, 3};
    UI::Button *button_return = new UI::Button(LoadTexture((TEXTURES_PATH/"button_return.png").u8string().c_str()), return_button_transform);
    button_return->DefineOnPressCallback(CloseGameSelectionCallback);
    games_ui->AddElement("button_return", button_return);

    // ---------------------
    // --- ROCKFALL GAME ---
    Transform2D button_rockfall_transform = {{100, 200}, 0, 4};
    UI::Button *button_rockfall = new UI::Button(LoadTexture((TEXTURES_PATH/"button_game_rockfall.png").u8string().c_str()), button_rockfall_transform);
    games_ui->AddElement("button_rockfall", button_rockfall);
    
    Transform2D label_rockfall_transform = {{150, 200}, 0, 1};
    UI::Label *label_rockfall = new UI::Label(label_rockfall_transform, "Rockfall", 30, WHITE);
    games_ui->AddElement("label_rockfal", label_rockfall);

    // ------------------------
    // --- ROCK FINDER GAME ---
    Transform2D button_finder_transform = {{100, 300}, 0, 4};
    UI::Button *button_rock_finder = new UI::Button(LoadTexture((TEXTURES_PATH/"button_game_rock_finder.png").u8string().c_str()), button_finder_transform);
    games_ui->AddElement("button_rock_finder", button_rock_finder);

    Transform2D label_finder_transform = {{150, 300}, 0, 1};
    UI::Label *label_rock_finder = new UI::Label(label_finder_transform, "Rock finder", 30, WHITE);
    games_ui->AddElement("label_rock_finder", label_rock_finder);

    // ------------------------------------
    // --- MORE GAMES COMING SOON LABEL ---
    Transform2D label_more_games_transform = {{40, 400}, 0, 1};
    UI::Label *label_more_games = new UI::Label(label_more_games_transform, "More games coming soon...", 25, WHITE);
    games_ui->AddElement("label_more_games", label_more_games);

    games_ui->DisableAll();
}

void InitUISystems()
{
    InitMainUI();
    InitGamesUI();
}
