#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "globals.hxx"
#include "interfaces.hxx"
#include "rockfallGameController.hpp"

void ExitCallback();
void DefaultCallback();
// Rockfall
void PauseRockfallCallback();
void ResumeRockfallGameCallback();
void RestartRockfallCallback();
void ExitRockfallGameCallback();
void StartRockfallGameCallback();

// --- UI Systems ---
// Coming soon
void IRanOutOfTimeCallback();
void CloseOutOfTimeMenuCallback();
// Cosmetics menu
void OpenCosmeticsMenuCallback();
void CloseCosmeticsMenuCallback();
// Minigame selection menu
void OpenGameSelectionCallback();
void CloseGameSelectionCallback();

#endif