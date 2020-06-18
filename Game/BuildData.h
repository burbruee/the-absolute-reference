#pragma once

#include "Object.h"
#include "BgMap.h"
#include "Sequence.h"
#include "Macros.h"
#include <stdint.h>

// TODO: Maybe arrange this into a struct, that gets stored into ROM at
// 0x40000.

// Build data appears to be 0x80 bytes long in the original ROM, then the game
// data follows. Build data starts at 0x40000 in ROM. To support fully-defined
// years, rather than two-character years, BuildDate has to be extended.
extern const char* BuildDate; // TODO: "NA (Month-three-character) (24-hour-time-of-day)"; what does NA mean? "Year N/A" (not available)? NUL-terminated string. Maybe "N/A", because it's only two characters (Y2K problem), and the game was made in 2000.

extern const uint16_t PalDataLength;
extern const uint16_t NumBgMaps;

extern const uint32_t _0x40014;
extern const uint16_t NumPalLists;

extern const int16_t NumObjectData;
extern const int16_t NumObjects;

extern const int16_t _0x4001E; // TODO: Could be the number of palettes (with 256-color palettes maybe counting as one).
extern const uint32_t** PalDataPtr;

extern const BgMap*** BgMapTablePtr;
extern const BgMap** BgMapsPtr;

extern const void*** SequenceDataTablePtr;

extern const ObjectData** ObjectDataTablePtr;

// TODO
//extern const MidiData** MidiDataPtr;

// TODO
//extern const RectData** RectDataTablePtr;

extern const Color*** PalTableDataPtr; // Appears to be a list of NULL-terminated sequences of palette data pointers. NumPalLists is the number of such NULL-terminated lists.

extern const uint8_t** ChecksumDataPtr; // TODO: Analyze function _0x60302C4; it checks against this table, and appears that sound ROM checksum is in index 0.
extern const uint32_t _0x40044; // TODO: Number of checksums? The checksum data appears to have 8 similar-looking 8-byte sequences.

extern const Color*** _0x40048;
