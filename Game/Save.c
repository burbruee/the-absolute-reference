#include "Save.h"
#include "Frame.h"
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
	InitSeed += Rand(1192u) + ScreenTime;
	Save->initSeed = InitSeed;
}

void AddGameTime(uint32_t gameTime) {
	if (Save->playCount < 0xEFFF) {
		Save->playCount++;
		Save->gameTime += gameTime;
	}
}

void SavePlayStatus() {
	Save->coinCount = CoinCount;

	WriteSave(coinCount);
	WriteSave(demoWaitTime);
	WriteSave(gameTime);
	WriteSave(playCount);
	WriteSave(twinCount);
	WriteSave(versusCount);
	Save->initSeed = InitSeed;
	WriteSave(initSeed);
	Save->playStatusChecksum = PlayStatusChecksum();
	WriteSave(playStatusChecksum);
}

bool LoadPlayStatus() {
	ReadSave(coinCount);
	ReadSave(demoWaitTime);
	ReadSave(gameTime);
	ReadSave(playCount);
	ReadSave(twinCount);
	ReadSave(versusCount);

	ReadSave(initSeed);
	InitSeed = Save->initSeed;

	ReadSave(playStatusChecksum);
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
	ReadSave(programChecksum);
	return programChecksum == Save->programChecksum;
}
