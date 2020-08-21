#pragma once

#include "Video/Object.h"
#include "Video/BgMap.h"
#include "Game/Sequence.h"
#include "BuildData/Checksum.h"
#include <stdint.h>

// TODO: Maybe arrange this into a struct, that gets stored into ROM at
// 0x40000.

// Build data appears to be 0x80 bytes long in the original ROM, then the game
// data follows. Build data starts at 0x40000 in ROM. To support fully-defined
// years, rather than two-character years, BuildDate has to be extended.
extern const char* const BuildDate; // TODO: "NA (Month-three-character) (24-hour-time-of-day)"; what does NA mean? "Year N/A" (not available)? NUL-terminated string. Maybe "N/A", because it's only two characters (Y2K problem), and the game was made in 2000.

extern const uint16_t PalDataLength;
extern const uint16_t NumBgMaps;

extern const uint32_t UNK_40014;
extern const uint16_t NumPalLists;

extern const int16_t NumObjectData;
extern const int16_t NumObjects;

extern const int16_t UNK_4001E; // TODO: Could be the number of palettes (with 256-color palettes maybe counting as one).
// TODO: Fix the pointers here.
// BuildChecksumDataPtr and SequenceDataTablePtr are correct.
extern const Color* const PalDataPtr;

extern ROMDATA BgMap** const BgMapTablePtr;
extern ROMDATA BgMap* const BgMapsPtr;

extern ROMDATA void** const SequenceDataTablePtr;

extern ROMDATA ObjectData* const ObjectDataTablePtr;

// TODO
//extern const MidiData** MidiDataPtr;

// TODO
//extern const RectData** RectDataTablePtr;

extern const Color** const PalTableDataPtr; // Appears to be a list of NULL-terminated sequences of palette data pointers. NumPalLists is the number of such NULL-terminated lists.

extern const Checksum* const BuildChecksumDataPtr;
extern const uint32_t UNK_40044; // TODO: Number of checksums? The checksum data appears to have 8 similar-looking 8-byte sequences.

extern const Color** const UNK_40048;
