#include "Game/Save.h"
#include "Main/Frame.h"
#include "Lib/Math.h"
#include "Lib/Macros.h"
#include <assert.h>

uint32_t CoinCount;

SaveData Save;

uint32_t BestMasterSectionTimes[10];
uint32_t BestTaDeathSectionTimes[10];

uint32_t InitSeed;

uint32_t PlayStatusChecksum() {
	Save.coinCount = CoinCount & 0xFFFF;

	return
		(CoinCount & 0xFFFF) +
		Save.demoWaitTime +
		Save.gameTime +
		Save.playCount +
		Save.twinCount +
		Save.versusCount +
		Save.initSeed + 1;
}

void InitPlayStatus() {
	CoinCount = 0u;
	Save.coinCount = 0u;
	Save.gameTime = 0u;
	Save.demoWaitTime = 0u;
	Save.versusCount = 0u;
	Save.twinCount = 0u;
	Save.playCount = 0u;
	InitSeed += Rand(1192u) + ScreenTime;
	Save.initSeed = InitSeed;
}

void AddGameTime(uint32_t gameTime) {
	if (Save.playCount < 0xEFFF) {
		Save.playCount++;
		Save.gameTime += gameTime;
	}
}

void SavePlayStatus() {
	Save.coinCount = CoinCount;

	SaveSet(coinCount);
	SaveSet(demoWaitTime);
	SaveSet(gameTime);
	SaveSet(playCount);
	SaveSet(twinCount);
	SaveSet(versusCount);
	Save.initSeed = InitSeed;
	SaveSet(initSeed);
	Save.playStatusChecksum = PlayStatusChecksum();
	SaveSet(playStatusChecksum);
}

bool LoadPlayStatus() {
	SaveGet(coinCount);
	SaveGet(demoWaitTime);
	SaveGet(gameTime);
	SaveGet(playCount);
	SaveGet(twinCount);
	SaveGet(versusCount);

	SaveGet(initSeed);
	InitSeed = Save.initSeed;

	SaveGet(playStatusChecksum);
	uint16_t playStatusChecksum = PlayStatusChecksum();
	bool checksumMatch = Save.playStatusChecksum == playStatusChecksum;
	if (!checksumMatch) {
		InitPlayStatus();
		SavePlayStatus();
	}
	return checksumMatch;
}

void SaveProgramChecksum(uint16_t programChecksum) {
	Save.programChecksum = programChecksum;
	SaveSet(programChecksum);
}

bool LoadProgramChecksum(uint16_t programChecksum) {
	SaveGet(programChecksum);
	return programChecksum == Save.programChecksum;
}
