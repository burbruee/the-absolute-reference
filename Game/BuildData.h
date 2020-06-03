#pragma once

#include "BgMap.h"
#include "Macros.h"
#include <stdint.h>

// TODO: Maybe arrange this into a struct, that gets stored into ROM at
// 0x40000.

// Build data appears to be 0x80 bytes long, then the game data follows.
// Build data starts at 0x40000 in ROM.
extern const char BuildDate[16]; // TODO: "NA (Month-three-character) (24-hour-time-of-day)"; what does NA mean? "Year N/A" (not available)? NUL-terminated string. Maybe "N/A", because it's only two characters (Y2K problem), and the game was made in 2000.
extern const uint16_t _0x40010; // 555 in TAP. Check other Psikyo SH-2 games?
extern const uint16_t NumBgMaps;
extern const uint32_t _0x40014; // 0 in TAP. Check other Psikyo SH-2 games?
extern const uint16_t NumPalLists;
extern const int16_t NumObjectData;
extern const int16_t NumObjects;
extern const int16_t _0x4001E; // TODO: Could be the number of palettes (with 256-color palettes maybe counting as one).
extern const uint32_t** PalDataTablePtr;
extern const BgMap*** BgMapTablePtr;
extern const BgMap** BgMapsPtr;
// TODO
//extern const SequenceData*** SequenceDataTablePtr;
extern const ObjectData** ObjectDataTablePtr;
// TODO
//extern const MidiData** MidiDataPtr;
// TODO
//extern const RectData** RectDataTablePtr;
extern const Color*** PalTablePtr; // Appears to be a list of NULL-terminated sequences of palette data pointers. NumPalLists is the number of such NULL-terminated lists.
extern const uint8_t** ChecksumDataTablePtr; // TODO: Analyze function _0x60302C4; it checks against this table, and appears that sound ROM checksum is in index 0. Create a ChecksumData struct for this?
extern const uint32_t _0x40044; // TODO: Number of checksums? The checksum data appears to have 8 similar-looking 8-byte sequences.
extern const Color*** _0x40048;
