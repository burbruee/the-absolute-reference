#pragma once

#include "PlatformTypes.h"
#include <stdint.h>

typedef enum BgMapTileInfo {
	BGMAPTILEINFO_TILENUM = 0x7FFFF,
	BGMAPTILEINFO_PERTILEPAL = 0x100000,
	BGMAPTILEINFO_BIT21 = 0x200000,
	BGMAPTILEINFO_PALNUM = 0xFF000000
} BgMapTileInfo;

typedef struct BgMapHeader {
	uint32_t tileInfo;
	// TODO: I believe UNK_4 and UNK_6 are map dimensions in terms of VRAM rows/columns; verify this, after analyzing how the code writes VRAM for background maps.
	int16_t UNK_4;
	int16_t UNK_6;
} BgMapHeader;

typedef struct BgMap16{
	BgMapHeader header;
	union {
		uint32_t align;
		uint16_t names[9600];
	};
} BgMap16;

typedef struct BgMap32 {
	BgMapHeader header;
	uint32_t names[9600];
} BgMap32;

typedef struct BgMap {
	BgMapHeader header;
	uint32_t names[1];
} BgMap;

// TODO: Change how these are provided to the code, to the same form as used in
// the original code. For testing with SDL2, loading these at startup will
// work.
extern ROMDATA BgMap16 BgMapSection0;
extern ROMDATA BgMap16 BgMapSection1;
extern ROMDATA BgMap32 BgMapSection2;
extern ROMDATA BgMap16 BgMapSection3;
extern ROMDATA BgMap32 BgMapSection4;
extern ROMDATA BgMap16 BgMapSection5;
extern ROMDATA BgMap16 BgMapSection6;
extern ROMDATA BgMap16 BgMapSection7;
extern ROMDATA BgMap16 BgMapSection8;
extern ROMDATA BgMap16 BgMapSection9;
extern ROMDATA BgMap16 BgMapVersus;

extern ROMDATA BgMap* BgMapTable[11];

#define BGMAP_SECTION(section) (section)
#define BGMAP_VERSUS 10
