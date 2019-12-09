#include "Save.h"
#include "Macros.h"

uint32_t CoinCount;

// TODO: _0x6065644 ... _0x606564C

static DATA(SaveStore, 0x100);
// Place save data at the end of save memory, while guaranteeing alignment for
// any type in the SaveData struct.
SaveData* const Save = &SaveStore[sizeof(SaveStore) - (sizeof(SaveData) / sizeof(void*)) * sizeof(void*) - (sizeof(SaveData) % sizeof(void*) == 0) * sizeof(void*)];

uint32_t BestMasterSectionTimes[10];
uint32_t BestTaDeathSectionTimes[10];

uint32_t InitSeed;
