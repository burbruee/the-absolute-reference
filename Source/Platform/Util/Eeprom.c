#include "Eeprom/Eeprom.h"
#include "Eeprom/Setting.h"
#include "Game/Save.h"
#include "Lib/Macros.h"
#include "HwData.h"
#include "Platform/Util/EepromData.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define EEPROM_coinCount 0x2C
#define EEPROM_demoWaitTime 0x30
#define EEPROM_gameTime 0x34
#define EEPROM_playCount 0x38
#define EEPROM_twinCount 0x3A
#define EEPROM_versusCount 0x3C
#define EEPROM_initSeed 0x3E
#define EEPROM_playStatusChecksum 0x40
#define EEPROM_rankings 0x44
#define EEPROM_doublesLevelRankings 0xDC
#define EEPROM_masterMedalRankings 0xF4
#define EEPROM_rankingsChecksum 0xFA
#define EEPROM_programChecksum 0xFC

#define EEPROM_SET8(offset, value) \
do { \
	EepromData[(offset)] = *(uint8_t*)(value) & 0xFFu; \
} while(false)

#define EEPROM_SET16(offset, value) \
do { \
	EepromData[(offset) + 0] = (*(uint16_t*)(value) >> 8) & 0xFFu; \
	EepromData[(offset) + 1] = (*(uint16_t*)(value) >> 0) & 0xFFu; \
} while(false)

#define EEPROM_SET32(offset, value) \
do { \
	EepromData[(offset) + 0] = (*(uint32_t*)(value) >> 24) & 0xFFu; \
	EepromData[(offset) + 1] = (*(uint32_t*)(value) >> 16) & 0xFFu; \
	EepromData[(offset) + 2] = (*(uint32_t*)(value) >>  8) & 0xFFu; \
	EepromData[(offset) + 3] = (*(uint32_t*)(value) >>  0) & 0xFFu; \
} while(false)

#define EEPROM_SET(offset, value, size) \
do { \
	switch ((size)) { \
	case sizeof(uint32_t): EEPROM_SET32((offset), (value)); break; \
	case sizeof(uint16_t): EEPROM_SET16((offset), (value)); break; \
	case sizeof(uint8_t) : EEPROM_SET8((offset), (value)); break; \
	default: EEPROM_SET32((offset), (value)); break; \
	} \
} \
while (false)

#define EEPROM_GET8(offset, value) \
do { \
	*(uint8_t*)(value) = EepromData[(offset)]; \
} while (false)

#define EEPROM_GET16(offset, value) \
do { \
	*(uint16_t*)(value) = \
		((uint16_t)EepromData[(offset) + 0] << 8) | \
		((uint16_t)EepromData[(offset) + 1] << 0); \
} while (false)

#define EEPROM_GET32(offset, value) \
do { \
	*(uint32_t*)(value) = \
		((uint32_t)EepromData[(offset) + 0] << 24) | \
		((uint32_t)EepromData[(offset) + 1] << 16) | \
		((uint32_t)EepromData[(offset) + 2] <<  8) | \
		((uint32_t)EepromData[(offset) + 3] <<  0); \
} while (false)

#define EEPROM_GET(offset, value, size) \
do { \
	switch ((size)) { \
	case sizeof(uint32_t): EEPROM_GET32((offset), (value)); break; \
	case sizeof(uint16_t): EEPROM_GET16((offset), (value)); break; \
	case sizeof(uint8_t) : EEPROM_GET8((offset), (value)); break; \
	default: EEPROM_GET32((offset), (value)); break; \
	} \
} \
while (false)

void ReadSettings() {
	for (size_t i = 0u; i < lengthof(Settings); i++) {
		Settings[i] = EepromData[i];
	}
}

void UNK_602FA7E() {
	// TODO: Unused, but might be useful, once the purpose of this function is understood.
}

void WriteSettings() {
	for (size_t i = 0u; i < lengthof(Settings); i++) {
		EepromData[i] = (uint8_t)Settings[i];
	}
}

void EepromGet(uint8_t offset, void* destination, size_t size) {
#define READEEPROM_CASE(m) \
case offsetof(SaveData, m): \
	assert(size == sizeoffield(SaveData, m)); \
	EEPROM_GET(EEPROM_##m, destination, size); \
	break

	switch (offset) {
		READEEPROM_CASE(coinCount);
		READEEPROM_CASE(demoWaitTime);
		READEEPROM_CASE(gameTime);
		READEEPROM_CASE(playCount);
		READEEPROM_CASE(twinCount);
		READEEPROM_CASE(versusCount);
		READEEPROM_CASE(initSeed);
		READEEPROM_CASE(playStatusChecksum);
		READEEPROM_CASE(rankingsChecksum);
		READEEPROM_CASE(programChecksum);

	default:
#define READEEPROM_RANKING(m) \
else if (offset >= offsetof(SaveData, m) && offset < offsetof(SaveData, m) + sizeoffield(SaveData, m)) { \
	const size_t rankingOffset = ((offset - offsetof(SaveData, m)) / sizeof(Ranking)) * (lengthoffield(Ranking, name) + sizeoffield(Ranking, data)); \
	for (size_t i = 0u; i < lengthoffield(Ranking, name); i++) { \
		 ((Ranking*)destination)->name[i] = EepromData[EEPROM_##m + rankingOffset + i]; \
	} \
	EEPROM_GET32(EEPROM_##m + rankingOffset + lengthoffield(Ranking, name), (void*)&(((Ranking*)destination)->data)); \
} \

		if (false) {}
		READEEPROM_RANKING(rankings)
		READEEPROM_RANKING(doublesLevelRankings)
		else if (offset >= offsetof(SaveData, masterMedalRankings) && offset < offsetof(SaveData, masterMedalRankings) + sizeoffield(SaveData, masterMedalRankings)) {
			EEPROM_GET16((size_t)EEPROM_masterMedalRankings + offset - offsetof(SaveData, masterMedalRankings), destination);
		}
		else {
			fprintf(stderr, "Invalid offset for reading from EEP-ROM requested.\n");
			exit(EXIT_FAILURE);
		}
	}
}

void EepromSet(uint8_t offset, void* source, size_t size) {
#define WRITEEEPROM_CASE(m) \
case offsetof(SaveData, m): \
	assert(size == sizeoffield(SaveData, m)); \
	EEPROM_SET(EEPROM_##m, source, size); \
	break

	switch (offset) {
		WRITEEEPROM_CASE(coinCount);
		WRITEEEPROM_CASE(demoWaitTime);
		WRITEEEPROM_CASE(gameTime);
		WRITEEEPROM_CASE(playCount);
		WRITEEEPROM_CASE(twinCount);
		WRITEEEPROM_CASE(versusCount);
		WRITEEEPROM_CASE(initSeed);
		WRITEEEPROM_CASE(playStatusChecksum);
		WRITEEEPROM_CASE(rankingsChecksum);
		WRITEEEPROM_CASE(programChecksum);

	default:
#define WRITEEEPROM_RANKING(m) \
else if (offset >= offsetof(SaveData, m) && offset < offsetof(SaveData, m) + sizeoffield(SaveData, m)) { \
	const size_t rankingOffset = ((offset - offsetof(SaveData, m)) / sizeof(Ranking)) * (lengthoffield(Ranking, name) + sizeoffield(Ranking, data)); \
	for (size_t i = 0u; i < lengthoffield(Ranking, name); i++) { \
		EepromData[EEPROM_##m + rankingOffset + i] = ((Ranking*)source)->name[i]; \
	} \
	EEPROM_SET32(EEPROM_##m + rankingOffset + lengthoffield(Ranking, name), (void*)&(((Ranking*)source)->data)); \
} \

		if (false) {}
		WRITEEEPROM_RANKING(rankings)
		WRITEEEPROM_RANKING(doublesLevelRankings)
		else if (offset >= offsetof(SaveData, masterMedalRankings) && offset < offsetof(SaveData, masterMedalRankings) + sizeoffield(SaveData, masterMedalRankings)) {
			EEPROM_SET16((size_t)EEPROM_masterMedalRankings + offset - offsetof(SaveData, masterMedalRankings), source);
		}
		else {
			fprintf(stderr, "Invalid offset for writing to EEP-ROM requested.\n");
			exit(EXIT_FAILURE);
		}
	}
}
