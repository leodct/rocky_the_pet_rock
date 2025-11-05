#include "callbacks.hpp"

void ExitCallback()
{
    closeGame = true;
}

void DefaultCallback()
{
    std::cout << "Default button callback!" << std::endl;
}

void OpenGameSelectionCallback()
{
    highlight_rock = false;
    games_ui->EnableAll();
    games_ui->SetAllVisibilityTo(true);
    main_ui->GetElement("button_exit")->Disable();
}

void CloseGameSelectionCallback()
{
    highlight_rock = true;
    games_ui->DisableAll();
    games_ui->SetAllVisibilityTo(false);
    main_ui->GetElement("button_exit")->Enable();
}

void PauseRockfallCallback()
{
    RockfallGameController::Pause();
    rockfall_pause_ui->EnableAll();
    rockfall_pause_ui->SetAllVisibilityTo(true);
    rockfall_ui->DisableAll();
}

void ResumeRockfallGameCallback()
{
    RockfallGameController::UnPause();
    rockfall_pause_ui->DisableAll();
    rockfall_pause_ui->SetAllVisibilityTo(false);
    rockfall_ui->EnableAll();
}

void RestartRockfallCallback()
{
    RockfallGameController::RestartGame();
    rockfall_pause_ui->DisableAll();
    rockfall_pause_ui->SetAllVisibilityTo(false);
    rockfall_game_over_ui->DisableAll();
    rockfall_game_over_ui->SetAllVisibilityTo(false);
    rockfall_ui->EnableAll();
}

void ExitRockfallGameCallback()
{
    RockfallGameController::EndGame();
    rockfall_pause_ui->DisableAll();
    rockfall_ui->DisableAll();
    scene_manager.LoadScene("main_scene");
    main_ui->GetElement("button_exit")->Enable();
}

void StartRockfallGameCallback()
{
    games_ui->DisableAll();
    games_ui->SetAllVisibilityTo(false);
    scene_manager.LoadScene("rockfall_game");
    RockfallGameController::StartGame();
    rockfall_pause_ui->DisableAll();
    rockfall_pause_ui->SetAllVisibilityTo(false);
    rockfall_game_over_ui->DisableAll();
    rockfall_game_over_ui->SetAllVisibilityTo(false);
    rockfall_ui->EnableAll();
}
