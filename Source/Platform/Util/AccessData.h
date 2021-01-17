#pragma once

#include "PlatformTypes.h"
#include "Video/Video.h"
#include <stdint.h>

bool OpenData();
void CloseData();

extern ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
extern ROMDATA Color PalSmallText[NUMPALCOLORS_4BPP];

#define TILEROM_SIZE 0x200000
extern uint8_t* TileData;

extern uint8_t* MidiNotes;
