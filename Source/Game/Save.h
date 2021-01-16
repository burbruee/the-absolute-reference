#pragma once

#include "Game/Play/Ranking.h"
#include "Eeprom/Eeprom.h"
#include <stdint.h>

extern uint32_t CoinCount;

// TODO: UNK_6065644 ... UNK_606564C. Might be save-related.

typedef struct SaveData {
	uint32_t header[44 / sizeof(uint32_t)]; // TODO: Investigate what length the header should be. For now, this is the entirety of data that precedes the first field of the save data struct.
	uint32_t coinCount;
	uint32_t demoWaitTime;
	uint32_t gameTime;
	uint16_t playCount;
	uint16_t twinCount;
	uint16_t versusCount;
	uint16_t initSeed;
	uint16_t playStatusChecksum;
	Ranking rankings[NUMRANKINGS];
	Ranking doublesLevelRankings[NUMRANKINGPLACES];
	uint16_t masterMedalRankings[NUMRANKINGPLACES];
	uint16_t rankingsChecksum;
	uint16_t programChecksum;
} SaveData;
extern SaveData Save;

// When accessing EEP-ROM to get/set save data, use these.
#define SaveSet(m) EepromSet((uint8_t)offsetof(SaveData, m), &Save.m, sizeoffield(SaveData, m))
#define SaveGet(m) EepromGet((uint8_t)offsetof(SaveData, m), &Save.m, sizeoffield(SaveData, m))

extern uint32_t BestMasterSectionTimes[10];
extern uint32_t BestTaDeathSectionTimes[10];

extern uint32_t InitSeed;

uint32_t PlayStatusChecksum();

void InitPlayStatus();

void AddGameTime(uint32_t gameTime);

void SavePlayStatus();

bool LoadPlayStatus();

void SaveProgramChecksum(uint16_t programChecksum);

bool LoadProgramChecksum(uint16_t programChecksum);
