#include "Save.h"
#include "Frame.h"
#include "Eeprom.h"
#include "Macros.h"

uint32_t CoinCount;

// TODO: _0x6065644 ... _0x606564C

static DATA(SaveMemory, 0x100);
// Place save data at the end of save memory, while guaranteeing alignment for
// any type in the SaveData struct.
SaveData* const Save = &SaveMemory[lengthof(SaveMemory) - (sizeof(SaveData) / sizeof(void*)) - (sizeof(SaveData) % sizeof(void*) != 0)];

uint32_t BestMasterSectionTimes[10];
uint32_t BestTaDeathSectionTimes[10];

uint32_t InitSeed;

uint32_t PlayStatusChecksum() {
	Save->coinCount = CoinCount & 0xFFFF;

	return
		(CoinCount & 0xFFFF) +
		Save->demoWaitTime +
		Save->gameTime +
		Save->playCount +
		Save->twinCount +
		Save->versusCount +
		Save->initSeed + 1;
}

void InitPlayStatus() {
	CoinCount = 0u;
	Save->coinCount = 0u;
	Save->gameTime = 0u;
	Save->demoWaitTime = 0u;
	Save->versusCount = 0u;
	Save->twinCount = 0u;
	Save->playCount = 0u;
	InitSeed += Rand(1192u) + NumScreenFrames;
	Save->initSeed = InitSeed;
}

void AddGameTime(uint32_t gameTime) {
	if (Save->playCount < 0xEFFF) {
		Save->playCount++;
		Save->gameTime += gameTime;
	}
}

#define SAVE(offset, name) WriteEeprom(offset, &Save->name, sizeof(Save->name))
void SavePlayStatus() {
	Save->coinCount = CoinCount;

	SAVE(0x2Cu, coinCount);
	SAVE(0x30u, demoWaitTime);
	SAVE(0x34u, gameTime);
	SAVE(0x38u, playCount);
	SAVE(0x3Au, twinCount);
	SAVE(0x3Cu, versusCount);
	Save->initSeed = InitSeed;
	SAVE(0x3Eu, initSeed);
	Save->playStatusChecksum = PlayStatusChecksum();
	SAVE(0x40, playStatusChecksum);
}

// NOTE: The original SH-2 code used offsetof(SaveData, fieldName) instead of
// hard-coded offset constants, but we want to maintain binary compatibility
// with TAP's save data, so we have to use hard-coded constants here.

#define LOAD(offset, name) ReadEeprom(offset, &Save->name, sizeof(Save->name));
bool LoadPlayStatus() {
	LOAD(0x2Cu, coinCount);
	LOAD(0x30u, demoWaitTime);
	LOAD(0x34u, gameTime);
	LOAD(0x38u, playCount);
	LOAD(0x3Au, twinCount);
	LOAD(0x3Cu, versusCount);

	LOAD(0x3Eu, initSeed);
	InitSeed = Save->initSeed;

	LOAD(0x40, playStatusChecksum);
	uint16_t playStatusChecksum = PlayStatusChecksum();
	bool checksumMatch = Save->playStatusChecksum == playStatusChecksum;
	if (!checksumMatch) {
		InitPlayStatus();
		SavePlayStatus();
	}
	return checksumMatch;
}

void SaveProgramChecksum(uint16_t programChecksum) {
	WriteEeprom(0xFCu, &programChecksum, sizeof(uint16_t));
	Save->programChecksum = programChecksum;
}

bool LoadProgramChecksum(uint16_t programChecksum) {
	LOAD(0xFCu, programChecksum);
	return programChecksum == Save->programChecksum;
}
