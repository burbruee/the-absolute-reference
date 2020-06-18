#pragma once

#include "PlatformTypes.h"
#include "VideoDefs.h"

typedef struct PalData {
	char header[8];
	Color data[555u * NUMPALCOLORS_4BPP];
} PalData;

extern ROMDATA PalData Pal;
#define PALPTR(i) ((const Color*)&Pal.data[(i) * NUMPALCOLORS_4BPP])

typedef struct PalTableData {
	char header[8];
	Color* data[133];
} PalTableData;

// The palettes table contains palette lists; each list in the table is an
// array of one or more palette pointers, terminated by a NULL pointer, each
// list directly following the previous in the table. BuildData.h's NumPalLists
// contains the number of palette lists in this table.
extern ROMDATA PalTableData PalTable;

// TODO: Define all palettes here.

#define PAL_BGMAPSECTION2 PALPTR(0x2)
#define PAL_BGMAPSECTION5 PALPTR(0x12)
#define PAL_BGMAPSECTION8 PALPTR(0x22)
#define PAL_BGMAPSECTION3 PALPTR(0x32)
#define PAL_BGMAPSECTION1 PALPTR(0x42)
#define PAL_BGMAPSECTION0 PALPTR(0x52)
#define PAL_BGMAPSECTION4 PALPTR(0x62)
#define PAL_BGMAPSECTION6 PALPTR(0x72)
#define PAL_BGMAPSECTION7 PALPTR(0x82)
#define PAL_BGMAPSECTION9 PALPTR(0x92)
#define PAL_BGMAPVERSUS PALPTR(0xA2)

#define PAL_WHITEFIELDBORDER PALPTR(0xFA)
#define PAL_DOUBLESFIELDBORDER PALPTR(0x10A)
#define PAL_NORMALFIELDBORDER PALPTR(0x11A)
#define PAL_REDFIELDBORDER PALPTR(0x12A)
#define PAL_GREENFIELDBORDER PALPTR(0x13A)
#define PAL_BLUEFIELDBORDER PALPTR(0x14A)

// TODO
#define PALTABLE_PALNUM48BLOCK PALPTR(0x15A)

#define PALTABLE_IBLOCK PALPTR(0x168)
#define PALTABLE_ZBLOCK PALPTR(0x172)
#define PALTABLE_SBLOCK PALPTR(0x17C)
#define PALTABLE_JBLOCK PALPTR(0x186)
#define PALTABLE_LBLOCK PALPTR(0x190)
#define PALTABLE_OBLOCK PALPTR(0x19A)
#define PALTABLE_TBLOCK PALPTR(0x1A4)
#define PALTABLE_GARBAGEBLOCK PALPTR(0x1AE)

// TODO
#define PALTABLE_PALNUM138BLOCK PALPTR(0x1B8)

#define PAL_TEXTOVERLAYBG PALPTR(0x20E)

#define PAL_STAFF PALPTR(0x210)
#define PAL_GRANDMASTER PALPTR(0x213)

#define PAL_MODESELECTED PALPTR(0x229)
#define PAL_MODENOTSELECTED PALPTR(0x22A)
