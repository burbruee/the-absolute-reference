#pragma once

#include "Main/MainLoop.h"
#include "Video/VideoDefs.h"
#include "Game/Play/Player.h"
#include "PlatformTypes.h"
#include <stdint.h>

extern ROMDATA Color PalCycleTextPal0[NUMPALCOLORS_4BPP];

extern GameMusic NextGameMusic;

void InitGame();
MainLoopState RunGameLoop();
uint8_t NumVersusRoundsSetting();
void UpdateGameMusic();
