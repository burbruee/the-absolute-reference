#pragma once

#include "Main/MainLoop.h"
#include "Video/VideoDefs.h"
#include "Game/Play/Player.h"
#include "PlatformTypes.h"
#include <stdint.h>

extern ROMDATA Color UNK_6032884[NUMPALCOLORS_4BPP];
extern ROMDATA Color UNK_60328C4[NUMPALCOLORS_4BPP];
extern ROMDATA Color PalCycleTextPal0[NUMPALCOLORS_4BPP];

extern GameMusic NextGameMusic;

void InitGame();
MainLoopState RunGameLoop();
uint8_t NumVersusRoundsSetting();
void UpdateGameMusic();
