#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "globals.hxx"
#include "interfaces.hxx"
#include "rockfallGameController.hpp"

void ExitCallback();
void DefaultCallback();
void OpenGameSelectionCallback();
void CloseGameSelectionCallback();
// Rockfall
void PauseRockfallCallback();
void ResumeRockfallGameCallback();
void RestartRockfallCallback();
void ExitRockfallGameCallback();
void StartRockfallGameCallback();

#endif