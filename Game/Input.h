#pragma once

#include "HwInput.h"
#include <stdbool.h>

extern bool PauseAllowed;

void InitCredits();
void UpdateInputs();
bool CanStart(PlayerNum playerNum, bool noDouble); // If noDouble is true, double credit coin mode is disabled.
void CheckBuyGame(PlayerNum playerNum, bool oneCreditPerGame);
