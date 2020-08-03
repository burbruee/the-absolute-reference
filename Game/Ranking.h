#pragma once

#include "Player.h"
#include "Object.h"
#include "HwInput.h"
#include "HwSprite.h"
#include <stdint.h>
#include <stdbool.h>

// TODO: Decide what to make private in Ranking.c, and what to keep here.

typedef uint32_t RankingFlag;
#define RANKINGFLAG_NONE 0u
#define RANKINGFLAG_PLACEMASK 3u
#define RANKINGFLAG_ALL 0x7FFFFu

typedef enum RankingIndex {
	RANKINGINDEX_MASTERSECTIONTIMES = 0,
	RANKINGINDEX_MASTER = 10,
	RANKINGINDEX_NORMAL = 13,
	RANKINGINDEX_DOUBLES = 16,
	RANKINGINDEX_MASTERTODAYSBEST = 19,
	RANKINGINDEX_NORMALTODAYSBEST = 22,
	RANKINGINDEX_DOUBLESTODAYSBEST = 25,
	NUMRANKINGS = 19
} RankingIndex;

typedef enum RankingPlace {
	RANKINGPLACE_FIRST,
	RANKINGPLACE_SECOND,
	RANKINGPLACE_THIRD,
	RANKINGPLACE_NULL,
	NUMRANKINGPLACES = RANKINGPLACE_NULL,
	RANKINGPLACE_NOSTANDING = -1
} RankingPlace;

typedef struct Ranking {
	char name[4];
	uint32_t data;
} Ranking;

#define RANKINGDATA_GETVALUE(rankingData) ((rankingData) & 0xFFFFF)
#define RANKINGDATA_GETGRADE(rankingData) (((rankingData) >> 27) & 0x1F)
#define RANKINGDATA_GETORANGELINE(rankingData) ((rankingData) & 0x02000000)
#define RANKINGDATA_GETGREENLINE(rankingData) ((rankingData) & 0x04000000)
#define RANKINGDATA_GET1PLEVEL(rankingData) (((rankingData) >> 16) & 0xFFFF)
#define RANKINGDATA_GET2PLEVEL(rankingData) ((rankingData) & 0xFFFF)
#define RANKINGDATA_GETLEVELSUM(rankingData) (RANKINGDATA_GET1PLEVEL(rankingData) + RANKINGDATA_GET2PLEVEL(rankingData))

#define RANKINGDATA_SETVALUE(rankingData, value) ((rankingData) = ((rankingData) & ~0xFFFFF) | ((value) & 0xFFFFF))
#define RANKINGDATA_SETGRADE(rankingData, grade) ((rankingData) = ((rankingData) & 0xF8000000) | (((grade) & 0x1F) << 27))
#define RANKINGDATA_SETORANGELINE(rankingData, line) ((rankingData) = ((rankingData) & 0x02000000) | (((line) & 1) << 25))
#define RANKINGDATA_SETGREENLINE(rankingData, line) ((rankingData) = ((rankingData) & ~0x04000000) | (((line) & 1) << 26))
#define RANKINGDATA_SET1PLEVEL(rankingData, level) ((rankingData) = ((rankingData) & ~0xFFFF0000) | (((level) & 0xFFFF) << 16))
#define RANKINGDATA_SET2PLEVEL(rankingData, level) ((rankingData) = ((rankingData) & ~0x0000FFFF) | ((level) & 0xFFFF))

typedef struct NameEntryData {
	Player *player;

	uint8_t waitFrames;
	uint16_t timeoutFrames;

	int8_t numChars;
	int8_t charIndex;
	int8_t autoshiftFrames;

	char name[5];
} NameEntryData;

typedef enum NewRankingState {
	NEWRANKING_INIT,
	NEWRANKING_NAMEENTRY,
	NEWRANKING_END
} NewRankingState;

typedef struct NewRankingData {
	Player *player;

	NewRankingState state;
	int16_t flashFrames;

	RankingFlag flags;
	RankingPlace place;
	RankingPlace todaysBestPlace;

	NameEntryData nameEntries[NUMPLAYERS];

	uint16_t labelScaleFrames;
	SpriteScale labelScaleX;
} NewRankingData;

extern NewRankingData NewRankings[NUMPLAYERS];

extern const ObjectData* ObjectTableRankingDigits[10];

// TODO: Test the ShowSectionTimes* functions to see if they might have been
// intended to display section times to the player at the new ranking screen.

void ShowSectionTimesEx(uint32_t* sectionTimes, int16_t y, int16_t x);
void _0x6011840();
// Set this to the section times you want displayed by ShowSectionTimes().
extern uint32_t SectionTimes[10];
void ShowSectionTimes();
void _0x6011854(); // TODO: Check TGM2 to see if this contains any code. Just calls ShowSectionTimes() in TAP.
void _0x6011858(); // TODO: Check TGM2 to see if this contains any code. Does nothing in TAP.

void InitRankings();
void InitTodaysBestRankings();

// TODO: Functions from _0x6011D28 to _0x6012828. I think they handle the
// rankings shown in the demo screen.
uint32_t RankingsChecksum();
void CheckSaveRankings();
void SaveRankings();
bool LoadRankings();

void NextPlayRanking(Player *player);

void InitNameEntry(NameEntryData *nameEntry, Player *player);
bool UpdateNameEntry(NameEntryData *nameEntry);
bool UpdatePlayRanking(Player *player);