#pragma once

#include "PlatformTypes.h"
#include "VideoDefs.h"

typedef struct PalDataTable {
	char header[8];
	Color data[0x22B * NUMPALCOLORS_4BPP];
} PalDataTable;

extern ROMDATA PalDataTable Pals;
#define PALPTR(i) ((const Color*)&Pals.data[(i) * NUMPALCOLORS_4BPP])

// TODO: Define all palettes here.

#define PAL_WHITEFIELDBORDER PALPTR(0xFA)
#define PAL_DOUBLESFIELDBORDER PALPTR(0x10A)
#define PAL_NORMALFIELDBORDER PALPTR(0x11A)
#define PAL_REDFIELDBORDER PALPTR(0x12A)
#define PAL_GREENFIELDBORDER PALPTR(0x13A)
#define PAL_BLUEFIELDBORDER PALPTR(0x14A)

#define PAL_TEXTOVERLAYBG PALPTR(0x20E)

#define PAL_MODESELECTED PALPTR(0x229)
#define PAL_MODENOTSELECTED PALPTR(0x22A)
