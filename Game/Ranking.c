#include "Ranking.h"
#include "Player.h"
#include "Debug.h"
#include "ShowGameStatus.h"
#include "ShowClearTime.h"
#include "ShowObject.h"
#include "Grade.h"
#include "Button.h"
#include "Frame.h"
#include "Save.h"
#include "Entity.h"
#include "Object.h"
#include "HwSprite.h"
#include "SoundEffect.h"
#include "Eeprom.h"

static Ranking MasterTodaysBestRankings[NUMRANKINGPLACES];
static uint16_t MasterTodaysBestMedalRankings[NUMRANKINGPLACES];

static Ranking NormalTodaysBestRankings[NUMRANKINGPLACES];

static Ranking DoublesTodaysBestTimeRankings[NUMRANKINGPLACES];
static Ranking DoublesTodaysBestLevelRankings[NUMRANKINGPLACES];

static RankingFlag RankingFlags;

NewRankingData NewRankings[NUMPLAYERS];

#define _CHAR(i) &OBJECTTABLE_RANKINGCHARS[i]
static const ObjectData* ObjectTableRankingChars[61] = {
	_CHAR(0), _CHAR(1), _CHAR(2), _CHAR(3), _CHAR(4),
	_CHAR(5), _CHAR(6), _CHAR(7), _CHAR(8), _CHAR(9),
	_CHAR(10), _CHAR(11), _CHAR(12), _CHAR(13), _CHAR(14),
	_CHAR(15), _CHAR(16), _CHAR(17), _CHAR(18), _CHAR(19),
	_CHAR(20), _CHAR(21), _CHAR(22), _CHAR(23), _CHAR(24),
	_CHAR(25), _CHAR(26), _CHAR(27), _CHAR(28), _CHAR(29),
	_CHAR(30), _CHAR(31), _CHAR(32), _CHAR(33), _CHAR(34),
	_CHAR(35), _CHAR(36), _CHAR(37), _CHAR(38), _CHAR(39),
	_CHAR(40), _CHAR(41), _CHAR(42), _CHAR(43), _CHAR(44),
	_CHAR(45), _CHAR(46), _CHAR(47), _CHAR(48), _CHAR(49),
	_CHAR(50), _CHAR(51), _CHAR(52), _CHAR(53), _CHAR(54),
	_CHAR(55), _CHAR(56), _CHAR(57), _CHAR(58), _CHAR(59),
	_CHAR(60)
};
#undef _CHAR

static const char* NameEntryChars[42] = {
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
	"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
	"U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3",
	"4", "5", "6", "7", "8", "9", "!", "?", ".", " ",
	"\\", "["
};

static const char* DefaultName = "ARK";

#define NUMINVALIDNAMES 20
static const char* InvalidNames[NUMINVALIDNAMES + 1] = {
	"   ", "A  ", "AA ", "AAA", "666",
	"AHO", "ASS", "AUM", "DIE", "ETA",
	"FUC", "FUK", "HIV", "IRA", "KKK",
	"OSI", "PEE", "PIS", "PLO", "SEX",
	"end"
};

static const ObjectData* ObjectTableRankingPlaces[NUMRANKINGPLACES + 1] = {
	&OBJECTTABLE_RANKINGPLACES[1],
	&OBJECTTABLE_RANKINGPLACES[2],
	&OBJECTTABLE_RANKINGPLACES[3],
	&OBJECTTABLE_RANKINGPLACES[0]
};

static void ShowRankingName(const char* name, int16_t y, int16_t x, uint8_t palNum, uint16_t layer);
static void _0x6011AD2(uint32_t score, int16_t y, int16_t x);
static void ShowRankingScore(uint32_t score, int16_t y, int16_t x);
static void ShowRankingTime(uint32_t time, int16_t y, int16_t x);
static void _0x6011D28(int16_t y, int16_t x);
static void _0x6011DB0(int16_t y, int16_t x);
static void _0x6011DF0(int16_t y, int16_t x);
static void _0x6011E48(int16_t y, int16_t x);
static void _0x6011E88(Entity* entity);
// _0x60122D8
// _0x6012354
// _0x6012828

static uint32_t RankingsChecksum();

static RankingPlace MasterRankingPlace(Player* player, Ranking* rankings, int16_t numRankings);
static RankingPlace NormalRankingPlace(Player* player, Ranking* rankings, int16_t numRankings);
static RankingPlace DoublesRankingPlace(uint16_t levelSum, uint32_t time, Ranking* timeRankings, Ranking* levelRankings, int16_t numRankings);

static RankingFlag NewRankingFlags(Player* player, NewRankingData* newRanking);

static RankingFlag AddNewModeRanking(NewRankingData* newRanking, Ranking* rankings, void* rankingExtras, RankingIndex rankingIndex, int16_t numRankings, RankingMode rankingMode);
static void AddNewRanking(NewRankingData* newRanking);

static void InitNameEntry(NameEntryData* nameEntry, Player* player);
static bool UpdateNameEntry(NameEntryData* nameEntry);

typedef enum EntryFlash {
	ENTRYFLASH_FALSE = 0x00,
	ENTRYFLASH_TRUE = 0xFF
} EntryFlash;
static void ShowRanking(NewRankingData* newRanking, EntryFlash entryFlash);
static void ShowRankingCodeNameEntry(NewRankingData* newRanking, EntryFlash entryFlash);
static void ShowRankingDoubles(NewRankingData* newRanking, EntryFlash entryFlash);

void ShowSectionTimesEx(uint32_t* sectionTimes, int16_t y, int16_t x) {
	uint32_t clearTime = 0u;
	for (size_t section = 0u; section < 10u; section++, y += 12) {
		ShowClearTime(sectionTimes[section], y, x);
		clearTime += sectionTimes[section];
	}
	ShowClearTime(clearTime, y + 32, x);
}

void _0x6011840() {
	// TODO: Empty. Check TGM2 to see if there's code there.
}

uint32_t SectionTimes[10];
void ShowSectionTimes() {
	ShowSectionTimesEx(SectionTimes, 40, 170);
}

void _0x6011854() {
	// TODO: Check TGM2 to see if this contains any code. Just calls ShowSectionTimes() in TAP.
	ShowSectionTimes();
}
void _0x6011858() {
	// TODO: Check TGM2 to see if this contains any code. Does nothing in TAP.
}

// TODO: Use macros here instead of hard-coded hex numbers.
#define _TA { 'T', '.', 'A', '\0' }

// All-time default rankings.

static const Ranking DefaultRankings[NUMRANKINGS] = {
	{ _TA, 0x00001518u },
	{ _TA, 0x08001518u },
	{ _TA, 0x10001518u },
	{ _TA, 0x18001518u },
	{ _TA, 0x20001518u },
	{ _TA, 0x28001518u },
	{ _TA, 0x30001518u },
	{ _TA, 0x38001518u },
	{ _TA, 0x40001518u },
	{ _TA, 0x48001518u },
	{ _TA, 0x48011940u },
	{ _TA, 0x3000FD20u },
	{ _TA, 0x1800E100u },
	{ _TA, 0x10001388u },
	{ _TA, 0x08000FA0u },
	{ _TA, 0x00000BB8u },
	{ _TA, 0x18011940u },
	{ _TA, 0x1000D2F0u },
	{ _TA, 0x08008CA0u },
};

static const Ranking DefaultDoublesLevelRankings[NUMRANKINGPLACES] = {
	{ _TA, 0x0096008Cu },
	{ _TA, 0x0064005Au },
	{ _TA, 0x00320028u }
};

// Today's best default rankings.

static const Ranking DefaultMasterTodaysBestRankings[NUMRANKINGPLACES] = {
	{ _TA, 0x18011940 },
	{ _TA, 0x1000FD20 },
	{ _TA, 0x0800E100 }
};

static const Ranking DefaultNormalTodaysBestRankings[NUMRANKINGPLACES] = {
	{ _TA, 0x00000BB8 },
	{ _TA, 0x000007D0 },
	{ _TA, 0x000003E8 }
};

static const Ranking DefaultDoublesTodaysBestTimeRankings[NUMRANKINGPLACES] = {
	{ _TA, 0x10011940 },
	{ _TA, 0x0800FD20 },
	{ _TA, 0x0000E100 }
};

static const Ranking DefaultDoublesTodaysBestLevelRankings[NUMRANKINGPLACES] = {
	{ _TA, 0x0032001E },
	{ _TA, 0x001E000A },
	{ _TA, 0x000A0000 }
};

#undef TA

void InitRankings() {
	RankingFlags = RANKINGFLAG_NONE;
	for (size_t i = 0u; i < NUMRANKINGS; i++) {
		Save->rankings[i] = DefaultRankings[i];
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		Save->doublesLevelRankings[place] = DefaultDoublesLevelRankings[place];
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		Save->masterMedalRankings[place] = 0u;
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

static void _0x6011AD2(uint32_t score, int16_t y, int16_t x) {
	if (score > 999999u) {
		score = 999999u;
	}

	x += 80;
	for (int16_t i = 0; i < 6; i++) {
		uint8_t digit = score % 10u;
		DisplayObject(ObjectTableRankingChars[0x10 + digit], y, x, 0u, 110u);
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
		DisplayObject(ObjectTableRankingDigits[score % 10], y, x, 0u, 110u);
		if ((score /= 10) == 0u) {
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

static void _0x6011D28(int16_t y, int16_t x) {
	ShowObject(OBJECTPTR(0x221), y, x + 31, 0u, 110u);
	ShowObject(OBJECTPTR(0x222), y, x + 85, 0u, 110u);
	ShowObject(OBJECTPTR(0x223), y, x + 126, 0u, 110u);

	ShowObject(OBJECTPTR(0x226), y, x + 215, 0u, 110u);
}

static void _0x6011DB0(int16_t y, int16_t x) {
	ShowObject(OBJECTPTR(0x221), y, x + 42, 0u, 110u);
	ShowObject(OBJECTPTR(0x227), y, x + 214, 0u, 110u);
}

static void _0x6011DF0(int16_t y, int16_t x) {
	ShowObject(OBJECTPTR(0x225), y, x + 10, 0, 110u);
	ShowObject(OBJECTPTR(0x221), y, x + 97, 0u, 110u);
	ShowObject(OBJECTPTR(0x222), y, x + 166, 0u, 110u);
	ShowObject(OBJECTPTR(0x223), y, x + 229, 0u, 110u);
}

static void _0x6011E48(int16_t y, int16_t x) {
	ShowObject(OBJECTPTR(0x224), y, x + 35, 0u, 110u);
	ShowObject(OBJECTPTR(0x223), y, x + 230, 0u, 110u);
}

// TODO: Entity updater.
// _0x6011E88

// TODO: Instantiates an entity updated by _0x6011E88
// _0x60122D8

// _0x6012354

// _0x6012828

static uint32_t RankingsChecksum() {
	uint32_t checksum = 0u;

	for (size_t i = 0u; i < NUMRANKINGS; i++) {
		Ranking* ranking = &Save->rankings[i];
		checksum += (uint32_t)ranking->name[0] << 8 | ranking->name[1];
		checksum += (uint32_t)ranking->name[2] << 8 | ranking->name[3];
		checksum += ranking->data >> 16;
		checksum += ranking->data & 0xFFFFu;
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		Ranking* ranking = &Save->doublesLevelRankings[place];
		checksum += (uint32_t)ranking->name[0] << 8 | ranking->name[1];
		checksum += (uint32_t)ranking->name[2] << 8 | ranking->name[3];
		checksum += ranking->data >> 16;
		checksum += ranking->data & 0xFFFFu;
	}

	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		checksum += Save->masterMedalRankings[place];
	}

	return ++checksum;
}

void CheckSaveRankings() {
	if (!Debug && RankingFlags) {
		uint16_t checksum = RankingsChecksum();
		Save->rankingsChecksum = checksum;
		WriteSave(rankingsChecksum);

		UpdateFrame();

		for (size_t i = 0u; i < NUMRANKINGS; i++) {
			RankingFlag rankingFlag = 1 << i;
			if (RankingFlags & rankingFlag) {
				RankingFlags &= ~rankingFlag;

				WriteSave(rankings[i]);

				if (i >= RANKINGINDEX_DOUBLES) {
					WriteSave(doublesLevelRankings[i - RANKINGINDEX_DOUBLES]);
				}

				if (i >= RANKINGINDEX_MASTER && i < RANKINGINDEX_NORMAL) {
					WriteSave(masterMedalRankings[i - RANKINGINDEX_MASTER]);
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

	ReadEeprom(offsetofSaveData(rankingsChecksum), &checksum, sizeof(uint16_t));
	Save->rankingsChecksum = checksum;

	for (size_t i = 0u; i < NUMRANKINGS; i++) {
		ReadSave(rankings[i]);
	}
	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		ReadSave(doublesLevelRankings[place]);
	}
	for (RankingPlace place = RANKINGPLACE_FIRST; place < NUMRANKINGPLACES; place++) {
		ReadSave(masterMedalRankings[place]);
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
		if (player->grade > RANKINGDATA_GETGRADE(ranking->data)) {
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
	       
		place = DoublesRankingPlace(Players[PLAYER1].level + Players[PLAYER2].level, player->clearTime, &Save->rankings[RANKINGINDEX_DOUBLES], Save->doublesLevelRankings, NUMRANKINGPLACES);
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

		place = MasterRankingPlace(player, &Save->rankings[RANKINGINDEX_MASTER], NUMRANKINGPLACES);
		if (place >= RANKINGPLACE_FIRST) {
			newRankingFlags |= 1 << (place + RANKINGINDEX_MASTER);
			newRanking->place = place;
		}

		place = MasterRankingPlace(player, MasterTodaysBestRankings, NUMRANKINGPLACES);
		if (place >= RANKINGPLACE_FIRST) {
			newRankingFlags |= 1 << (place + RANKINGINDEX_MASTERTODAYSBEST);
			newRanking->todaysBestPlace = place;
		}

		Ranking* ranking = &Save->rankings[RANKINGINDEX_MASTERSECTIONTIMES];
		uint32_t* sectionTime = Grades[player->num].sectionTimes;
		for (int16_t section = 0; section < 10; section++) {
			if (*sectionTime != 0u && *sectionTime < RANKINGDATA_GETVALUE(ranking->data)) {
				newRankingFlags |= 1 << (section + RANKINGINDEX_MASTERSECTIONTIMES);
			}
		}
	}
	else if (player->modeFlags & MODE_NORMAL) {
		RankingPlace place;

		place = NormalRankingPlace(player, &Save->rankings[RANKINGINDEX_NORMAL], NUMRANKINGPLACES);
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

typedef enum RankingMode {
	RANKINGMODE_MASTER,
	RANKINGMODE_NORMAL,
	RANKINGMODE_DOUBLES
} RankingMode;

static RankingFlag AddNewModeRanking(NewRankingData* newRanking, Ranking* rankings, void* rankingExtras, RankingIndex rankingIndex, int16_t numRankings, RankingMode rankingMode) {
	uint16_t* masterMedalRankings = (uint16_t*)rankingExtras;
	Ranking* doublesLevelRankings = (Ranking*)rankingExtras;
	RankingFlag rankingFlags = RANKINGFLAG_NONE;
	RankingIndex rankingBit = rankingIndex + numRankings - 1;
	Player* player1 = &Players[PLAYER1];
	Player* player2 = &Players[PLAYER2];
	for (int16_t place = numRankings - 1; place >= RANKINGPLACE_FIRST; place--) {
		rankingFlags |= 1 << rankingBit;
		if (newRanking->flags & (1 << rankingBit)) {
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
					int16_t medalRankingBit = 0;
					MedalColor* medal = newRanking->player->medalColors;
					for (size_t i = 0; i < NUMMEDALTYPES; i++, medalRankingBit += 2) {
						masterMedalRankings[place] |= (*medal++ & 3) << medalRankingBit;
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
					*(uint32_t*)doublesLevelRankings[place].name = *(uint32_t*)newRanking->nameEntries[PLAYER2].name;
					RANKINGDATA_SET1PLEVEL(doublesLevelRankings[place].data, player1->level);
					RANKINGDATA_SET2PLEVEL(doublesLevelRankings[place].data, player2->level);
					break;

				default:
					break;
			}
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
		RankingFlags |= AddNewModeRanking(newRanking, &Save->rankings[RANKINGINDEX_NORMAL], NULL, RANKINGINDEX_NORMAL, NUMRANKINGPLACES, RANKINGMODE_NORMAL);
	}
	if (newRanking->flags & (7 << RANKINGINDEX_NORMALTODAYSBEST)) {
		AddNewModeRanking(newRanking, NormalTodaysBestRankings, NULL, RANKINGINDEX_NORMALTODAYSBEST, NUMRANKINGPLACES, RANKINGMODE_NORMAL);
	}

	if (newRanking->flags & (7 << RANKINGINDEX_MASTER)) {
		RankingFlags |= AddNewModeRanking(newRanking, &Save->rankings[RANKINGINDEX_MASTER], Save->masterMedalRankings, RANKINGINDEX_MASTER, NUMRANKINGPLACES, RANKINGMODE_MASTER);
	}
	if (newRanking->flags & (7 << RANKINGINDEX_MASTERTODAYSBEST)) {
		AddNewModeRanking(newRanking, MasterTodaysBestRankings, MasterTodaysBestMedalRankings, RANKINGINDEX_MASTERTODAYSBEST, NUMRANKINGPLACES, RANKINGMODE_MASTER);
	}

	if (newRanking->flags & (7 << RANKINGINDEX_DOUBLES)) {
		RankingFlags |= AddNewModeRanking(newRanking, &Save->rankings[RANKINGINDEX_DOUBLES], Save->doublesLevelRankings, RANKINGINDEX_DOUBLES, NUMRANKINGPLACES, RANKINGMODE_DOUBLES);
	}
	if (newRanking->flags & (7 << RANKINGINDEX_DOUBLESTODAYSBEST)) {
		AddNewModeRanking(newRanking, DoublesTodaysBestTimeRankings, DoublesTodaysBestLevelRankings, RANKINGINDEX_DOUBLESTODAYSBEST, NUMRANKINGPLACES, RANKINGMODE_DOUBLES);
	}

	Ranking* ranking = &Save->rankings[RANKINGINDEX_MASTERSECTIONTIMES];
	uint32_t* sectionTime = Grades[newRanking->player->num].sectionTimes;
	for (uint16_t section = 0u; section < 10u; section++, sectionTime++) {
		if (newRanking->flags & ((1 << section) << RANKINGINDEX_MASTERSECTIONTIMES)) {
			*(uint32_t*)ranking->name = *(uint32_t*)newRanking->nameEntries->name;
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

static void InitNameEntry(NameEntryData* nameEntry, Player* player) {
	nameEntry->player = player;

	nameEntry->timeoutFrames = TIME(0, 30, 0);

	nameEntry->numChars = 0u;
	nameEntry->charIndex = 0u;
	nameEntry->autoshiftFrames = 12u;

	nameEntry->name[3] = *NameEntryChars[39];
	nameEntry->name[2] = *NameEntryChars[39];
	nameEntry->name[1] = *NameEntryChars[39];
	nameEntry->name[0] = *NameEntryChars[39];
	nameEntry->name[4] = '\0';
}

// TODO: Replace numeric literals assigned to charIndex with defined constants.
static bool UpdateNameEntry(NameEntryData* nameEntry) {
	bool done = false;

	if (nameEntry->waitFrames != 0u) {
		nameEntry->waitFrames--;
	}

	ButtonInput buttonsDown = GameButtonsDown[nameEntry->player->num];
	ButtonInput buttonsNew = GameButtonsNew[nameEntry->player->num];
	switch (nameEntry->numChars) {
	case 0:
	case 1:
	case 2:
		if (buttonsNew & BUTTON_RIGHT) {
			nameEntry->charIndex++;
			nameEntry->charIndex %= 42;
			if (nameEntry->numChars == 0 && nameEntry->charIndex == 40) {
				nameEntry->charIndex = 41;
			}
			nameEntry->autoshiftFrames = 12;
		}

		if (buttonsNew & BUTTON_LEFT) {
			if (nameEntry->charIndex-- == 0) {
				nameEntry->charIndex = 41;
			}
			if (nameEntry->numChars == 0 && nameEntry->charIndex == 40) {
				nameEntry->charIndex = 39;
			}
			nameEntry->autoshiftFrames = 12;
		}

		if (buttonsDown & (BUTTON_LEFT | BUTTON_RIGHT)) {
			if (nameEntry->autoshiftFrames-- == 0) {
				if (buttonsDown & BUTTON_RIGHT) {
					nameEntry->charIndex++;
					nameEntry->charIndex %= 42;
					if (nameEntry->numChars == 0 && nameEntry->charIndex == 40) {
						nameEntry->charIndex = 41;
					}
				}
				else {
					if (nameEntry->charIndex-- == 0) {
						nameEntry->charIndex = 41;
					}
					if (nameEntry->numChars == 0 && nameEntry->charIndex == 40) {
						nameEntry->charIndex = 39;
					}
				}
			}
			nameEntry->autoshiftFrames = 12;
		}

		if ((buttonsNew & BUTTON_1) && nameEntry->waitFrames == 0) {
			PlaySoundEffect(SOUNDEFFECT_SELECT);
			if (nameEntry->charIndex == 40) {
				if (nameEntry->numChars != 0) {
					nameEntry->name[--nameEntry->numChars] = ' ';
					if (nameEntry->numChars == 0) {
						nameEntry->charIndex = 0;
					}
				}
			}
			else if (nameEntry->charIndex == 41) {
				nameEntry->numChars = 4;
				nameEntry->charIndex = 39;
			}
			else {
				nameEntry->name[nameEntry->numChars] = *NameEntryChars[nameEntry->charIndex];
				if (++nameEntry->numChars == 3) {
					nameEntry->charIndex = 41;
				}
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
		if ((buttonsNew & BUTTON_RIGHT) && nameEntry->charIndex++ == 41) {
			nameEntry->charIndex = 40;
		}
		if ((buttonsNew & BUTTON_LEFT) && nameEntry->charIndex-- == 40) {
			nameEntry->charIndex = 41;
		}
		if (buttonsNew & BUTTON_1) {
			PlaySoundEffect(SOUNDEFFECT_SELECT);
			if (nameEntry->charIndex == 40) {
				nameEntry->name[--nameEntry->numChars] = ' ';
			}
			else {
				nameEntry->numChars = 4;
				nameEntry->charIndex = 39;
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
		nameEntry->charIndex = 39;
	}

	if (done) {
		for (size_t i = 0; StringNCompare(InvalidNames[i], InvalidNames[NUMINVALIDNAMES], 3) != 0; i++) {
			if (StringNCompare(InvalidNames[i], nameEntry->name, 3) == 0) {
				*(uint32_t*)nameEntry->name = *(const uint32_t*)DefaultName;
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

	if ((GameFlags & GAME_TWIN) && ROTATED_ANY(GameButtonsDown[player->num])) {
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
						ShowRankingDoubles(newRanking, ENTRYFLASH_FALSE);
					}
					else {
						if (newRanking->labelScaleFrames == 0u && UpdateNameEntry(newRanking->nameEntries)) {
							newRanking->state++;
							newRanking->flashFrames = 192;
							AddNewRanking(newRanking);
						}
						ShowRanking(newRanking, ENTRYFLASH_FALSE);
					}
					return false;

				case NEWRANKING_END:
					if (newRanking->player->modeFlags & MODE_DOUBLES) {
						ShowRankingDoubles(newRanking, ENTRYFLASH_TRUE);
					}
					else {
						ShowRanking(newRanking, ENTRYFLASH_TRUE);
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

static void ShowRanking(NewRankingData* newRanking, EntryFlash entryFlash) {
	Player* player = newRanking->player;
	if ((player->modeFlags & (MODE_NORMAL | MODE_MASTER | MODE_TGMPLUS | MODE_TADEATH)) && !newRanking->flags) {
		ShowRankingCodeNameEntry(newRanking, entryFlash);
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
				ShowTime(player->masteringTime, 148, labelX);
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

static void ShowRankingCodeNameEntry(NewRankingData* newRanking, EntryFlash entryFlash) {
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

static void ShowRankingDoubles(NewRankingData* newRanking, EntryFlash entryFlash) {
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
	ShowStatusNumberEx(newRanking->nameEntries[PLAYER1].player->level, 169, 189, 0u, 110u, 3, false, NUMALIGN_RIGHT);

	ShowRankingName(newRanking->nameEntries[PLAYER2].name, 185, 124, entryFlash == ENTRYFLASH_TRUE || newRanking->nameEntries[PLAYER2].numChars > 3 ? flashPal : 0u, 110u);
	DisplayObject(ObjectTableRankingChars[*NameEntryChars[newRanking->nameEntries[PLAYER2].charIndex] - ' '], 185, 124 + newRanking->nameEntries[PLAYER2].charIndex * 16, flashPal, 110u);
	ShowStatusNumberEx(newRanking->nameEntries[PLAYER2].player->level, 187, 189, 0u, 110u, 3, false, NUMALIGN_RIGHT);
}
