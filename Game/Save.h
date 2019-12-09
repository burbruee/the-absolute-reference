#pragma once

#include "Ranking.h"
#include <stdint.h>

extern uint32_t CoinCount;

// TODO: _0x6065644 ... _0x606564C. Might be save-related.

// This save data code isn't directly equivalent to TAP's code, because this
// code is guaranteed to place the save data at the end of the struct. Since
// the C standard allows the compiler to reorder struct fields, there's no
// guarantee that the save data would be at the end of the struct if there was
// a header space placed before the fields, which wouldn't guarantee the sort
// of memory layout that's compatible with the PsikyoSH. For example, the
// PsikyoSH appears to store a region setting at the start of EEPROM, which
// must be guaranteed to not clash with the game's save data.
//
// TAP directly stores the save data in a 256-byte chunk of memory, accessing
// offsets in it, instead of the pointer indirection used here.
typedef struct SaveData {
	uint32_t coinCount;
	uint32_t demoWaitTime;
	uint32_t gameTime;
	uint16_t playCount;
	uint16_t twinCount;
	uint16_t versusCount;
	uint16_t initSeed;
	uint16_t playStatusChecksum;
	RankingData rankings[NUMRANKINGS];
	RankingData doublesLevelRankings[NUMRANKINGPLACES];
	uint16_t masterMedalRankings[NUMRANKINGPLACES];
	uint16_t rankingsChecksum;
	uint16_t programChecksum;
} SaveData;
extern SaveData* const Save;

extern uint32_t BestMasterSectionTimes[10];
extern uint32_t BestTaDeathSectionTimes[10];

extern uint32_t InitSeed;