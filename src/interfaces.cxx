#include "interfaces.hxx"

#define middle UI::Label::ALIGNMENT::MIDDLE

UIContainer *main_ui = new UIContainer;
void InitMainUI()
{
    // ===============
    // === MAIN UI ===
    // ===============

    // === BOTTOM SIDE ===

    // -------------------------
    // --- BUTTON SHENNIGANS ---
    float button_spacing = 85;
    float button_offset  = 50;
    const float BUTTON_Y = 580;

    // -----------------
    // Buttons panel
    Transform2D bottom_panel_transform;
    bottom_panel_transform.position = {0, BUTTON_Y - 50};
    bottom_panel_transform.rotation = 0;
    bottom_panel_transform.scale    = 1;
    Color panel_col = {220, 220, 220, 250};
    Color panel_edge_col = {200, 200, 200, 255};
    UI::Panel* buttons_panel = new UI::Panel(bottom_panel_transform, Vector2{WINDOW_SIZE.x, WINDOW_SIZE.y - (BUTTON_Y - 50)}, panel_col, panel_edge_col, 5);
    buttons_panel->SetDrawOrder(-1);
    main_ui->AddElement("buttons_panel", buttons_panel);
    // -----------------
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
    cosmetics_btn->DefineOnPressCallback(OpenCosmeticsMenuCallback);
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




    // === TOP SIDE ===

    // --- Panel ---
    Transform2D top_panel_transform = {{0,0}, 0, 1};
    UI::Panel *top_panel = new UI::Panel(top_panel_transform, Vector2{WINDOW_SIZE.x, 75}, panel_col, panel_edge_col, 5);
    top_panel->SetDrawOrder(-1);
    main_ui->AddElement("top_panel", top_panel);

    // --- Money counter panel ---
    Color money_panel_col = {200, 200, 200, 255};
    Color money_panel_edge_col = {150, 150, 150, 255};
    UI::Panel *money_panel = new UI::Panel({{250, 20}, 0, 1}, {100, 35}, money_panel_col, money_panel_edge_col, 5);
    money_panel->SetDrawOrder(1);
    main_ui->AddElement("balance_display_panel", money_panel);
    // --- Money counter ---
    Transform2D money_counter_transform = {{260, 40}, 0, 1};
    UI::VariableDisplay<int> *balance_display = new UI::VariableDisplay<int>(&balance, money_counter_transform, 20, BLACK);
    balance_display->SetDrawOrder(2);
    main_ui->AddElement("balance_display", balance_display);
    // --- Money icon ---
    UI::ImageDisplay *icon_balance = new UI::ImageDisplay(LoadTexture((TEXTURES_PATH/"rock_token.png").u8string().c_str()), {{210, 15}, 0, 4});
    icon_balance->SetDrawOrder(2);
    main_ui->AddElement("icon_balance", icon_balance);
    
}

UIContainer *games_ui = new UIContainer;
void InitGamesUI()
{
    // =============================
    // === MINIGAME SELECTION UI ===
    // =============================
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
    button_rockfall->DefineOnPressCallback(StartRockfallGameCallback);
    games_ui->AddElement("button_rockfall", button_rockfall);
    
    Transform2D label_rockfall_transform = {{150, 200}, 0, 1};
    UI::Label *label_rockfall = new UI::Label(label_rockfall_transform, "Rockfall", 30, WHITE);
    games_ui->AddElement("label_rockfal", label_rockfall);

    // ------------------------
    // --- ROCK FINDER GAME ---
    //Transform2D button_finder_transform = {{100, 300}, 0, 4};
    //UI::Button *button_rock_finder = new UI::Button(LoadTexture((TEXTURES_PATH/"button_game_rock_finder.png").u8string().c_str()), button_finder_transform);
    //games_ui->AddElement("button_rock_finder", button_rock_finder);

    //Transform2D label_finder_transform = {{150, 300}, 0, 1};
    //UI::Label *label_rock_finder = new UI::Label(label_finder_transform, "Rock finder", 30, WHITE);
    //games_ui->AddElement("label_rock_finder", label_rock_finder);

    // ------------------------------------
    // --- MORE GAMES COMING SOON LABEL ---
    Transform2D label_more_games_transform = {{40, 400}, 0, 1};
    UI::Label *label_more_games = new UI::Label(label_more_games_transform, "More games coming soon...", 25, WHITE);
    games_ui->AddElement("label_more_games", label_more_games);

    games_ui->SetAllVisibilityTo(false);
    games_ui->DisableAll();
}

UIContainer *rockfall_ui = new UIContainer;
void InitRockfallGameUI(){
    // ========================
    // === ROCKFALL GAME UI ===
    // ========================
    Color panel_col = {220, 220, 220, 250};
    Color panel_edge_col = {200, 200, 200, 255};
    // --- Panel ---
    Transform2D top_panel_transform = {{0,0}, 0, 1};
    UI::Panel *top_panel = new UI::Panel(top_panel_transform, Vector2{WINDOW_SIZE.x, 75}, panel_col, panel_edge_col, 5);
    top_panel->SetDrawOrder(-1);
    rockfall_ui->AddElement("top_panel", top_panel);

    //======================
    // === MONEY COUNTER ===
    // --- Money counter panel ---
    Color money_panel_col = {200, 200, 200, 255};
    Color money_panel_edge_col = {150, 150, 150, 255};
    UI::Panel *money_panel = new UI::Panel({{250, 20}, 0, 1}, {100, 35}, money_panel_col, money_panel_edge_col, 5);
    money_panel->SetDrawOrder(1);
    rockfall_ui->AddElement("balance_display_panel", money_panel);
    // --- Money counter ---
    Transform2D money_counter_transform = {{260, 40}, 0, 1};
    UI::VariableDisplay<int> *balance_display = new UI::VariableDisplay<int>(&balance, money_counter_transform, 20, BLACK);
    balance_display->SetDrawOrder(2);
    rockfall_ui->AddElement("balance_display", balance_display);
    // --- Money icon ---
    UI::ImageDisplay *icon_balance = new UI::ImageDisplay(LoadTexture((TEXTURES_PATH/"rock_token.png").u8string().c_str()), {{210, 15}, 0, 4});
    icon_balance->SetDrawOrder(2);
    rockfall_ui->AddElement("icon_balance", icon_balance);

    // =====================
    // === SCORE COUNTER ===
    UI::Panel *score_panel = new UI::Panel({{95, 20}, 0, 1}, {100, 35}, money_panel_col, money_panel_edge_col, 5);
    score_panel->SetDrawOrder(1);
    rockfall_ui->AddElement("score_panel", score_panel);
    UI::VariableDisplay<int> *score_display = new UI::VariableDisplay<int>(&RockfallGameController::GetScoreRef(), {{120, 40}, 0, 1}, 20, BLACK);
    score_display->SetDrawOrder(2);
    rockfall_ui->AddElement("score_display", score_display);
    UI::ImageDisplay *score_image = new UI::ImageDisplay(LoadTexture((TEXTURES_PATH/"rockfall_game/rock_7.png").u8string().c_str()), {{75, 20}, 0, 3});
    score_image->SetDrawOrder(2);
    rockfall_ui->AddElement("score_image", score_image);

    // --- Pause button ---
    UI::Button *button_pause = new UI::Button(LoadTexture((TEXTURES_PATH/"button_return.png").u8string().c_str()), {{35, 35}, 0, 3});
    button_pause->SetDrawOrder(2);
    button_pause->DefineOnPressCallback(PauseRockfallCallback);
    rockfall_ui->AddElement("button_pause", button_pause);
}

UIContainer *rockfall_pause_ui = new UIContainer;
void InitRockfallPauseUI(){
    // ==================
    // === PAUSE MENU ===
    // --- Panel ---
    Color pause_panel_col      = { 245, 225, 210, 225 };
    Color pause_panel_edge_col = { 245, 225, 210, 255 };
    Transform2D pause_panel_transform = {{25, 100}, 0, 1};
    UI::Panel *pause_panel = new UI::Panel(pause_panel_transform, {WINDOW_SIZE.x - pause_panel_transform.position.x * 2, WINDOW_SIZE.y - pause_panel_transform.position.y - pause_panel_transform.position.x}, pause_panel_col, pause_panel_edge_col, 5);
    pause_panel->SetDrawOrder(-1);
    rockfall_pause_ui->AddElement("pause_panel", pause_panel);
    // --- Paused text --
    UI::Label *label_paused = new UI::Label({{WINDOW_SIZE.x / 2, pause_panel_transform.position.y + 40}, 0, 1}, "- Game Paused -", 35, BLACK, middle);
    rockfall_pause_ui->AddElement("label_paused", label_paused);
    // --- Resume button ---
    UI::Button *button_resume = new UI::Button(LoadTexture((TEXTURES_PATH/"wide_button_green.png").u8string().c_str()), {{WINDOW_SIZE.x / 2, 225}, 0, 4});
    button_resume->DefineOnPressCallback(ResumeRockfallGameCallback);
    rockfall_pause_ui->AddElement("button_resume", button_resume);
    // --- Resume button label ---
    UI::Label *label_button_resume = new UI::Label(button_resume->GetTransform(), "Resume", 30, WHITE, middle);
    label_button_resume->SetDrawOrder(1);
    rockfall_pause_ui->AddElement("label_button_resume", label_button_resume);
    // --- Restart button ---
    UI::Button *button_restart = new UI::Button(LoadTexture((TEXTURES_PATH/"wide_button_yellow.png").u8string().c_str()), {{WINDOW_SIZE.x / 2, 300}, 0, 4});
    button_restart->DefineOnPressCallback(RestartRockfallCallback);
    rockfall_pause_ui->AddElement("button_restrt", button_restart);
    // --- Restart button label ---
    UI::Label *label_button_restart = new UI::Label(button_restart->GetTransform(), "Restart", 30, WHITE, middle);
    label_button_restart->SetDrawOrder(1);
    rockfall_pause_ui->AddElement("label_button_restart", label_button_restart);
    // --- Exit button ---
    UI::Button *button_exit = new UI::Button(LoadTexture((TEXTURES_PATH/"wide_button_red.png").u8string().c_str()), {{WINDOW_SIZE.x / 2, 375}, 0, 4});
    button_exit->DefineOnPressCallback(ExitRockfallGameCallback);
    rockfall_pause_ui->AddElement("button_exit", button_exit);
    // --- Button exit label ---
    UI::Label *label_button_exit = new UI::Label(button_exit->GetTransform(), "Exit", 30, WHITE, middle);
    label_button_exit->SetDrawOrder(1);
    rockfall_pause_ui->AddElement("label_button_exit", label_button_exit);
}

UIContainer *rockfall_game_over_ui = new UIContainer;
void InitRockfallGameOverUI(){
    // ======================
    // === GAME OVER MENU ===
    // --- Panel ---
    Color pause_panel_col      = { 245, 225, 210, 225 };
    Color pause_panel_edge_col = { 245, 225, 210, 255 };
    Transform2D pause_panel_transform = {{25, 100}, 0, 1};
    UI::Panel *pause_panel = new UI::Panel(pause_panel_transform, {WINDOW_SIZE.x - pause_panel_transform.position.x * 2, WINDOW_SIZE.y - pause_panel_transform.position.y - pause_panel_transform.position.x}, pause_panel_col, pause_panel_edge_col, 5);
    pause_panel->SetDrawOrder(-1);
    rockfall_game_over_ui->AddElement("pause_panel", pause_panel);
    
    // --- Score label ---
    int score_font_size = 25;
    std::string score_text = "Score: ";
    Transform2D score_transform = {{WINDOW_SIZE.x / 2 + (MeasureTextEx(GetFontDefault(), score_text.c_str(), score_font_size, 1).x) / 2, WINDOW_SIZE.y / 3.15f}, 0, 1};
    UI::Label *label_score = new UI::Label(score_transform, score_text.c_str(), score_font_size, BLACK, UI::Label::ALIGNMENT::RIGHT);
    rockfall_game_over_ui->AddElement("label_score", label_score);
    // --- Score display ---
    UI::VariableDisplay<int> *score_display = new UI::VariableDisplay<int>(&RockfallGameController::GetScoreRef(), score_transform, score_font_size, BLACK, UI::Label::ALIGNMENT::LEFT);
    rockfall_game_over_ui->AddElement("score_display", score_display);
    // --- High score label ---
    int high_score_font_size = 20;
    std::string high_score_text = "All time best: ";
    Transform2D high_score_transform = {{WINDOW_SIZE.x / 2 + (MeasureTextEx(GetFontDefault(), high_score_text.c_str(), high_score_font_size, 1).x / 2), WINDOW_SIZE.y / 2.75f}, 0, 1};
    UI::Label *label_high_score = new UI::Label(high_score_transform, high_score_text.c_str(), high_score_font_size, BLACK, UI::Label::ALIGNMENT::RIGHT);
    rockfall_game_over_ui->AddElement("label_high_score", label_high_score);
    // --- High score display ---
    UI::VariableDisplay<int> *high_score_display = new UI::VariableDisplay<int>(&rockfall_high_score, high_score_transform, high_score_font_size, BLACK, UI::Label::ALIGNMENT::LEFT);
    rockfall_game_over_ui->AddElement("high_score_display", high_score_display);


    // --- Paused text --
    UI::Label *label_paused = new UI::Label({{WINDOW_SIZE.x / 2, pause_panel_transform.position.y + 40}, 0, 1}, "- Game Over -", 35, BLACK, middle);
    rockfall_game_over_ui->AddElement("label_paused", label_paused);
    // --- Restart button ---
    UI::Button *button_restart = new UI::Button(LoadTexture((TEXTURES_PATH/"wide_button_yellow.png").u8string().c_str()), {{WINDOW_SIZE.x / 2, 300}, 0, 4});
    button_restart->DefineOnPressCallback(RestartRockfallCallback);
    rockfall_game_over_ui->AddElement("button_restrt", button_restart);
    // --- Restart button label ---
    UI::Label *label_button_restart = new UI::Label(button_restart->GetTransform(), "Restart", 30, WHITE, middle);
    label_button_restart->SetDrawOrder(1);
    rockfall_game_over_ui->AddElement("label_button_restart", label_button_restart);
    // --- Exit button ---
    UI::Button *button_exit = new UI::Button(LoadTexture((TEXTURES_PATH/"wide_button_red.png").u8string().c_str()), {{WINDOW_SIZE.x / 2, 375}, 0, 4});
    button_exit->DefineOnPressCallback(ExitRockfallGameCallback);
    rockfall_game_over_ui->AddElement("button_exit", button_exit);
    // --- Button exit label ---
    UI::Label *label_button_exit = new UI::Label(button_exit->GetTransform(), "Exit", 30, WHITE, middle);
    label_button_exit->SetDrawOrder(1);
    rockfall_game_over_ui->AddElement("label_button_exit", label_button_exit);
}

UIContainer *out_of_time_menu = new UIContainer;
void InitOutOfTimeMenu(){
    // ======================
    // === GAME OVER MENU ===
    // --- Panel ---
    Color panel_col      = { 245, 225, 210, 225 };
    Color panel_edge_col = { 245, 225, 210, 255 };
    Transform2D panel_transform = {{WINDOW_SIZE.x / 10, WINDOW_SIZE.y / 5}, 0, 1};
    UI::Panel *panel = new UI::Panel(panel_transform, {WINDOW_SIZE.x - panel_transform.position.x * 2, WINDOW_SIZE.y - panel_transform.position.y - panel_transform.position.x}, panel_col, panel_edge_col, 5);
    panel->SetDrawOrder(-1);
    out_of_time_menu->AddElement("panel", panel);

    UI::Label *label_notime = new UI::Label({{WINDOW_SIZE.x / 2 - 100, WINDOW_SIZE.y / 2}, 0, 1}, "Coming Soon...", 40, BLACK);
    out_of_time_menu->AddElement("label_notime", label_notime);

    UI::Label *label_notime2 = new UI::Label({{WINDOW_SIZE.x / 2 - 100, WINDOW_SIZE.y / 1.8f}, 0, 1}, "(I ran out of time)", 25, BLACK);
    out_of_time_menu->AddElement("label_notime2", label_notime2);

    UI::Button *button_awman = new UI::Button(LoadTexture((TEXTURES_PATH/"wide_button_green.png").u8string().c_str()), {{WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 1.5f}, 0, 4});
    button_awman->DefineOnPressCallback(CloseOutOfTimeMenuCallback);
    out_of_time_menu->AddElement("button_awman", button_awman);

    UI::Label *label_awman = new UI::Label({{WINDOW_SIZE.x / 2 - 50, WINDOW_SIZE.y / 1.5f}, 0, 1}, "aw man", 30, BLACK);
    out_of_time_menu->AddElement("label_awman", label_awman);  

    out_of_time_menu->SetDrawOrder(5); 
    out_of_time_menu->DisableAll();
    out_of_time_menu->SetAllVisibilityTo(false);
}

UIContainer *cosmetics_selection_menu = new UIContainer;
void InitCosmeticsMenu(){
    // PANEL
    Transform2D panel_transform  = {{20, 100}, 0, 1};
    Vector2     panel_dimensions = {WINDOW_SIZE.x - 2*panel_transform.position.x, WINDOW_SIZE.y - 2*panel_transform.position.y - 40};
    Color       panel_col        = { 200, 225, 210, 230 };
    Color       panel_edge_col   = { 180, 205, 190, 255 };
    UI::Panel  *panel            = new UI::Panel(panel_transform, panel_dimensions, panel_col, panel_edge_col, 5);
    panel->SetDrawOrder(-1);
    cosmetics_selection_menu->AddElement("panel", panel);
    // RETURN BUTTON
    Transform2D button_return_transform = {{35, 115}, 0, 3};
    UI::Button *button_return    = new UI::Button(LoadTexture((TEXTURES_PATH/"button_return.png").u8string().c_str()), button_return_transform);
    button_return->DefineOnPressCallback(CloseCosmeticsMenuCallback);
    cosmetics_selection_menu->AddElement("button_return", button_return);
    // COSMETICS LABEL
    UI::Label *label_cosmetics = new UI::Label({{WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 4}, 0, 1}, "- Cosmetics -", 40, BLACK, middle);
    cosmetics_selection_menu->AddElement("label_cosmetics", label_cosmetics);
    
    cosmetics_selection_menu->SetAllVisibilityTo(false);
    cosmetics_selection_menu->DisableAll();
    cosmetics_selection_menu->SetDrawOrder(1);
}

void InitRockfallUI(){
    InitRockfallGameUI();
    InitRockfallPauseUI();
    rockfall_pause_ui->SetAllVisibilityTo(false);
    rockfall_pause_ui->DisableAll();
    rockfall_pause_ui->SetDrawOrder(1);
    InitRockfallGameOverUI();
    rockfall_game_over_ui->SetAllVisibilityTo(false);
    rockfall_game_over_ui->DisableAll();
    rockfall_game_over_ui->SetDrawOrder(1);
}

void InitUISystems()
{
    InitMainUI();
    InitGamesUI();
    InitRockfallUI();
    InitOutOfTimeMenu();
    InitCosmeticsMenu();
}
