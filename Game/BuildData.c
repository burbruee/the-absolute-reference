#include "BuildData.h"
#include "Pal.h"
#include "BgMap.h"
#include "Checksum.h"

// TODO: Implement a macro for BuildDate generated from the build system with
// the actual date of building. Replace "NA" with the full year (2020, etc.).
const char* BuildDate = "NA Dec 12 11:54";

const uint16_t _0x40010 = 555u;
const uint16_t NumBgMaps = 11u;
const uint32_t _0x40014 = 0u;
const uint16_t NumPalLists = 19u;
const int16_t NumObjectData = 0x6DC;
const int16_t NumObjects = 1682;
const int16_t _0x4001E = 516;
const Color** PalDataTablePtr = &Pals.data;
const BgMap*** BgMapTablePtr = &BgMapTable;

// TODO: Currently the code doesn't ensure the background maps are stored
// adjacently in memory, so this pointer won't work as expected. A solution
// would be to create a generic data array that all the maps are stored into,
// then getting pointers from that array for the background map table.
const BgMap** BgMapsPtr = &BgMapSection2;

const void*** SequenceDataTablePtr = &SequenceDataTable;
const ObjectData** ObjectDataTablePtr = &Objects.data;
// TODO
//const MidiData** MidiDataPtr;
// TODO
//const RectData** RectDataTablePtr;
const Color*** PalTablePtr = &PalsTable.palettes;
const uint8_t** ChecksumDataPtr = &Checksum.data;
const uint32_t _0x40044 = 8u;
const Color*** _0x40048 = &PalsTable.palettes;