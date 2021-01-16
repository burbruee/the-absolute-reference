// TODO: Replace literals with macros defined by the build system.

#include "BuildData/BuildData.h"
#include "Lib/Macros.h"
#include "Video/Pal.h"
#include "Video/BgMap.h"
#include "Sound/Sound.h"

// TODO: Replace "NA" with the full year (2020, etc.). This is the build date
// as defined in the original TAP ROM. UNK_40048 can be removed to make room for
// the two additional characters in BuildDate, to keep the build data under
// 0x80 bytes, as it's unused and redundant. And keep a comment saying this was
// the original BuildDate in TAP, alongside the BuildDate in TGM2.
const char* const BuildDate = "NA Dec 12 11:54";

const uint16_t PalDataLength = lengthoffield(PalData, data);

const uint16_t NumBgMaps = lengthof(BgMapTable);

const uint32_t UNK_40014 = 0u;
const uint16_t NumPalLists = 19u;

const int16_t NumObjectData = lengthoffield(ObjectDataTable, data);
const int16_t NumObjects = 1682;

const int16_t UNK_4001E = 516;

const Color* const PalDataPtr = Pal.data;

ROMDATA BgMap** const BgMapTablePtr = BgMapTable;

// TODO: Currently the code doesn't ensure the background maps are stored
// adjacently in memory, so this pointer won't work as expected. A solution
// would be to create a generic data array that all the maps are stored into,
// then getting pointers from that array for the background map table.
ROMDATA BgMap* const BgMapsPtr = (ROMDATA BgMap*)&BgMapSection2;

ROMDATA void** const SequenceDataTablePtr = SequenceDataTable;
ROMDATA ObjectData* const ObjectDataTablePtr = Objects.data;
ROMDATA MidiData* const MidiPtr = &Midi;
// TODO
//ROMDATA RectData* const RectDataTablePtr = RectDataTable;
const Color** const PalTableDataPtr = PalTable.data;
const Checksum* const BuildChecksumDataPtr = BuildChecksum.checksums;
const uint32_t UNK_40044 = 8u;
const Color** const UNK_40048 = PalTable.data;

const char* const DeveloperWebsite = "http://www.arika.co.jp";
