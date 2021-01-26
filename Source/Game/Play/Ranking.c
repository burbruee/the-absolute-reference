#include "Game/Play/Ranking.h"
#include "Game/Play/Player.h"
#include "Game/Play/Debug.h"
#include "Game/Play/Grade.h"
#include "Game/Play/Medal.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/ShowGameStatus.h"
#include "Game/Graphics/ShowClearTime.h"
#include "Game/Graphics/ShowObject.h"
#include "Main/MainLoop.h"
#include "Main/AttractLoop.h"
#include "Input/Button.h"
#include "Main/Frame.h"
#include "Game/Save.h"
#include "Video/Object.h"
#include "Video/HwSprite.h"
#include "Sound/Sound.h"
#include "Sound/SoundEffect.h"
#include "Eeprom/Eeprom.h"
#include "Lib/LibC.h"

static Ranking MasterTodaysBestRankings[NUMRANKINGPLACES];
static uint16_t MasterTodaysBestMedalRankings[NUMRANKINGPLACES];

static Ranking NormalTodaysBestRankings[NUMRANKINGPLACES];

static Ranking DoublesTodaysBestTimeRankings[NUMRANKINGPLACES];
static Ranking DoublesTodaysBestLevelRankings[NUMRANKINGPLACES];

static RankingFlag RankingFlags;

NewRankingData NewRankings[NUMPLAYERS];

#define CHAROBJECT(i) &OBJECTTABLE_RANKINGCHARS[i]
static const ObjectData* const ObjectTableRankingChars[61] = {
	CHAROBJECT( 0), CHAROBJECT( 1), CHAROBJECT( 2), CHAROBJECT( 3), CHAROBJECT( 4),
	CHAROBJECT( 5), CHAROBJECT( 6), CHAROBJECT( 7), CHAROBJECT( 8), CHAROBJECT( 9),
	CHAROBJECT(10), CHAROBJECT(11), CHAROBJECT(12), CHAROBJECT(13), CHAROBJECT(14),
	CHAROBJECT(15), CHAROBJECT(16), CHAROBJECT(17), CHAROBJECT(18), CHAROBJECT(19),
	CHAROBJECT(20), CHAROBJECT(21), CHAROBJECT(22), CHAROBJECT(23), CHAROBJECT(24),
	CHAROBJECT(25), CHAROBJECT(26), CHAROBJECT(27), CHAROBJECT(28), CHAROBJECT(29),
	CHAROBJECT(30), CHAROBJECT(31), CHAROBJECT(32), CHAROBJECT(33), CHAROBJECT(34),
	CHAROBJECT(35), CHAROBJECT(36), CHAROBJECT(37), CHAROBJECT(38), CHAROBJECT(39),
	CHAROBJECT(40), CHAROBJECT(41), CHAROBJECT(42), CHAROBJECT(43), CHAROBJECT(44),
	CHAROBJECT(45), CHAROBJECT(46), CHAROBJECT(47), CHAROBJECT(48), CHAROBJECT(49),
	CHAROBJECT(50), CHAROBJECT(51), CHAROBJECT(52), CHAROBJECT(53), CHAROBJECT(54),
	CHAROBJECT(55), CHAROBJECT(56), CHAROBJECT(57), CHAROBJECT(58), CHAROBJECT(59),
	CHAROBJECT(60)
};
#undef CHAROBJECT

#define NAMEENTRYCHARS_ENTERABLE 40
#define NAMEENTRYCHARS_BACKSPACE (NAMEENTRYCHARS_ENTERABLE + 0)
#define NAMEENTRYCHARS_END       (NAMEENTRYCHARS_ENTERABLE + 1)
static const char* const NameEntryChars[NAMEENTRYCHARS_END + 1] = {
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
	"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
	"U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3",
	"4", "5", "6", "7", "8", "9", "!", "?", ".", " ",
	"\\", "["
};

static const char DefaultName[] = { 'A', 'R', 'K', '\0' };

static const char* const InvalidNames[] = {
	"   ", "A  ", "AA ", "AAA", "666",
	"AHO", "ASS", "AUM", "DIE", "ETA",
	"FUC", "FUK", "HIV", "IRA", "KKK",
	"OSI", "PEE", "PIS", "PLO", "SEX",
	"end"
};

static const ObjectData* const ObjectTableRankingPlaces[NUMRANKINGPLACES + 1] = {
	&OBJECTTABLE_RANKINGPLACES[1],
	&OBJECTTABLE_RANKINGPLACES[2],
	&OBJECTTABLE_RANKINGPLACES[3],
	&OBJECTTABLE_RANKINGPLACES[0]
};

static void ShowRankingName(const char* name, int16_t y, int16_t x, uint8_t palNum, uint16_t layer);
static void UNK_6011AD2(uint32_t score, int16_t y, int16_t x);
static void ShowRankingScore(uint32_t score, int16_t y, int16_t x);
static void ShowRankingTime(uint32_t time, int16_t y, int16_t x);
static void UNK_6011D28(int16_t y, int16_t x);
static void UNK_6011DB0(int16_t y, int16_t x);
static void UNK_6011DF0(int16_t y, int16_t x);
static void UNK_6011E48(int16_t y, int16_t x);
static void UpdateEntityRanking(Entity* entity);
// UNK_60122D8
// UNK_6012354
// UNK_6012828

static uint32_t RankingsChecksum();

static RankingPlace MasterRankingPlace(Player* player, Ranking* rankings, int16_t numRankings);
static RankingPlace NormalRankingPlace(Player* player, Ranking* rankings, int16_t numRankings);
static RankingPlace DoublesRankingPlace(uint16_t levelSum, uint32_t time, Ranking* timeRankings, Ranking* levelRankings, int16_t numRankings);

static RankingFlag NewRankingFlags(Player* player, NewRankingData* newRanking);

typedef enum RankingMode {
	RANKINGMODE_MASTER,
	RANKINGMODE_NORMAL,
	RANKINGMODE_DOUBLES
} RankingMode;

static RankingFlag AddNewModeRanking(NewRankingData* newRanking, Ranking* rankings, void* rankingExtras, RankingIndex rankingIndex, int16_t numRankings, RankingMode rankingMode);
static void AddNewRanking(NewRankingData* newRanking);

typedef enum EntryFlash {
	ENTRYFLASH_FALSE = 0x00,
	ENTRYFLASH_TRUE = 0xFF
} EntryFlash;
static void ShowPlayRanking(NewRankingData* newRanking, EntryFlash entryFlash);
static void ShowPlayRankingCodeNameEntry(NewRankingData* newRanking, EntryFlash entryFlash);
static void ShowPlayRankingDoubles(NewRankingData* newRanking, EntryFlash entryFlash);

void ShowSectionTimesEx(uint32_t* sectionTimes, int16_t y, int16_t x) {
	uint32_t clearTime = 0u;
	for (size_t section = 0u; section < 10u; section++, y += 12) {
		ShowClearTime(sectionTimes[section], y, x);
		clearTime += sectionTimes[section];
	}
	ShowClearTime(clearTime, y + 32, x);
}

void UNK_6011840() {
	// TODO: Empty. Check TGM2 to see if there's code there.
}

uint32_t SectionTimes[10];
void ShowSectionTimes() {
	ShowSectionTimesEx(SectionTimes, 40, 170);
}

void UNK_6011854() {
	// TODO: Check TGM2 to see if this contains any code. Just calls ShowSectionTimes() in TAP.
	ShowSectionTimes();
}
void UNK_6011858() {
	// TODO: Check TGM2 to see if this contains any code. Does nothing in TAP.
}

// TODO: Use macros here instead of hard-coded hex numbers.
#define TA_NAME { 'T', '.', 'A', '\0' }

// All-time default rankings.

static const Ranking DefaultRankings[NUMRANKINGS] = {
	{ TA_NAME, 0x00001518u },
	{ TA_NAME, 0x08001518u },
	{ TA_NAME, 0x10001518u },
	{ TA_NAME, 0x18001518u },
	{ TA_NAME, 0x20001518u },
	{ TA_NAME, 0x28001518u },
	{ TA_NAME, 0x30001518u },
	{ TA_NAME, 0x38001518u },
	{ TA_NAME, 0x40001518u },
	{ TA_NAME, 0x48001518u },
	{ TA_NAME, 0x48011940u },
	{ TA_NAME, 0x3000FD20u },
	{ TA_NAME, 0x1800E100u },
	{ TA_NAME, 0x10001388u },
	{ TA_NAME, 0x08000FA0u },
	{ TA_NAME, 0x00000BB8u },
	{ TA_NAME, 0x18011940u },
	{ TA_NAME, 0x1000D2F0u },
	{ TA_NAME, 0x08008CA0u },
};

static const Ranking DefaultDoublesLevelRankings[NUMRANKINGPLACES] = {
	{ TA_NAME, 0x0096008Cu },
	{ TA_NAME, 0x0064005Au },
	{ TA_NAME, 0x00320028u }
};

// Today's best default rankings.

static const Ranking DefaultMasterTodaysBestRankings[NUMRANKINGPLACES] = {
	{ TA_NAME, 0x18011940 },
	{ TA_NAME, 0x1000FD20 },
	{ TA_NAME, 0x0800E100 }
};

static const Ranking DefaultNormalTodaysBestRankings[NUMRANKINGPLACES] = {
	{ TA_NAME, 0x00000BB8 },
	{ TA_NAME, 0x000007D0 },
	{ TA_NAME, 0x000003E8 }
};

static const Ranking DefaultDoublesTodaysBestTimeRankings[NUMRANKINGPLACES] = {
	{ TA_NAME, 0x10011940 },
	{ TA_NAME, 0x0800FD20 },
	{ TA_NAME, 0x0000E100 }
};

static const Ranking DefaultDoublesTodaysBestLevelRankings[NUMRANKINGPLACES] = {
	{ TA_NAME, 0x0032001E },
	{ TA_NAME, 0x001E000A },
	{ TA_NAME, 0x000A0000 }
};

#undef TA_NAME

void InitRankings() {
	RankingFlags = RANKINGFLAG_NONE;
	for (size_t i = 0u; i < NUMRANKINGS; i++) {
		Save.rankings[i] = DefaultRankings[i];
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		Save.doublesLevelRankings[place] = DefaultDoublesLevelRankings[place];
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		Save.masterMedalRankings[place] = 0u;
	}
}

void InitTodaysBestRankings() {
	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		MasterTodaysBestRankings[place] = DefaultMasterTodaysBestRankings[place];
		MasterTodaysBestMedalRankings[place] = 0u;
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		NormalTodaysBestRankings[place] = DefaultNormalTodaysBestRankings[place];
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		DoublesTodaysBestTimeRankings[place] = DefaultDoublesTodaysBestTimeRankings[place];
		DoublesTodaysBestLevelRankings[place] = DefaultDoublesTodaysBestLevelRankings[place];
	}
}

static void ShowRankingName(const char* name, int16_t y, int16_t x, uint8_t palNum, uint16_t layer) {
	for (const char* c = name; *c != '\0'; c++, x += 16) {
		DisplayObject(ObjectTableRankingChars[*c - ' '], y, x, palNum, layer);
	}
}

static void UNK_6011AD2(uint32_t score, int16_t y, int16_t x) {
	if (score > 999999u) {
		score = 999999u;
	}

	x += 80;
	for (int16_t i = 0; i < 6; i++) {
		DisplayObject(ObjectTableRankingChars[(16u + score % 10u) & 0x1Fu], y, x, 0u, 110u);
		score /= 10u;
		x -= 16;
		if (score == 0u) {
			return;
		}
	}
}

const ObjectData* ObjectTableRankingDigits[10] = {
	&OBJECTTABLE_RANKINGDIGITS[0],
	&OBJECTTABLE_RANKINGDIGITS[1],
	&OBJECTTABLE_RANKINGDIGITS[2],
	&OBJECTTABLE_RANKINGDIGITS[3],
	&OBJECTTABLE_RANKINGDIGITS[4],
	&OBJECTTABLE_RANKINGDIGITS[5],
	&OBJECTTABLE_RANKINGDIGITS[6],
	&OBJECTTABLE_RANKINGDIGITS[7],
	&OBJECTTABLE_RANKINGDIGITS[8],
	&OBJECTTABLE_RANKINGDIGITS[9],
};

static void ShowRankingScore(uint32_t score, int16_t y, int16_t x) {
	if (score > 999999u) {
		score = 999999u;
	}

	x += 60;
	for (int16_t i = 0; i < 6; i++, x -= 12) {
		DisplayObject(ObjectTableRankingDigits[(score & 0xFu) % 10u], y, x, 0u, 110u);
		if ((score /= 10u) == 0u) {
			break;
		}
	}
}

static void ShowRankingTime(uint32_t time, int16_t y, int16_t x) {
	uint8_t minutes = time / TIME(1, 0, 0);
	time -= minutes * TIME(1, 0, 0);
	uint8_t seconds = time / TIME(0, 1, 0);
	time -= seconds * TIME(0, 1, 0);
	uint8_t centiseconds = (time * 100) / 60;

	DisplayObject(ObjectTableRankingDigits[minutes / 10], y, x, 0u, 110u);
	DisplayObject(ObjectTableRankingDigits[minutes % 10], y, x + 11, 0u, 110u);

	DisplayObject(OBJECT_RANKINGTIMECOLON, y, x + 22, 0u, 110u);

	DisplayObject(ObjectTableRankingDigits[seconds / 10], y, x + 27, 0u, 110u);
	DisplayObject(ObjectTableRankingDigits[seconds % 10], y, x + 38, 0u, 110u);

	DisplayObject(OBJECT_RANKINGTIMECOLON, y, x + 49, 0u, 110u);

	DisplayObject(ObjectTableRankingDigits[centiseconds / 10], y, x + 54, 0u, 110u);
	DisplayObject(ObjectTableRankingDigits[centiseconds % 10], y, x + 65, 0u, 110u);
}

static void UNK_6011D28(int16_t y, int16_t x) {
	ShowObject(OBJECTPTR(0x221), y, x + 31, 0u, 110u);
	ShowObject(OBJECTPTR(0x222), y, x + 85, 0u, 110u);
	ShowObject(OBJECTPTR(0x223), y, x + 126, 0u, 110u);

	ShowObject(OBJECTPTR(0x226), y, x + 215, 0u, 110u);
}

static void UNK_6011DB0(int16_t y, int16_t x) {
	ShowObject(OBJECTPTR(0x221), y, x + 42, 0u, 110u);
	ShowObject(OBJECTPTR(0x227), y, x + 214, 0u, 110u);
}

static void UNK_6011DF0(int16_t y, int16_t x) {
	ShowObject(OBJECTPTR(0x225), y, x + 10, 0, 110u);
	ShowObject(OBJECTPTR(0x221), y, x + 97, 0u, 110u);
	ShowObject(OBJECTPTR(0x222), y, x + 166, 0u, 110u);
	ShowObject(OBJECTPTR(0x223), y, x + 229, 0u, 110u);
}

static void UNK_6011E48(int16_t y, int16_t x) {
	ShowObject(OBJECTPTR(0x224), y, x + 35, 0u, 110u);
	ShowObject(OBJECTPTR(0x223), y, x + 230, 0u, 110u);
}

typedef struct RankingInstanceData {
	void* rankingExtras;
	int16_t y;
	int16_t x;
	int16_t lineX;
	uint16_t index;
} RankingInstanceData;

static const ObjectData* UNK_3B0EC[20] = {
	OBJECTPTR(0x229),
	OBJECTPTR(0X22A),
	OBJECTPTR(0X22B),
	OBJECTPTR(0X22C),
	OBJECTPTR(0X22D),
	OBJECTPTR(0X22E),
	OBJECTPTR(0X22F),
	OBJECTPTR(0X230),
	OBJECTPTR(0X231),
	OBJECTPTR(0X232),
	OBJECTPTR(0X233),
	OBJECTPTR(0X234),
	OBJECTPTR(0X235),
	OBJECTPTR(0X236),
	OBJECTPTR(0X237),
	OBJECTPTR(0X238),
	OBJECTPTR(0X239),
	OBJECTPTR(0X23A),
	OBJECTPTR(0X23C),
	OBJECTPTR(0X23B)
};

static void UpdateEntityRanking(Entity* entity) {
	STATIC_ASSERT(sizeof(RankingInstanceData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(RankingInstanceData, data, entity);

	if (entity->values[3] == 0) {
		if (data->lineX != 0) {
			data->lineX += 24;
			if (data->lineX > 0) {
				data->lineX = 0;
			}
		}
	}
	else {
		entity->values[3]--;
	}

	if (entity->values[2] != 0) {
		entity->values[2]--;
		return;
	}

	if (data->x != 0) {
		int16_t stepX = data->x;
		if (stepX < 0) {
			stepX += 3;
		}
		stepX /= 4;
		if (stepX > 16) {
			stepX = 16;
		}
		data->x -= stepX;
	}
	const Ranking* const ranking = ENTITY_DATA(entity).ranking;
	const int16_t y = data->y;
	const int16_t x = data->x;
	const int16_t x0 = x + 10;
	const int16_t y0 = y + 10;
	const int16_t y1 = y + 2;
	switch (entity->values[1]) {
	case RANKINGSCREEN_MASTER:
		DisplayObject(ObjectTableRankingChars[data->index + 16], y, x0, 0, 110u);
		ShowRankingName(ranking->name, y, x + 31, 0u, 110u);
		DisplayObject(UNK_3B0EC[RANKINGDATA_GETGRADE(ranking->data) % NUMPLAYERGRADES], y, x + 85, 0u, 110u);
		ShowRankingTime(RANKINGDATA_GETVALUE(ranking->data), y1, x + 126);
		ShowRankingMedals(*(uint16_t*)data->rankingExtras, y, x + 215);
		if (RANKINGDATA_GETORANGELINE(ranking->data)) {
			DisplayObjectEx(OBJECTPTR(0x265), y0, data->lineX, 202u, 40u, 0xCu, UNSCALED, false);
		}
		else if (RANKINGDATA_GETGREENLINE(ranking->data)) {
			DisplayObjectEx(OBJECTPTR(0x265), y0, data->lineX, 0u, 40u, 0xCu, UNSCALED, false);
		}
		break;

	case RANKINGSCREEN_NORMAL:
		DisplayObject(ObjectTableRankingChars[data->index + 16], y, x0, 0u, 110u);
		ShowRankingName(ranking->name, y, x + 42, 0u, 110u);
		UNK_6011AD2(RANKINGDATA_GETVALUE(ranking->data), y, x + 214);
		if (RANKINGDATA_GETGREENLINE(ranking->data)) {
			DisplayObjectEx(OBJECTPTR(0x265), y0, data->lineX, 0, 40u, 0xCu, UNSCALED, false);
		}
		break;

	case RANKINGSCREEN_SECTIONTIME:
		if (data->index == 0u) {
			ShowStatusNumEx(0u, y1, x0, 0u, 40u, 3, false, NUMALIGN_RIGHT);
		}
		else {
			ShowStatusNumEx(NextSectionLevels[data->index - 1], y1, x0, 0u, 40u, 3, false, NUMALIGN_RIGHT);
		}
		DisplayObject(OBJECTPTR(0x228), y1, x + 34, 0u, 110u);
		ShowStatusNumEx(NextSectionLevels[data->index], y1, x + 42, 0u, 40u, 3, false, NUMALIGN_RIGHT);
		ShowRankingName(ranking->name, y, x + 97, 0u, 110u);
		DisplayObject(UNK_3B0EC[RANKINGDATA_GETGRADE(ranking->data) % NUMPLAYERGRADES], y, x + 166, 0, 110u);
		ShowRankingTime(RANKINGDATA_GETVALUE(ranking->data), y1, x + 229);
		if (RANKINGDATA_GETORANGELINE(ranking->data)) {
			DisplayObjectEx(OBJECTPTR(0x265), y0, data->lineX, 202u, 40u, 0xCu, UNSCALED, false);
		}
		else if (RANKINGDATA_GETGREENLINE(ranking->data)) {
			DisplayObjectEx(OBJECTPTR(0x265), y0, data->lineX, 0, 40u, 0xCu, UNSCALED, false);
		}
		break;

	case RANKINGSCREEN_DOUBLES:
		DisplayObject(ObjectTableRankingChars[data->index + 16], y, x0, 0u, 110u);
		ShowRankingName(ranking->name, y, x + 35, 0u, 110u);
		ShowStatusNumEx(RANKINGDATA_GET1PLEVEL(((Ranking*)data->rankingExtras)->data), y1, x + 85, 0u, 40u, 3, false, NUMALIGN_RIGHT);
		ShowRankingName("&", y, x + 118, 0u, 110u);
		ShowRankingName(((Ranking*)data->rankingExtras)->name, y, x + 141, 0u, 110u);
		ShowStatusNumEx(RANKINGDATA_GET2PLEVEL(((Ranking*)data->rankingExtras)->data), y1, x + 191, 0u, 40u, 3, false, NUMALIGN_RIGHT);
		ShowRankingTime(RANKINGDATA_GETVALUE(ranking->data), y1, x + 230);
		if (RANKINGDATA_GETGREENLINE(ranking->data)) {
			DisplayObjectEx(OBJECTPTR(0x265), y0, data->lineX, 0, 40u, 0xCu, UNSCALED, false);
		}
		break;

	default:
		break;
	}
}

static void ShowRanking(Ranking* ranking, void* rankingExtras, int16_t y, int16_t x, uint16_t index, RankingScreenState rankingScreen, int16_t showDelay, int16_t lineScrollDelay) {
	Entity* entity;
	if ((entity = AllocEntity())) {
		entity->update = UpdateEntityRanking;

		entity->values[0] = 1;
		entity->values[1] = rankingScreen;
		entity->values[2] = showDelay;
		entity->values[3] = lineScrollDelay;

		ENTITY_DATA(entity).ranking = ranking;

		ENTITY_INST_DATA_PTR(RankingInstanceData, data, entity);
		data->rankingExtras = rankingExtras;
		data->y = y;
		data->x = VIDEO_WIDTH;
		data->lineX = -VIDEO_WIDTH;
		data->index = index;
	}
}

void ShowRankings(RankingScreenState rankingScreen) {
	switch (rankingScreen) {
	case RANKINGSCREEN_MASTER:
		for (RankingPlace place = RANKINGPLACE_FIRST, y = 50, showDelay = 0; place < NUMRANKINGPLACES; place++, y += 22, showDelay += 12) {
			ShowRanking(&Save.rankings[RANKINGINDEX_MASTER + place], &Save.masterMedalRankings[place], y, 0, place + 1u, RANKINGSCREEN_MASTER, showDelay, 90);
		}
		for (RankingPlace place = RANKINGPLACE_FIRST, y = 170, showDelay = 36; place < NUMRANKINGPLACES; place++, y += 22, showDelay += 12) {
			ShowRanking(&MasterTodaysBestRankings[place], &MasterTodaysBestMedalRankings[place], y, 0, place + 1u, RANKINGSCREEN_MASTER, showDelay, 90);
		}
		break;

	case RANKINGSCREEN_NORMAL:
		for (RankingPlace place = RANKINGPLACE_FIRST, y = 50, showDelay = 0; place < NUMRANKINGPLACES; place++, y += 22, showDelay += 12) {
			ShowRanking(&Save.rankings[RANKINGINDEX_NORMAL + place], NULL, y, 0, place + 1u, RANKINGSCREEN_NORMAL, showDelay, 90);
		}
		for (RankingPlace place = 0, y = 170, showDelay = 36; place < NUMRANKINGPLACES; place++, y += 22, showDelay += 12) {
			ShowRanking(&NormalTodaysBestRankings[place], NULL, y, 0, place + 1u, RANKINGSCREEN_NORMAL, showDelay, 90);
		}
		break;

	case RANKINGSCREEN_SECTIONTIME:
		for (int16_t section = 0, y = 54, showDelay = 0; section < 10; section++, y += 19, showDelay += 12) {
			ShowRanking(&Save.rankings[RANKINGINDEX_MASTERSECTIONTIMES + section], NULL, y, 0, section, RANKINGSCREEN_SECTIONTIME, showDelay, 138);
		}
		break;

	case RANKINGSCREEN_DOUBLES:
		for (RankingPlace place = RANKINGPLACE_FIRST, y = 50, showDelay = 0; place < NUMRANKINGPLACES; place++, y += 22, showDelay += 12) {
			ShowRanking(&Save.rankings[RANKINGINDEX_DOUBLES + place], &Save.doublesLevelRankings[place], y, 0, place + 1u, RANKINGSCREEN_DOUBLES, showDelay, 90);
		}
		for (RankingPlace place = 0, y = 170, showDelay = 36; place < NUMRANKINGPLACES; place++, y += 22, showDelay += 12) {
			ShowRanking(&DoublesTodaysBestTimeRankings[place], &DoublesTodaysBestLevelRankings[place], y, 0, place + 1u, RANKINGSCREEN_DOUBLES, showDelay, 90);
		}
		break;

	default:
		break;
	}
}

ScreenState ShowRankingScreen(RankingScreenState rankingScreen) {
	UNK_6020ECE();
	ShowRankings(rankingScreen);

	uint16_t frames = 0u;
	bool done = false;
	RankingScreenState nextRankingScreen = rankingScreen;
	while (!UpdateFrame()) {
		frames++;
		if (UNK_6064750 == NULL) {
			if (done) {
				switch (Screen) {
				case SCREEN_NORMALRANKING:
					return SCREEN_MASTERRANKING;

				case SCREEN_MASTERRANKING:
					return SCREEN_SECTIONTIMERANKING;

				default:
					return SCREEN_DEVELOPER;
				}
			}
			if (frames >= TIME(0, 6, 0)) {
				UNK_6029546(2, 20, 0, 6);
				done = true;
			}
		}

		if (MainLoop == MAINLOOP_ATTRACT && NextScreenVersionTitle()) {
			UNK_602406E();
			return SCREEN_VERSIONTITLE;
		}

		if (UNK_6064750 == NULL) {
			if ((GameButtonsNew[PLAYER1] & BUTTON_LEFT) || (GameButtonsNew[PLAYER2] & BUTTON_LEFT)) {
				nextRankingScreen = (nextRankingScreen + 3) % NUMRANKINGSCREENS;
				InitEntities();
				ShowRankings(nextRankingScreen);
				UNK_6029546(0, 20, 0, 6);
				frames = 0u;
			}
			else if ((GameButtonsNew[PLAYER1] & BUTTON_RIGHT) || (GameButtonsNew[PLAYER2] & BUTTON_RIGHT)) {
				nextRankingScreen = (nextRankingScreen + 1) % NUMRANKINGSCREENS;
				InitEntities();
				ShowRankings(nextRankingScreen);
				UNK_6029546(0, 20, 0, 6);
				frames = 0u;
			}
		}

		switch (nextRankingScreen) {
		case RANKINGSCREEN_MASTER:
			ShowObject(OBJECTPTR(0x217), 14, 10, 0u, 110u);
			UNK_6011D28(35, 0);
			ShowObject(OBJECTPTR(0x21F), 134, 10, 0u, 110u);
			UNK_6011D28(155, 0);
			break;

		case RANKINGSCREEN_NORMAL:
			ShowObject(OBJECTPTR(0x219), 14, 10, 0u, 110u);
			UNK_6011DB0(35, 0);
			ShowObject(OBJECTPTR(0x21F), 134, 10, 0u, 110u);
			UNK_6011DB0(155, 0);
			break;

		case RANKINGSCREEN_SECTIONTIME:
			ShowObject(OBJECTPTR(0x21B), 14, 10, 0u, 110u);
			UNK_6011DF0(37, 0);
			break;

		case RANKINGSCREEN_DOUBLES:
			ShowObject(OBJECTPTR(0x21D), 14, 10, 0u, 110u);
			UNK_6011E48(35, 0);
			ShowObject(OBJECTPTR(0x21F), 134, 10, 0u, 110u);
			UNK_6011E48(155, 0);
			break;

		default:
			break;
		}
	}

	UNK_602406E();
	return SCREEN_TESTMODE;
}

static uint32_t RankingsChecksum() {
	uint32_t checksum = 0u;

	for (size_t i = 0u; i < NUMRANKINGS; i++) {
		Ranking* ranking = &Save.rankings[i];
		checksum += (uint32_t)ranking->name[0] << 8 | ranking->name[1];
		checksum += (uint32_t)ranking->name[2] << 8 | ranking->name[3];
		checksum += ranking->data >> 16;
		checksum += ranking->data & 0xFFFFu;
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		Ranking* ranking = &Save.doublesLevelRankings[place];
		checksum += (uint32_t)ranking->name[0] << 8 | ranking->name[1];
		checksum += (uint32_t)ranking->name[2] << 8 | ranking->name[3];
		checksum += ranking->data >> 16;
		checksum += ranking->data & 0xFFFFu;
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		checksum += Save.masterMedalRankings[place];
	}

	return ++checksum;
}

void CheckSaveRankings() {
	if (!Debug && RankingFlags) {
		uint16_t checksum = RankingsChecksum();
		Save.rankingsChecksum = checksum;
		SaveSet(rankingsChecksum);

		UpdateFrame();

		for (size_t i = 0u; i < NUMRANKINGS; i++) {
			RankingFlag rankingFlag = 1 << i;
			if (RankingFlags & rankingFlag) {
				RankingFlags &= ~rankingFlag;

				SaveSet(rankings[i]);

				if (i >= RANKINGINDEX_DOUBLES) {
					SaveSet(doublesLevelRankings[i - RANKINGINDEX_DOUBLES]);
				}

				if (i >= RANKINGINDEX_MASTER && i < RANKINGINDEX_NORMAL) {
					SaveSet(masterMedalRankings[i - RANKINGINDEX_MASTER]);
				}

				UpdateFrame();
			}
		}
	}
}

void SaveRankings() {
	RankingFlags = RANKINGFLAG_ALL;
	CheckSaveRankings();
}

bool LoadRankings() {
	uint16_t checksum;

	EepromGet(offsetof(SaveData, rankingsChecksum), &checksum, sizeoffield(SaveData, rankingsChecksum));
	Save.rankingsChecksum = checksum;

	for (size_t i = 0u; i < NUMRANKINGS; i++) {
		SaveGet(rankings[i]);
	}
	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		SaveGet(doublesLevelRankings[place]);
	}
	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		SaveGet(masterMedalRankings[place]);
	}

	if (checksum != (uint16_t)RankingsChecksum()) {
		InitRankings();
		RankingFlags = RANKINGFLAG_ALL;
		CheckSaveRankings();
		return false;
	}
	else {
		return true;
	}
}

static RankingPlace MasterRankingPlace(Player* player, Ranking* rankings, int16_t numRankings) {
	Ranking* ranking = rankings;
	for (RankingPlace place = RANKINGPLACE_FIRST; place < numRankings; place++, ranking++) {
		if ((uint32_t)player->grade > RANKINGDATA_GETGRADE(ranking->data)) {
			return place;
		}
		else if (player->grade == RANKINGDATA_GETGRADE(ranking->data)) {
			if (RANKINGDATA_GETORANGELINE(ranking->data)) {
				if (!(player->miscFlags & MISC_ORANGELINE)) {
					continue;
				}
			}
			else if (player->miscFlags & MISC_ORANGELINE) {
				return place;
			}

			if (RANKINGDATA_GETGREENLINE(ranking->data)) {
				if (player->level != 999u) {
					continue;
				}
			}
			else if (player->level == 999u) {
				return place;
			}

			if (player->masteringTime < RANKINGDATA_GETVALUE(ranking->data)) {
				return place;
			}
		}
	}
	return RANKINGPLACE_NOSTANDING;
}

static RankingPlace NormalRankingPlace(Player* player, Ranking* rankings, int16_t numRankings) {
	Ranking* ranking = rankings;
	for (RankingPlace place = RANKINGPLACE_FIRST; place < numRankings; place++, ranking++) {
		if (player->score > RANKINGDATA_GETVALUE(ranking->data)) {
			return place;
		}
	}
	return RANKINGPLACE_NOSTANDING;
}

static RankingPlace DoublesRankingPlace(uint16_t levelSum, uint32_t time, Ranking* timeRankings, Ranking* levelRankings, int16_t numRankings) {
	Ranking* timeRanking = timeRankings;
	for (RankingPlace place = RANKINGPLACE_FIRST; place < numRankings; place++, timeRanking++) {
		if (
				(levelSum > RANKINGDATA_GETLEVELSUM(levelRankings[place].data)) ||
				(levelSum == RANKINGDATA_GETLEVELSUM(levelRankings[place].data) && time < RANKINGDATA_GETVALUE(timeRanking->data))) {
			return place;
		}
	}
	return RANKINGPLACE_NOSTANDING;
}

static RankingFlag NewRankingFlags(Player* player, NewRankingData* newRanking) {
	RankingFlag newRankingFlags = RANKINGFLAG_NONE;

	NewRankingData tempNewRanking;
	if (newRanking == NULL) {
		newRanking = &tempNewRanking;
	}

	newRanking->todaysBestPlace = NUMRANKINGPLACES;
	newRanking->place = NUMRANKINGPLACES;

	if (player->modeFlags & MODE_DOUBLES) {
		RankingPlace place;
	
		place = DoublesRankingPlace(Players[PLAYER1].level + Players[PLAYER2].level, player->clearTime, &Save.rankings[RANKINGINDEX_DOUBLES], Save.doublesLevelRankings, NUMRANKINGPLACES);
		if (place >= RANKINGPLACE_FIRST) {
			newRankingFlags |= 1 << (place + RANKINGINDEX_DOUBLES);
			newRanking->place = place;
		}

		place = DoublesRankingPlace(Players[PLAYER1].level + Players[PLAYER2].level, player->clearTime, DoublesTodaysBestTimeRankings, DoublesTodaysBestLevelRankings, NUMRANKINGPLACES);
		if (place >= RANKINGPLACE_FIRST) {
			newRankingFlags |= 1 << (place + RANKINGINDEX_DOUBLESTODAYSBEST);
			newRanking->todaysBestPlace = place;
		}
	}
	else if (player->modeFlags & MODE_MASTER) {
		RankingPlace place;

		place = MasterRankingPlace(player, &Save.rankings[RANKINGINDEX_MASTER], NUMRANKINGPLACES);
		if (place >= RANKINGPLACE_FIRST) {
			newRankingFlags |= 1 << (place + RANKINGINDEX_MASTER);
			newRanking->place = place;
		}

		place = MasterRankingPlace(player, MasterTodaysBestRankings, NUMRANKINGPLACES);
		if (place >= RANKINGPLACE_FIRST) {
			newRankingFlags |= 1 << (place + RANKINGINDEX_MASTERTODAYSBEST);
			newRanking->todaysBestPlace = place;
		}

		Ranking* ranking = &Save.rankings[RANKINGINDEX_MASTERSECTIONTIMES];
		uint32_t* sectionTime = Grades[player->num].sectionTimes;
		for (int16_t section = 0; section < 10; ranking++, section++, sectionTime++) {
			if (*sectionTime != 0u && *sectionTime < RANKINGDATA_GETVALUE(ranking->data)) {
				newRankingFlags |= 1 << (section + RANKINGINDEX_MASTERSECTIONTIMES);
			}
		}
	}
	else if (player->modeFlags & MODE_NORMAL) {
		RankingPlace place;

		place = NormalRankingPlace(player, &Save.rankings[RANKINGINDEX_NORMAL], NUMRANKINGPLACES);
		if (place >= RANKINGPLACE_FIRST) {
			newRankingFlags |= 1 << (place + RANKINGINDEX_NORMAL);
			newRanking->place = place;
		}

		place = NormalRankingPlace(player, NormalTodaysBestRankings, NUMRANKINGPLACES);
		if (place >= RANKINGPLACE_FIRST) {
			newRankingFlags |= 1 << (place + RANKINGINDEX_NORMALTODAYSBEST);
			newRanking->todaysBestPlace = place;
		}
	}

	return newRankingFlags;
}

void NextPlayRanking(Player* player) {
	RankingFlag newRankingFlags = NewRankingFlags(player, &NewRankings[player->num]);
	NextPlay(player, (PlayData){.flags = PLAYFLAG_NONE, .state = PLAYSTATE_RANKING});

	NewRankingData* newRanking = &NewRankings[player->num];
	newRanking->state = NEWRANKING_INIT;
	newRanking->player = player;
	newRanking->flags = newRankingFlags;
}

static RankingFlag AddNewModeRanking(NewRankingData* newRanking, Ranking* rankings, void* rankingExtras, RankingIndex rankingIndex, int16_t numRankings, RankingMode rankingMode) {
	uint16_t* masterMedalRankings = (uint16_t*)rankingExtras;
	Ranking* doublesLevelRankings = (Ranking*)rankingExtras;
	RankingFlag rankingFlags = RANKINGFLAG_NONE;
	RankingIndex rankingBit = rankingIndex + numRankings;
	Player* player1 = &Players[PLAYER1];
	Player* player2 = &Players[PLAYER2];
	for (int16_t place = numRankings - 1; place >= RANKINGPLACE_FIRST; place--) {
		rankingFlags |= 1 << --rankingBit;
		if (newRanking->flags & (1 << rankingBit)) {
			MemCopy(sizeoffield(Ranking, name), rankings[place].name, newRanking->nameEntries->name);
			switch (rankingMode) {
				case RANKINGMODE_MASTER:
					RANKINGDATA_SETVALUE(rankings[place].data, newRanking->player->masteringTime);
					RANKINGDATA_SETGRADE(rankings[place].data, newRanking->player->grade);
					if (newRanking->player->level == 999u) {
						RANKINGDATA_SETGREENLINE(rankings[place].data, true);
					}
					if (newRanking->player->miscFlags & MISC_ORANGELINE) {
						RANKINGDATA_SETORANGELINE(rankings[place].data, true);
					}
					uint16_t medalRankingBit = 0u;
					MedalColor* medal = newRanking->player->medalColors;
					masterMedalRankings[place] = 0u;
					for (size_t i = 0u; i < NUMMEDALTYPES; i++, medalRankingBit += 2u, medal++) {
						masterMedalRankings[place] |= ((uint16_t)*medal & 3u) << medalRankingBit;
					}
					break;

				case RANKINGMODE_NORMAL:
					RANKINGDATA_SETVALUE(rankings[place].data, newRanking->player->score);
					if (newRanking->player->level == 300u) {
						RANKINGDATA_SETGREENLINE(rankings[place].data, true);
					}
					if (newRanking->player->miscFlags & MISC_ORANGELINE) {
						RANKINGDATA_SETORANGELINE(rankings[place].data, true);
					}
					break;

				case RANKINGMODE_DOUBLES:
					RANKINGDATA_SETVALUE(rankings[place].data, newRanking->player->clearTime);
					RANKINGDATA_SETGRADE(rankings[place].data, newRanking->player->grade);
					if (player1->level == 300u && player2->level == 300u) {
						RANKINGDATA_SETGREENLINE(rankings[place].data, true);
					}
					if (player1->miscFlags & MISC_ORANGELINE) {
						RANKINGDATA_SETORANGELINE(rankings[place].data, true);
					}
					MemCopy(sizeoffield(Ranking, name), doublesLevelRankings[place].name, newRanking->nameEntries[PLAYER2].name);
					RANKINGDATA_SET1PLEVEL(doublesLevelRankings[place].data, player1->level);
					RANKINGDATA_SET2PLEVEL(doublesLevelRankings[place].data, player2->level);
					break;

				default:
					break;
			}
			break;
		}
		else if (place > RANKINGPLACE_FIRST) {
			rankings[place] = rankings[place - 1];
			switch (rankingMode) {
				case RANKINGMODE_MASTER:
					masterMedalRankings[place] = masterMedalRankings[place - 1];
					break;

				case RANKINGMODE_DOUBLES:
					doublesLevelRankings[place] = doublesLevelRankings[place - 1];
					break;

				default:
					break;
			}
		}
	}
	return rankingFlags;
}

static void AddNewRanking(NewRankingData* newRanking) {
	newRanking->flags = NewRankingFlags(newRanking->player, NULL);

	if (newRanking->flags & (7 << RANKINGINDEX_NORMAL)) {
		RankingFlags |= AddNewModeRanking(newRanking, &Save.rankings[RANKINGINDEX_NORMAL], NULL, RANKINGINDEX_NORMAL, NUMRANKINGPLACES, RANKINGMODE_NORMAL);
	}
	if (newRanking->flags & (7 << RANKINGINDEX_NORMALTODAYSBEST)) {
		AddNewModeRanking(newRanking, NormalTodaysBestRankings, NULL, RANKINGINDEX_NORMALTODAYSBEST, NUMRANKINGPLACES, RANKINGMODE_NORMAL);
	}

	if (newRanking->flags & (7 << RANKINGINDEX_MASTER)) {
		RankingFlags |= AddNewModeRanking(newRanking, &Save.rankings[RANKINGINDEX_MASTER], Save.masterMedalRankings, RANKINGINDEX_MASTER, NUMRANKINGPLACES, RANKINGMODE_MASTER);
	}
	if (newRanking->flags & (7 << RANKINGINDEX_MASTERTODAYSBEST)) {
		AddNewModeRanking(newRanking, MasterTodaysBestRankings, MasterTodaysBestMedalRankings, RANKINGINDEX_MASTERTODAYSBEST, NUMRANKINGPLACES, RANKINGMODE_MASTER);
	}

	if (newRanking->flags & (7 << RANKINGINDEX_DOUBLES)) {
		RankingFlags |= AddNewModeRanking(newRanking, &Save.rankings[RANKINGINDEX_DOUBLES], Save.doublesLevelRankings, RANKINGINDEX_DOUBLES, NUMRANKINGPLACES, RANKINGMODE_DOUBLES);
	}
	if (newRanking->flags & (7 << RANKINGINDEX_DOUBLESTODAYSBEST)) {
		AddNewModeRanking(newRanking, DoublesTodaysBestTimeRankings, DoublesTodaysBestLevelRankings, RANKINGINDEX_DOUBLESTODAYSBEST, NUMRANKINGPLACES, RANKINGMODE_DOUBLES);
	}

	Ranking* ranking = &Save.rankings[RANKINGINDEX_MASTERSECTIONTIMES];
	uint32_t* sectionTime = Grades[newRanking->player->num].sectionTimes;
	for (uint16_t section = 0u; section < 10u; sectionTime++, section++, ranking++) {
		if (newRanking->flags & ((1 << section) << RANKINGINDEX_MASTERSECTIONTIMES)) {
			MemCopy(sizeoffield(Ranking, name), ranking->name, newRanking->nameEntries->name);
			RANKINGDATA_SETGRADE(ranking->data, newRanking->player->grade);
			RANKINGDATA_SETVALUE(ranking->data, *sectionTime);
			if (newRanking->player->level == 999u) {
				RANKINGDATA_SETGREENLINE(ranking->data, true);
			}
			if (newRanking->player->miscFlags & MISC_ORANGELINE) {
				RANKINGDATA_SETORANGELINE(ranking->data, true);
			}
			RankingFlags |= (1 << section) << RANKINGINDEX_MASTERSECTIONTIMES;
		}
	}
}

void InitNameEntry(NameEntryData* nameEntry, Player* player) {
	nameEntry->player = player;

	nameEntry->timeoutFrames = TIME(0, 30, 0);
	nameEntry->numChars = 0u;
	nameEntry->charIndex = 0u;
	nameEntry->autoshiftFrames = CS(20);
	nameEntry->waitFrames = CS(50);

	nameEntry->name[3] = *NameEntryChars[NAMEENTRYCHARS_ENTERABLE - 1];
	nameEntry->name[2] = *NameEntryChars[NAMEENTRYCHARS_ENTERABLE - 1];
	nameEntry->name[1] = *NameEntryChars[NAMEENTRYCHARS_ENTERABLE - 1];
	nameEntry->name[0] = *NameEntryChars[NAMEENTRYCHARS_ENTERABLE - 1];
	nameEntry->name[4] = '\0';
}

#define NAMEENTRY_AUTOSHIFTFRAMES 12
bool UpdateNameEntry(NameEntryData* nameEntry) {
	bool done = false;

	if (nameEntry->waitFrames != 0u) {
		nameEntry->waitFrames--;
	}

	const ButtonInput buttonsDown = GameButtonsDown[nameEntry->player->num];
	const ButtonInput buttonsNew = GameButtonsNew[nameEntry->player->num];
	switch (nameEntry->numChars) {
	case 0:
	case 1:
	case 2:
		if (buttonsNew & BUTTON_RIGHT) {
			nameEntry->charIndex++;
			nameEntry->charIndex %= lengthof(NameEntryChars);
			if (nameEntry->numChars == 0 && nameEntry->charIndex == NAMEENTRYCHARS_BACKSPACE) {
				nameEntry->charIndex = NAMEENTRYCHARS_END;
			}
			nameEntry->autoshiftFrames = NAMEENTRY_AUTOSHIFTFRAMES;
		}

		if (buttonsNew & BUTTON_LEFT) {
			if (nameEntry->charIndex-- == 0) {
				nameEntry->charIndex = NAMEENTRYCHARS_END;
			}
			if (nameEntry->numChars == 0 && nameEntry->charIndex == NAMEENTRYCHARS_BACKSPACE) {
				nameEntry->charIndex = NAMEENTRYCHARS_ENTERABLE - 1;
			}
			nameEntry->autoshiftFrames = NAMEENTRY_AUTOSHIFTFRAMES;
		}

		if ((buttonsDown & (BUTTON_LEFT | BUTTON_RIGHT)) && nameEntry->autoshiftFrames-- == 0) {
			if (buttonsDown & BUTTON_RIGHT) {
				nameEntry->charIndex++;
				nameEntry->charIndex %= lengthof(NameEntryChars);
				if (nameEntry->numChars == 0 && nameEntry->charIndex == NAMEENTRYCHARS_BACKSPACE) {
					nameEntry->charIndex = NAMEENTRYCHARS_END;
				}
			}
			else {
				if (nameEntry->charIndex-- == 0) {
					nameEntry->charIndex = NAMEENTRYCHARS_END;
				}
				if (nameEntry->numChars == 0 && nameEntry->charIndex == NAMEENTRYCHARS_BACKSPACE) {
					nameEntry->charIndex = NAMEENTRYCHARS_ENTERABLE - 1;
				}
			}
			nameEntry->autoshiftFrames = NAMEENTRY_AUTOSHIFTFRAMES;
		}

		if ((buttonsNew & BUTTON_1) && nameEntry->waitFrames == 0) {
			PlaySoundEffect(SOUNDEFFECT_SELECT);
			switch (nameEntry->charIndex) {
			case NAMEENTRYCHARS_BACKSPACE:
				if (nameEntry->numChars != 0) {
					nameEntry->name[--nameEntry->numChars] = ' ';
					if (nameEntry->numChars == 0) {
						nameEntry->charIndex = 0;
					}
				}
				break;

			case NAMEENTRYCHARS_END:
				done = true;
				nameEntry->numChars = 4;
				nameEntry->charIndex = NAMEENTRYCHARS_ENTERABLE - 1;
				break;

			default:
				nameEntry->name[nameEntry->numChars] = *NameEntryChars[nameEntry->charIndex];
				if (++nameEntry->numChars == 3) {
					nameEntry->charIndex = NAMEENTRYCHARS_END;
				}
				break;
			}
		}
		else if (buttonsNew & BUTTON_2) {
			PlaySoundEffect(SOUNDEFFECT_SELECT);
			if (nameEntry->numChars != 0) {
				if (--nameEntry->numChars == 0) {
					nameEntry->charIndex = 0;
				}
				nameEntry->name[nameEntry->numChars] = ' ';
			}
		}
		break;

	case 3:
		if ((buttonsNew & BUTTON_RIGHT) && nameEntry->charIndex++ == NAMEENTRYCHARS_END) {
			nameEntry->charIndex = NAMEENTRYCHARS_BACKSPACE;
		}
		if ((buttonsNew & BUTTON_LEFT) && nameEntry->charIndex-- == NAMEENTRYCHARS_BACKSPACE) {
			nameEntry->charIndex = NAMEENTRYCHARS_END;
		}
		if (buttonsNew & BUTTON_1) {
			PlaySoundEffect(SOUNDEFFECT_SELECT);
			if (nameEntry->charIndex == NAMEENTRYCHARS_BACKSPACE) {
				nameEntry->name[--nameEntry->numChars] = ' ';
			}
			else {
				done = true;
				nameEntry->numChars = 4;
				nameEntry->charIndex = NAMEENTRYCHARS_ENTERABLE - 1;
			}
		}
		else if (buttonsNew & BUTTON_2) {
			PlaySoundEffect(SOUNDEFFECT_SELECT);
			nameEntry->name[--nameEntry->numChars] = ' ';
		}
		break;

	case 4:
	default:
		return true;
	}

	if (--nameEntry->timeoutFrames == 0) {
		done = true;
		nameEntry->charIndex = NAMEENTRYCHARS_ENTERABLE - 1;
	}

	if (done) {
		for (size_t i = 0u; StringNCompare(InvalidNames[i], InvalidNames[lengthof(InvalidNames) - 1], 3u) != 0; i++) {
			if (StringNCompare(InvalidNames[i], nameEntry->name, 3u) == 0) {
				MemCopy(sizeof(DefaultName), nameEntry->name, DefaultName);
				break;
			}
		}
		nameEntry->name[3] = '\0';
		PlaySoundEffect(SOUNDEFFECT_CHEER);
	}
	return done;
}

bool UpdatePlayRanking(Player* player) {
	NewRankingData* newRanking = &NewRankings[player->num];

	if ((GameFlags & GAME_TWIN) && GameButtonsDown[player->num] == (BUTTON_3 | BUTTON_2 | BUTTON_1)) {
		player->nowFlags |= NOW_SHOWRANKINGCODE;
	}

	if (player->modeFlags & (MODE_20G | MODE_BIG | MODE_ITEM | MODE_TLS)) {
		player->nowFlags &= ~NOW_SHOWRANKINGCODE;
	}

	if (player->values[2] == 1) {
		if (!(player->nowFlags & NOW_SHOWRANKINGCODE) && !newRanking->flags) {
			player->nowFlags &= ~NOW_NAMEENTRY;
			NextPlay(player, (PlayData) { .flags = PLAYFLAG_NONE, .state = PLAYSTATE_GAMEOVER });
			return true;
		}
		else {
			newRanking->labelScaleX += 0x04;
			if (newRanking->labelScaleX > UNSCALED) {
				newRanking->labelScaleX = UNSCALED;
			}
			switch (newRanking->state) {
				case NEWRANKING_INIT:
					newRanking->state++;
					newRanking->labelScaleFrames = 18u;
					newRanking->labelScaleX = 0x04u;
					player->nowFlags |= NOW_NAMEENTRY;
					if (newRanking->player->modeFlags & MODE_DOUBLES) {
						InitNameEntry(&newRanking->nameEntries[PLAYER1], &Players[PLAYER1]);
						InitNameEntry(&newRanking->nameEntries[PLAYER2], &Players[PLAYER2]);
					}
					else {
						InitNameEntry(newRanking->nameEntries, newRanking->player);
					}
				case NEWRANKING_NAMEENTRY:
					if (newRanking->labelScaleFrames != 0u) {
						newRanking->labelScaleFrames--;
					}

					if (newRanking->player->modeFlags & MODE_DOUBLES) {
						if (newRanking->labelScaleFrames == 0u && UpdateNameEntry(&newRanking->nameEntries[PLAYER1]) && UpdateNameEntry(&newRanking->nameEntries[PLAYER2])) {
							newRanking->state++;
							newRanking->flashFrames = 192;
							AddNewRanking(newRanking);
						}
						ShowPlayRankingDoubles(newRanking, ENTRYFLASH_FALSE);
					}
					else {
						if (newRanking->labelScaleFrames == 0u && UpdateNameEntry(newRanking->nameEntries)) {
							newRanking->state++;
							newRanking->flashFrames = 192;
							AddNewRanking(newRanking);
						}
						ShowPlayRanking(newRanking, ENTRYFLASH_FALSE);
					}
					return false;

				case NEWRANKING_END:
					if (newRanking->player->modeFlags & MODE_DOUBLES) {
						ShowPlayRankingDoubles(newRanking, ENTRYFLASH_TRUE);
					}
					else {
						ShowPlayRanking(newRanking, ENTRYFLASH_TRUE);
					}

					if (--newRanking->flashFrames < 0) {
						player->nowFlags &= ~NOW_NAMEENTRY;
						NextPlay(player, (PlayData){.flags = PLAYFLAG_NONE, .state = PLAYSTATE_GAMEOVER});
						if (player->nowFlags & NOW_SHOWRANKINGCODE) {
							player->values[0] = 1440;
						}
						NewRankingCode(player);
						return true;
					}
					else {
						return false;
					}

				default:
					return false;
			}
		}
	}
	// BUG: Likely an oversight here; the original code probably only had
	// "if (player->values[2] == 1)", with no else-case, resulting in
	// whatever was in the return value register being returned.
	else {
		return player->values[2];
	}
}

static void ShowPlayRanking(NewRankingData* newRanking, EntryFlash entryFlash) {
	Player* player = newRanking->player;
	if ((player->modeFlags & (MODE_NORMAL | MODE_MASTER | MODE_TGMPLUS | MODE_TADEATH)) && !newRanking->flags) {
		ShowPlayRankingCodeNameEntry(newRanking, entryFlash);
	}
	else {
		int16_t rankingX = player->num * 160;
		int16_t labelX = rankingX + 20;
		DisplayObjectEx(OBJECTPTR(0x243), 42, labelX, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
		DisplayObjectEx(OBJECTPTR(0x244), 85, labelX, 0u, 100u, UNSCALED, newRanking->labelScaleX, false);
		if (player->modeFlags & MODE_MASTER) {
			DisplayObjectEx(OBJECTPTR(0x245), 128, labelX, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
		}
		else if (player->modeFlags & MODE_NORMAL) {
			DisplayObjectEx(OBJECTPTR(0x24B), 128, labelX, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
		}
		DisplayObjectEx(OBJECT_NAMEENTRYLABEL, 171, labelX, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
		if (newRanking->labelScaleFrames == 0u) {
			int16_t rankingPlaceX = rankingX + 38;
			DisplayObject(ObjectTableRankingPlaces[newRanking->place], 62, rankingPlaceX, 0u, 110u);
			DisplayObject(ObjectTableRankingPlaces[newRanking->todaysBestPlace], 105, rankingPlaceX, 0u, 110u);
			if (player->modeFlags & MODE_MASTER) {
				ShowClearTime(player->masteringTime, 148, labelX);
			}
			else if (player->modeFlags & MODE_NORMAL) {
				ShowRankingScore(player->score, 148, rankingX + 24);
			}
			uint8_t flashPal = ScreenTime % 4 ? 8u : 0u;
			if (entryFlash == ENTRYFLASH_TRUE) {
				ShowRankingName(newRanking->nameEntries->name, 183, rankingX + 36, flashPal, 110u);
			}
			else {
				ShowRankingName(newRanking->nameEntries->name, 183, rankingX + 36, 0u, 110u);
			}
			DisplayObject(ObjectTableRankingChars[*NameEntryChars[newRanking->nameEntries->charIndex] - ' '], 183, rankingX + 36 + newRanking->nameEntries->numChars * 16, flashPal, 110u);
		}
	}
}

static void ShowPlayRankingCodeNameEntry(NewRankingData* newRanking, EntryFlash entryFlash) {
	int16_t labelX = newRanking->player->num * 160;
	DisplayObjectEx(OBJECT_NAMEENTRYLABEL, 105, labelX + 20, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
	if (newRanking->labelScaleFrames != 0u) {
		return;
	}

	uint8_t flashPalNum = ScreenTime % 4 ? 8u : 0u;
	if (entryFlash == ENTRYFLASH_TRUE) {
		ShowRankingName(newRanking->nameEntries->name, 117, labelX + 36, flashPalNum, 110u);
	}
	else {
		ShowRankingName(newRanking->nameEntries->name, 117, labelX + 36, 0u, 110u);
	}
	DisplayObject(ObjectTableRankingChars[*NameEntryChars[newRanking->nameEntries->charIndex] - ' '], 117, labelX + 36 + newRanking->nameEntries->numChars * 16, flashPalNum, 110u);
}

static void ShowPlayRankingDoubles(NewRankingData* newRanking, EntryFlash entryFlash) {
	DisplayObjectEx(OBJECT_DOUBLESRANKINGLABEL, 43, 107, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
	DisplayObjectEx(OBJECT_DOUBLESTODAYSBESTRANKINGLABEL, 80, 107, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
	DisplayObjectEx(OBJECT_DOUBLESCLEARTIMELABEL, 117, 107, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
	DisplayObjectEx(OBJECT_DOUBLESNAMELEVELRANKINGLABEL, 107 + 47, 107, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
	DisplayObjectEx(OBJECT_DOUBLES1PRANKINGLABEL, 107 + 65, 107, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);
	DisplayObjectEx(OBJECT_DOUBLES2PRANKINGLABEL, 107 + 83, 107, 0u, 110u, UNSCALED, newRanking->labelScaleX, false);

	if (newRanking->labelScaleFrames != 0u) {
		return;
	}

	DisplayObject(ObjectTableRankingPlaces[newRanking->place], 56, 138, 0u, 110u);
	DisplayObject(ObjectTableRankingPlaces[newRanking->todaysBestPlace], 93, 138, 0u, 110u);

	ShowClearTime(newRanking->player->clearTime, 130, 121);

	uint8_t flashPal = ScreenTime % 4 ? 8u : 0u;

	ShowRankingName(newRanking->nameEntries[PLAYER1].name, 167, 124, entryFlash == ENTRYFLASH_TRUE || newRanking->nameEntries[PLAYER1].numChars > 3 ? flashPal : 0u, 110u);
	DisplayObject(ObjectTableRankingChars[*NameEntryChars[newRanking->nameEntries[PLAYER1].charIndex] - ' '], 167, 124, flashPal, 110u);
	ShowStatusNumEx(newRanking->nameEntries[PLAYER1].player->level, 169, 189, 0u, 110u, 3, false, NUMALIGN_RIGHT);

	ShowRankingName(newRanking->nameEntries[PLAYER2].name, 185, 124, entryFlash == ENTRYFLASH_TRUE || newRanking->nameEntries[PLAYER2].numChars > 3 ? flashPal : 0u, 110u);
	DisplayObject(ObjectTableRankingChars[*NameEntryChars[newRanking->nameEntries[PLAYER2].charIndex] - ' '], 185, 124 + newRanking->nameEntries[PLAYER2].charIndex * 16, flashPal, 110u);
	ShowStatusNumEx(newRanking->nameEntries[PLAYER2].player->level, 187, 189, 0u, 110u, 3, false, NUMALIGN_RIGHT);
}
