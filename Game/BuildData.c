// TODO: Replace literals with macros defined by the build system.

#include "BuildData.h"
#include "Pal.h"
#include "BgMap.h"

// TODO: Replace "NA" with the full year (2020, etc.). This is the build date
// as defined in the original TAP ROM. _0x40048 can be removed to make room for
// the two additional characters in BuildDate, to keep the build data under
// 0x80 bytes, as it's unused and redundant. And keep a comment saying this was
// the original BuildDate in TAP, alongside the BuildDate in TGM2.
const char* BuildDate = "NA Dec 12 11:54";

const uint16_t PalDataLength = lengthoffield(PalData, data);

const uint16_t NumBgMaps = lengthof(BgMapTable);

const uint32_t _0x40014 = 0u;
const uint16_t NumPalLists = 19u;

const int16_t NumObjectData = lengthoffield(ObjectDataTable, data);
const int16_t NumObjects = 1682;

const int16_t _0x4001E = 516;

const Color* PalDataPtr = Pal.data;
const BgMap*** BgMapTablePtr = &BgMapTable;

// TODO: Currently the code doesn't ensure the background maps are stored
// adjacently in memory, so this pointer won't work as expected. A solution
// would be to create a generic data array that all the maps are stored into,
// then getting pointers from that array for the background map table.
const BgMap** BgMapsPtr = &BgMapSection2;

const void** SequenceDataTablePtr = SequenceDataTable;
//const ObjectData* ObjectDataTablePtr = (const ObjectData*)Objects.data;
// TODO
//const MidiData* MidiPtr = &Midi;
// TODO
//const RectData* RectDataTablePtr = RectDataTable;
const Color** PalTableDataPtr = PalTable.data;
const Checksum* BuildChecksumDataPtr;// = (const Checksum*)BuildChecksum.checksums;
const uint32_t _0x40044 = 8u;
const Color** _0x40048 = PalTable.data;
