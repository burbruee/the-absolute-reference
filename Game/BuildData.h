#pragma once

#include "Macros.h"
#include <stdint.h>

typedef struct BgMapHeader {
	uint32_t format;
	// TODO: I believe _0x4 and _0x6 are map dimensions in terms of VRAM rows/columns; verify this, after analyzing how the code writes VRAM for background maps.
	uint16_t _0x4;
	uint16_t _0x6;
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

// Build data appears to be 0x80 bytes long, then the game data follows.
extern const char BuildDate[16]; // TODO: "NA (Month-three-character) (24-hour-time-of-day)"; what does NA mean? "Year N/A" (not available)? NUL-terminated string. Maybe "N/A", because it's only two characters (Y2K problem), and the game was made in 2000.
extern const uint16_t _0x40010; // 555 in TAP. Check other Psikyo SH-2 games?
extern const uint16_t NumBgMaps;
extern const uint32_t _0x40014; // 0 in TAP. Check other Psikyo SH-2 games?
extern const uint16_t NumPalLists;
extern const int16_t NumObjectData;
extern const int16_t NumObjects;
extern const int16_t _0x4001E; // TODO: Could be the number of palettes (with 256-color palettes maybe counting as one).
extern const uint32_t **PalDataTablePtr;
extern const BgMap ***BgMapTablePtr;
extern const BgMap **BgMapDataTablePtr;
extern const SequenceEntry ***SequenceTablePtr; // Correct.
extern const ObjectData **ObjectDataTablePtr;
extern const MidiData **MidiDataPtr;
extern const RectData **RectDataTablePtr;
extern const PalTable ***PalTablePtr; // Appears to be a list of NULL-terminated sequences of palette data pointers. NumPalLists is the number of such NULL-terminated lists.
extern const uint8_t **ChecksumDataTablePtr; // TODO: Analyze function _0x60302C4; it checks against this table, and appears that sound ROM checksum is in index 0. Create a ChecksumData struct for this?
extern const uint32_t _0x40044; // TODO: Number of checksums? The checksum data appears to have 8 similar-looking 8-byte sequences.
