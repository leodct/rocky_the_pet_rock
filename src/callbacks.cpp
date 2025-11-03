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
    main_ui->GetElement("button_exit")->Disable();
}

void CloseGameSelectionCallback()
{
    highlight_rock = true;
    games_ui->DisableAll();
    main_ui->GetElement("button_exit")->Enable();
}
