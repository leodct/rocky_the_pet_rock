#ifndef INTERFACES_H
#define INTERFACES_H

#include "globals.hxx"
#include "UI.hpp"
#include "callbacks.hpp"
#include "rockfallGameController.hpp"
extern UIContainer* main_ui;
extern UIContainer* games_ui;
extern UIContainer* rockfall_ui;
extern UIContainer* rockfall_pause_ui;

void InitUISystems();
#endif