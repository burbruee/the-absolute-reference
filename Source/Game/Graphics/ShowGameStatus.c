#include "Game/Graphics/ShowGameStatus.h"
#include "Game/Graphics/ShowObject.h"
#include "Game/Graphics/Layer.h"
#include "Game/Play/Player.h"
#include "Game/PalNum.h"
#include "Video/Object.h"
#include "Video/HwSprite.h"
#include "Main/MainLoop.h"
#include "Main/GameLoop.h"
#include "Main/Frame.h"
#include "Input/Input.h"
#include "Input/Credit.h"
#include "Eeprom/Setting.h"

const ObjectData* ObjectTableStatusDigits[10] = {
	&OBJECTTABLE_STATUSDIGITS[0],
	&OBJECTTABLE_STATUSDIGITS[1],
	&OBJECTTABLE_STATUSDIGITS[2],
	&OBJECTTABLE_STATUSDIGITS[3],
	&OBJECTTABLE_STATUSDIGITS[4],
	&OBJECTTABLE_STATUSDIGITS[5],
	&OBJECTTABLE_STATUSDIGITS[6],
	&OBJECTTABLE_STATUSDIGITS[7],
	&OBJECTTABLE_STATUSDIGITS[8],
	&OBJECTTABLE_STATUSDIGITS[9],
};

void ShowNextLabel(Player* player, int16_t x) {
	if (player->modeFlags & MODE_ITEM) {
		player->nextScale += player->nextScaleV;
		if (player->nextScale < F16(0, 0x00)) {
			player->nextScaleA = F16(0, 0x00);
			player->nextScaleV = F16(0, 0x00);
			player->nextScale = F16(0, 0x00);
		}
		else {
			player->nextScaleV -= player->nextScaleA;
		}

		if (!(player->miscFlags & MISC_ITEMHEADSUP)) {
			player->miscFlags |= MISC_ITEMHEADSUP;
			player->nextScale = F16(0, 0x00);
			player->nextScaleV = F16(8, 0x00);
			player->nextScaleA = F16(1, 0x00);
		}
		else if ((ScreenTime % 32u) == 0u && player->numItemBarBlocks > 17u) {
			player->nextScale = F16(0, 0x00);
			player->nextScaleV = (player->numItemBarBlocks - 17) << 9;
			player->nextScaleA = (player->numItemBarBlocks - 17) << 5;
		}

		int16_t offsetX = -(F16I(player->nextScale) * 18) / (UNSCALED + 1);
		int16_t offsetY = -(F16I(player->nextScale) * 6) / (UNSCALED + 1);
		SpriteScale nextScale = SPRITESCALE(F16I(player->nextScale));
		DisplayObjectEx(OBJECT_NEXTBLOCKLABEL, 26 + offsetY, x + offsetX, PALNUM_OBJECT, 61u, nextScale, nextScale, false);
	}
	else {
		DisplayObject(OBJECT_NEXTBLOCKLABEL, 26, x, PALNUM_OBJECT, 61u);
	}
}

void ShowScoreLabel(Player* player) {
	int16_t x;
	if (player->num == PLAYER1) {
		x = 108;
	}
	else {
		x = 268;
	}

	DisplayObject(OBJECT_SCORELABEL, 130, x, PALNUM_OBJECT, LAYER_GAMESTATUS);
}

void ShowLevelLabel(Player* player, int16_t y, int16_t x) {
	if ((player->modeFlags & MODE_VERSUS) && player->level >= NextSectionLevels[Settings[SETTING_MAXVERSUSSECTION]] - 20) {
		player->levelScale += player->levelScaleV;
		if (player->levelScale < F16(0, 0x00)) {
			player->levelScale = F16(0, 0x00);
			player->levelScaleV = F16(0, 0x00);
		}
		else {
			player->levelScaleV -= F16(0, 0x40);
		}

		if (ScreenTime % 20) {
			player->levelScale = F16(0, 0x00);
			player->levelScaleV = F16(4, 0x00);
		}

		int16_t offsetX = -(F16I(player->levelScale) << 5) / ((UNSCALED + 1) * 2);
		int16_t offsetY = -(F16I(player->levelScale) << 3) / ((UNSCALED + 1) * 2);
		SpriteScale levelScale = SPRITESCALE(F16I(player->levelScale));
		DisplayObjectEx(OBJECT_LEVELLABEL, y + offsetY, x + offsetX, PALNUM_OBJECT, 110, levelScale, levelScale, false);
	}
	else {
		DisplayObject(OBJECT_LEVELLABEL, y, x, PALNUM_OBJECT, LAYER_GAMESTATUS);
	}
}

static const ObjectData* ObjectTableGravityBars[21] = {
	OBJECTPTR(0x154),
	OBJECTPTR(0x184),
	OBJECTPTR(0x185),
	OBJECTPTR(0x186),
	OBJECTPTR(0x187),
	OBJECTPTR(0x188),
	OBJECTPTR(0x189),
	OBJECTPTR(0x18A),
	OBJECTPTR(0x18B),
	OBJECTPTR(0x18C),
	OBJECTPTR(0x18D),
	OBJECTPTR(0x18E),
	OBJECTPTR(0x18F),
	OBJECTPTR(0x190),
	OBJECTPTR(0x191),
	OBJECTPTR(0x192),
	OBJECTPTR(0x193),
	OBJECTPTR(0x194),
	OBJECTPTR(0x195),
	OBJECTPTR(0x196),
	OBJECTPTR(0x154)
};

void ShowLevel(Player* player, int16_t nextSectionLevel, int16_t y, int16_t x, uint8_t palNum) {
	ShowStatusNumEx(player->level, y, x, palNum, LAYER_GAMESTATUS, 3, false, NUMALIGN_RIGHT);
	ShowStatusNumEx(nextSectionLevel, y + 15, x, palNum, LAYER_GAMESTATUS, 3, false, NUMALIGN_RIGHT);

	size_t gravityBarWidth = player->gravity >> 15;
	if (gravityBarWidth > 20u) {
		gravityBarWidth = 20u;
	}
	DisplayObject(ObjectTableGravityBars[gravityBarWidth], y + 11, x, palNum, LAYER_GAMESTATUS);
}

static const ObjectData* ObjectTableGameStatusTimeDigits[10] = {
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[0],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[1],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[2],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[3],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[4],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[5],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[6],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[7],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[8],
	&OBJECTTABLE_GAMESTATUSTIMEDIGITS[9]
};

void ShowGameTime(uint32_t time, int16_t x, uint8_t palNum) {
	uint8_t minutes = time / TIME(1, 0, 0);
	time -= minutes * TIME(1, 0, 0);
	uint8_t seconds = time / TIME(0, 1, 0);
	time -= seconds * TIME(0, 1, 0);
	uint8_t centiseconds = (time * 100u) / TIME(0, 1, 0);

	DisplayObject(ObjectTableGameStatusTimeDigits[minutes / 10u], 215, x, palNum, LAYER_GAMESTATUS);
	DisplayObject(ObjectTableGameStatusTimeDigits[minutes % 10u], 215, x + TIMEDIGIT_WIDTH, palNum, LAYER_GAMESTATUS);

	DisplayObject(OBJECT_TIMECOLON, 215, x + TIMENUM_WIDTH, palNum, LAYER_GAMESTATUS);

	DisplayObject(ObjectTableGameStatusTimeDigits[seconds / 10u], 215, x + TIMENUM_WIDTH + TIMECOLON_WIDTH, palNum, LAYER_GAMESTATUS);
	DisplayObject(ObjectTableGameStatusTimeDigits[seconds % 10u], 215, x + TIMENUM_WIDTH + TIMECOLON_WIDTH + TIMEDIGIT_WIDTH, palNum, LAYER_GAMESTATUS);

	DisplayObject(OBJECT_TIMECOLON, 215, x + TIMENUM_WIDTH + TIMECOLON_WIDTH + TIMENUM_WIDTH, palNum, LAYER_GAMESTATUS);

	DisplayObject(ObjectTableGameStatusTimeDigits[centiseconds / 10u], 215, x + (TIMENUM_WIDTH + TIMECOLON_WIDTH) * 2, palNum, LAYER_GAMESTATUS);
	DisplayObject(ObjectTableGameStatusTimeDigits[centiseconds % 10u], 215, x + (TIMENUM_WIDTH + TIMECOLON_WIDTH) * 2 + TIMEDIGIT_WIDTH, palNum, LAYER_GAMESTATUS);
}

const ObjectData* ObjectTableGrades[NUMPLAYERGRADES] = {
	&OBJECTTABLE_GRADES[PLAYERGRADE_9],
	&OBJECTTABLE_GRADES[PLAYERGRADE_8],
	&OBJECTTABLE_GRADES[PLAYERGRADE_7],
	&OBJECTTABLE_GRADES[PLAYERGRADE_6],
	&OBJECTTABLE_GRADES[PLAYERGRADE_5],
	&OBJECTTABLE_GRADES[PLAYERGRADE_4],
	&OBJECTTABLE_GRADES[PLAYERGRADE_3],
	&OBJECTTABLE_GRADES[PLAYERGRADE_2],
	&OBJECTTABLE_GRADES[PLAYERGRADE_1],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S1],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S2],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S3],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S4],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S5],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S6],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S7],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S8],
	&OBJECTTABLE_GRADES[PLAYERGRADE_S9],
	&OBJECTTABLE_GRADES[PLAYERGRADE_M],
	&OBJECTTABLE_GRADES[PLAYERGRADE_GM],
};

void ShowGrade(Player* player, uint8_t palNum) {
	if (player->gradeScale > UNSCALED) {
		int16_t offset = -((((player->gradeScale - UNSCALED) << 10) / UNSCALED) << 4) >> 10;
		DisplayObjectEx(ObjectTableGrades[player->grade], 35 + offset, player->screenPos[0] + 50 + offset, palNum, 110u, player->gradeScale & 0xFFu, player->gradeScale & 0xFFu, false);
		DisplayObject(ObjectTableGrades[player->gradeBefore], 35, player->screenPos[0] + 50, palNum, LAYER_GAMESTATUS);

		if (CurrentPauseMode < PAUSEMODE_GAME) {
			player->gradeScale -= 4u;
		}
		if (player->gradeScale <= UNSCALED) {
			player->gradeScale = UNSCALED;
			player->gradeBefore = player->grade;
		}
	}
	else {
		DisplayObject(ObjectTableGrades[player->grade], 35, player->screenPos[0] + 50, palNum, LAYER_GAMESTATUS);
		if (player->gradeBefore != player->grade) {
			player->gradeScale = (UNSCALED + 1u) * 2u;
		}
	}
}

void ShowItemBar(Player* player) {
	uint8_t palNum = 13u;
	int16_t x = player->num == PLAYER1 ? 111 : 197;

	if (player->numItemBarBlocks == 20u && ScreenTime % 4u) {
		palNum = 153u;
	}

	int16_t remainingProgress = (player->numItemBarBlocks * 74u) / 20;
	int16_t i = remainingProgress;
	if (i < 0) {
		i = 15;
	}
	i /= 16;
	int16_t y = 62;
	// Display 16-pixel chunks for the bottom of the item bar.
	while (i > 0) {
		DisplayObject(&OBJECTTABLE_ITEMBARCHUNKS[15], y + 80, x, palNum, LAYER_GAMESTATUS);
		y -= 16;
		i--;
		remainingProgress -= 16;
	}
	// Display the chunk matching the remaining progress for the top of the item bar.
	if (remainingProgress > 0) {
		DisplayObject(&OBJECTTABLE_ITEMBARCHUNKS[remainingProgress], y + 80, x, palNum, LAYER_GAMESTATUS);
	}

	DisplayObject(OBJECT_ITEMBARBG, 77, x, 148u, LAYER_GAMESTATUS - 1u);
	DisplayObject(OBJECT_ITEMBARBORDER, 80, x, 12u, LAYER_GAMESTATUS);
}

void ShowVersusRounds(Player* player) {
	const int16_t x = player->num == PLAYER1 ? 111 : 197;
	const int16_t numVersusRounds = NumVersusRoundsSetting();
	for (int16_t versusRound = 0; versusRound < numVersusRounds; versusRound++) {
		DisplayObject(OBJECT_VERSUSROUNDLIGHT, 35 + 14 * versusRound, x, Game.numVersusRoundWins[player->num] > versusRound ? 15u : 14u, LAYER_GAMESTATUS);
	}
}

void ShowVersusWins(bool versus) {
	if (Game.numVersusWins > 0u) {
		int16_t x, y;
		if (versus) {
			if (Game.versusWinner == PLAYER1) {
				x = Players[Game.versusWinner].screenPos[0];
			}
			else {
				x = Players[Game.versusWinner].screenPos[0] - 50;
			}
			y = Players[Game.versusWinner].screenPos[1] + 2;
		}
		else {
			if (Game.versusWinner == PLAYER1) {
				x = 108;
			}
			else {
				x = 268;
			}
			y = 118;
		}
		for (int16_t digits = Game.numVersusWins, base10Place = 10; base10Place != 0; base10Place /= 10, x += VERSUSWINDIGIT_WIDTH) {
			int16_t digit = digits / base10Place;
			digits -= base10Place * digit;
			if (base10Place != 10 || digit != 0) {
				DisplayObject(&OBJECT_VERSUSWINDIGITS[digit], y, x, PALNUM_OBJECT, LAYER_GAMESTATUS);
			}
		}
		DisplayObject(OBJECT_VERSUSWINLABEL, y, x, PALNUM_OBJECT, LAYER_GAMESTATUS);
	}
}

void ShowItemDescription(Player* player) {
	if (ItemDescriptions[player->num] != ITEMTYPE_NULL) {
		DisplayObject(&OBJECTTABLE_ITEMDESCRIPTIONS[ItemDescriptions[player->num]], player->screenPos[1] - 190, player->screenPos[0] + 20, PALNUM_ITEMDESCRIPTION, 61u);
	}

	if (player->nextBlockItemType != ITEMTYPE_NULL) {
		ItemDescriptions[player->num] = player->nextBlockItemType;
	}
}

void ShowOtherItemDescriptions(Item* item) {
	uint8_t numPlayerItems[NUMPLAYERS] = { 0u, 0u };
	ItemCategory itemCats[NUMPLAYERS] = { ITEMCAT_NONE, ITEMCAT_NONE };
	// 0: Player 1 good + player 2 bad
	// 1: Player 1 bad + player 2 good
	// 2: Player 1 neutral
	// 3: Player 2 neutral
	// NOTE: The original code only initializes index 0; standard compilers
	// will zero initialize the whole array, though, fixing a bug below.
	uint8_t itemCatTotals[4] = {0};
	ItemCategory itemCat = GetItemCategory(item->type);
	if (itemCat != ITEMCAT_NONE) {
		numPlayerItems[item->activatingPlayer->num]++;
		itemCats[item->activatingPlayer->num] = itemCat;
		item = item->next;
		for (int16_t itemNum = 0; itemNum < NUMITEMTYPES - 1; itemNum++) {
			if (item == NULL) {
				break;
			}
			Item* curItem = item;
			item = curItem->next;

			itemCat = GetItemCategory(curItem->type);
			if (itemCat != ITEMCAT_NONE) {
				Player* player = curItem->activatingPlayer;
				// Player 1:
				// GOOD: 0
				// BAD: 1
				// NEUTRAL: 2
				//
				// Player 2:
				// GOOD: 1
				// BAD: 0
				// NEUTRAL: 3
				int16_t totalSelect = (itemCat - 1) ^ player->num;
				numPlayerItems[player->num]++;
				itemCats[player->num] = itemCat;
				if (numPlayerItems[PLAYER1] == 1u && numPlayerItems[PLAYER2] == 1u && itemCats[PLAYER1] == itemCats[PLAYER2]) {
					continue;
				}

				if (numPlayerItems[player->num] == 1u && player->activeItemType == curItem->type) {
					continue;
				}

				// BUG: In the original code, indices 1 and up aren't
				// initialized. But the values are never used, so the bug has
				// no effect on behavior.
				itemCatTotals[totalSelect]++;

				// Good & player 1: 1
				// Bad & player 1: 3
				// Neutral & player 1: 5
				//
				// Good & player 2: 2
				// Bad & player 2: 2
				// Neutral & player 2: 6
				int16_t y, x;
				switch (totalSelect + itemCat) {
				// Player 1 bottom left of field.
				case 1:
				case 3:
				case 5:
					y = player->screenPos[1] + 5;
					x = player->screenPos[0] - 60;
					break;

				// Player 2 bottom right of field.
				case 2:
				case 6:
					y = player->screenPos[1] + 5;
					x = player->screenPos[0] + 30;
					break;

				default:
					// BUG: The original didn't handle the default case. This was added just to silence compiler warnings.
					y = player->screenPos[1] + 5;
					x = player->screenPos[0] + 30;
					break;
				}

				if (curItem->type != ITEMTYPE_NULL) {
					DisplayObject(&OBJECTTABLE_ITEMDESCRIPTIONS[TOITEMNUM(curItem->type)], y, x, PALNUM_ITEMDESCRIPTION, LAYER_GAMESTATUS);
				}
			}
		}
	}
}

// TODO: Name the objects used here.
void UNK_600FC50() {
	if (ScreenTime & 0x60) {
		int16_t y, x;
		if (Demo) {
			y = 170;
		}
		else {
			y = 200;
		}
		x = 160;

		const ObjectData* object1 = OBJECTPTR(0x14B);
		const uint16_t layer = 110u;
		if (Settings[SETTING_COINMODE] != COINMODE_FREE) {
			const ObjectData* object0 = OBJECTPTR(0x155);
			if (Settings[SETTING_COINSLOT] == COINSLOT_SAME) {
				if (!CanStart(PLAYER1, false)) {
					if (PayCoins[PLAYER1] != 0 || PayCoins[PLAYER2] != 0) {
						ShowObject(object0, y, x, PALNUM_OBJECT, layer);
					}
					else {
						ShowObject(OBJECTPTR(0x15B), y, x, PALNUM_OBJECT, layer);
					}
				}
				else {
					ShowObject(object1, y, x, PALNUM_OBJECT, layer);
				}
			}
			else {
				if (CanStart(PLAYER1, false)) {
					ShowObject(object1, y, 80, PALNUM_OBJECT, layer);
				}
				else if (PayCoins[PLAYER1] != 0) {
					ShowObject(object0, y, 80, PALNUM_OBJECT, layer);
				}
				else {
					ShowObject(OBJECTPTR(0x15B), y, 80, PALNUM_OBJECT, layer);
				}

				if (CanStart(PLAYER2, false)) {
					ShowObject(object1, y, x, PALNUM_OBJECT, layer);
				}
				else if (PayCoins[PLAYER2] != 0) {
					ShowObject(object0, y, x, PALNUM_OBJECT, layer);
				}
				else {
					ShowObject(OBJECTPTR(0x15B), y, x, PALNUM_OBJECT, layer);
				}
			}
		}
		else {
			ShowObject(object1, y, x, PALNUM_OBJECT, layer);
		}
	}
}

void ShowStartRequirement(Player* player) {
	if ((ScreenTime & 0x60) && CanStart(player->num, false)) {
		const uint16_t layer = 125u;
		if (Settings[SETTING_COINMODE] == COINMODE_FREE) {
			if (player->num == PLAYER1) {
				DisplayObject(OBJECT_PUSH1PSTARTBUTTON, 100, player->screenPos[0], PALNUM_OBJECT, layer);
			}
			else {
				DisplayObject(OBJECT_PUSH2PSTARTBUTTON, 100, player->screenPos[0], PALNUM_OBJECT, layer);
			}
		}
		else {
			uint8_t coins;
			if (Settings[SETTING_COINSLOT] == COINSLOT_SAME) {
				coins = PayCoins[PLAYER1] + PayCoins[PLAYER2];
			}
			else if (player->num == PLAYER1) {
				coins = PayCoins[PLAYER1];
			}
			else {
				coins = PayCoins[PLAYER2];
			}

			if (coins != 0u) {
				DisplayObject(OBJECT_ADDCOIN, 100, player->screenPos[0], PALNUM_OBJECT, layer);
			}
			else {
				DisplayObject(OBJECT_INSERTCOIN, 100, player->screenPos[0], PALNUM_OBJECT, layer);
			}
		}
	}
}

void ShowFree(Started started, int16_t y) {
	const uint16_t layer = 110u;
	if (Settings[SETTING_COINSLOT] == COINSLOT_SAME) {
		ShowObject(OBJECT_FREE, y, 147, PALNUM_OBJECT, layer);
	}
	else {
		if (!(started & STARTED_PLAYER1)) {
			ShowObject(OBJECT_FREE, y, 10, PALNUM_OBJECT, layer);
		}
		if (!(started & STARTED_PLAYER2)) {
			ShowObject(OBJECT_FREE, y, 285, PALNUM_OBJECT, layer);
		}
	}
}

void ShowNumServices(uint8_t numServices, int16_t x, int16_t y) {
	const uint16_t layer = 110u;
	ShowObject(ObjectTableStatusDigits[numServices], y, x, PALNUM_OBJECT, layer);
	ShowObject(OBJECT_SERVICE, y, x, PALNUM_OBJECT, layer);
}

void ShowNumCredits(uint8_t numCredits, int16_t x, int16_t y) {
	const uint16_t layer = 110u;
	ShowObject(ObjectTableStatusDigits[numCredits], y, x, PALNUM_OBJECT, layer);
	ShowObject(OBJECT_CREDIT, y, x + 10, PALNUM_OBJECT, layer);
}

static const ObjectData* ObjectTablePrices[5] = {
	// TODO: Create OBJECT_* definitions for these pointers.
	OBJECTPTR(0x15D),
	OBJECTPTR(0x15F),
	OBJECTPTR(0x161),
	OBJECTPTR(0x163),
	OBJECTPTR(0x165)
};

void ShowNumCoins(uint8_t numCoins, uint8_t price, int16_t x, int16_t y, bool last) {
	const uint16_t layer = 110u;
	ShowObject(ObjectTableStatusDigits[numCoins], y, x, PALNUM_OBJECT, layer);
	ShowObject(ObjectTablePrices[price], y, x, PALNUM_OBJECT, layer);
	if (last) {
		ShowObject(OBJECT_COINCOLON, y, x + 26, PALNUM_OBJECT, layer);
	}
	else {
		ShowObject(OBJECT_COINCOLON, y, x + 27, PALNUM_OBJECT, layer);
	}
}

void ShowCredits() {
	Started started = STARTED_NONE;

	if (Players[PLAYER1].nowFlags & NOW_STARTED) {
		started |= STARTED_PLAYER1;
	}
	if (Players[PLAYER2].nowFlags & NOW_STARTED) {
		started |= STARTED_PLAYER2;
	}

	if (Demo) {
		started = STARTED_NONE;
	}

	if (started != STARTED_ALL) {
		if (Settings[SETTING_COINMODE] == COINMODE_FREE) {
			ShowFree(started, 228);
		}
		else if (Settings[SETTING_COINSLOT] == COINSLOT_SAME) {
			if (*Credits != 0) {
				ShowNumCredits(*Credits, 267, 228);
			}
			else if (PayCoins[PLAYER1] != 0 && PayCoins[PLAYER2] != 0) {
				ShowNumCoins(PayCoins[PLAYER1], (uint8_t)Settings[SETTING_PRICE1P], 218, 228, false);
				ShowNumCoins(PayCoins[PLAYER2], (uint8_t)Settings[SETTING_PRICE2P], 251, 228, true);
			}
			else if (PayCoins[PLAYER1] != 0) {
				ShowNumCoins(PayCoins[PLAYER1], (uint8_t)Settings[SETTING_PRICE1P], 251, 228, true);
			}
			else if (PayCoins[PLAYER2] != 0) {
				ShowNumCoins(PayCoins[PLAYER2], (uint8_t)Settings[SETTING_PRICE2P], 251, 228, true);
			}
		}
		else {
			if (!(started & STARTED_PLAYER1)) {
				if (Credits[PLAYER1] != 0) {
					ShowNumCredits(Credits[PLAYER1], 10, 228);
				}
				else if (PayCoins[PLAYER1] != 0) {
					ShowNumCoins(PayCoins[PLAYER1], (uint8_t)Settings[SETTING_PRICE1P], 10, 228, true);
				}
			}
			if (!(started & STARTED_PLAYER2)) {
				if (Credits[PLAYER2] != 0) {
					ShowNumCredits(Credits[PLAYER2], 267, 228);
				}
				else if (PayCoins[PLAYER2] != 0) {
					ShowNumCoins(PayCoins[PLAYER2], (uint8_t)Settings[SETTING_PRICE2P], 251, 228, true);
				}
			}
			if (Services != 0) {
				ShowNumServices(Services, 135, 228);
			}
		}
	}
}

static const ModeFlag ModeCodeFlagsShow[NUMMODECODES + 1] = {
	MODE_20G,
	MODE_BIG,
	MODE_TLS,
	MODE_NOITEM,
	MODE_CEMENT,
	-1
};

// TODO: Document what &OBJECTTABLE_MODECODES[3] is.
static const ObjectData* ObjectTableModeCodes[NUMMODECODES] = {
	&OBJECTTABLE_MODECODES[0],
	&OBJECTTABLE_MODECODES[1],
	&OBJECTTABLE_MODECODES[2],
	&OBJECTTABLE_MODECODES[4],
	&OBJECTTABLE_MODECODES[5]
};

static const int16_t ModeCodeOffsetX[NUMMODECODES + 1] = { 22, 36, 24, 31, 32, -1 };

void ShowModeCodes(Player* player) {
	int16_t y, x;
	uint16_t layer;
	if (player->modeFlags & MODE_VERSUS) {
		y = 80;
		x = 80 + 80;
		layer = LAYER_GAMESTATUS;
	}
	else {
		if (MainLoop == MAINLOOP_DEMO) {
			y = 180;
			x = FieldPos[(player->num + 2) * 2];
			layer = LAYER_GAMESTATUS;
		}
		else {
			y = 170;
			x = FieldPos[(player->num * 2) + 0];
			layer = 110u;
		}
	}

	if (!(player->modeFlags & MODE_DOUBLES)) {
		ModeFlag modeCodeFlags = player->modeFlags;
		if (!(player->nowFlags & NOW_ABSENT)) {
			modeCodeFlags &= ~MODE_BIG;
		}

		if ((player->modeFlags & MODE_TADEATH) && !(player->nowFlags & NOW_ABSENT)) {
			modeCodeFlags &= ~(MODE_TLS | MODE_BIG | MODE_20G);
		}

		const ObjectData** object = ObjectTableModeCodes;
		const int16_t* offsetX = ModeCodeOffsetX;
		const ModeFlag* flag = ModeCodeFlagsShow;
		for (size_t i = 0u; i < NUMMODECODES; i++, object++, offsetX++, flag++) {
			if (modeCodeFlags & *flag) {
				ShowObject(*object, y, x - *offsetX, PALNUM_OBJECT, layer);
				y += MODECODE_HEIGHT;
			}
		}
	}
}

void ShowChallengerMode(Player* player) {
	int16_t x = FieldPos[(player->num * NUMPLAYERS) + 0];
	#define SHOWMODE(flagMode, textMode) \
	do { \
		if (player->otherPlayer->modeFlags & (flagMode)) { \
			ShowText(x - TextWidth(textMode) / 2, 155, (textMode), 15u, false); \
		} \
	} while (false)
	SHOWMODE(MODE_NORMAL, "NORMAL");
	SHOWMODE(MODE_MASTER, "MASTER");
	SHOWMODE(MODE_TGMPLUS, "TGM+");
	SHOWMODE(MODE_TADEATH, "T.A. DEATH");

	ModeFlag modeCodeFlags = player->otherPlayer->modeFlags;
	if ((player->otherPlayer->modeFlags & MODE_TGMPLUS) && !(player->otherPlayer->nowFlags & NOW_ABSENT)) {
		modeCodeFlags &= ~(MODE_BIG | MODE_ITEM);
	}

	if ((player->otherPlayer->modeFlags & MODE_TADEATH) && !(player->otherPlayer->nowFlags & NOW_ABSENT)) {
		modeCodeFlags &= ~(MODE_20G | MODE_BIG | MODE_ITEM | MODE_TLS);
	}

	const ObjectData** object = ObjectTableModeCodes;
	const int16_t* offsetX = ModeCodeOffsetX;
	const ModeFlag* flag = ModeCodeFlagsShow;
	const uint16_t layer = 110u;
	for (size_t i = 0u; i < NUMMODECODES; i++, object++, offsetX++, flag++) {
		if (modeCodeFlags & *flag) {
			ShowObject(*object, 170, x - *offsetX, PALNUM_OBJECT, layer);
		}
	}
}

void SelectPlayerStatusColor(Player* player, uint8_t* statusPalNums) {
	bool gold = false;
	if (player->gravity >= F32(20, 0x0000) && ScreenTime % 4u) {
		gold = true;
	}

	if (player->gravity >= F32(20, 0x0000) && (player->nowFlags & NOW_STARTED)) {
		gold = true;
	}

	if (gold) {
		// TODO: Palette names.
		statusPalNums[STATUS_GRADE] = 7u;
		statusPalNums[STATUS_TIME] = 8u;
		statusPalNums[STATUS_PROGRESS] = 9u;
	}
	else {
		statusPalNums[STATUS_GRADE] = PALNUM_OBJECT;
		statusPalNums[STATUS_TIME] = PALNUM_OBJECT;
		statusPalNums[STATUS_PROGRESS] = PALNUM_OBJECT;
	}
}

void ShowPlayersStatus() {
	Player* player;

	if (GameFlags & (GAME_TWIN | GAME_VERSUS)) {
		DisplayObjectEx(OBJECT_SINGLENEXTBLOCKBG, 12, Players[PLAYER1].screenPos[0] - 44, PALNUM_NEXTBLOCKBG, 60u, UNSCALED, UNSCALED, true);
		DisplayObjectEx(OBJECT_SINGLENEXTBLOCKBG, 12, Players[PLAYER2].screenPos[0] - 44, PALNUM_NEXTBLOCKBG, 60u, UNSCALED, UNSCALED, true);
	}
	else if (GameFlags & GAME_DOUBLES) {
		DisplayObjectEx(OBJECT_DOUBLESNEXTBLOCKBG, 12, 98, PALNUM_NEXTBLOCKBG, 60u, UNSCALED, UNSCALED, true);
	}

	uint8_t statusPalNums[NUMSTATUSES];
	int16_t levelY = 169;
	if (GameFlags & GAME_DOUBLES) {
		player = Players;
		uint8_t* progressPalNum = &statusPalNums[STATUS_PROGRESS];
		levelY += 13;

		ShowNextLabel(Players, 92);
		ShowGameTime(Players->clearTime, 104, PALNUM_OBJECT);
		for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++, player++) {
			SelectPlayerStatusColor(player, statusPalNums);

			ShowLevelLabel(player, levelY, playerNum == PLAYER1 ? 62 : 226);
			ShowLevel(player, NextSectionLevels[2], levelY, playerNum == PLAYER1 ? 69 : 226, *progressPalNum);
		}
	}
	else if (GameFlags & GAME_TWIN) {
		player = Players;
		uint8_t* gradePalNum = &statusPalNums[STATUS_GRADE];
		uint8_t* timePalNum = &statusPalNums[STATUS_TIME];
		uint8_t* progressPalNum = &statusPalNums[STATUS_PROGRESS];
		for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++, player++) {
			SelectPlayerStatusColor(player, statusPalNums);

			ShowNextLabel(player, playerNum == PLAYER1 ? 12 : 172);
			ShowScoreLabel(player);
			ShowLevelLabel(player, levelY, playerNum == PLAYER1 ? 108 : 268);

			ShowGameTime(player->clearTime, player->screenPos[0] - (player->matrixWidth / 2) * 8, *timePalNum);

			ShowItemDescription(player);

			if (player->modeFlags & MODE_MASTER) {
				ShowGrade(player, *gradePalNum);
			}
			if ((player->modeFlags & MODE_TADEATH) && player->grade >= PLAYERGRADE_M) {
				ShowGrade(player, *gradePalNum);
			}
			if (player->modeFlags & (MODE_MASTER | MODE_TADEATH)) {
				ShowMedals(player);
			}

			ShowStatusNumEx(player->score, 145, player->num == PLAYER1 ? 108 : 268, *progressPalNum, 40u, 6, false, NUMALIGN_CENTER);
			if (player->modeFlags & (MODE_MASTER | MODE_TGMPLUS | MODE_TADEATH)) {
				ShowLevel(player, NextSectionLevels[player->section], levelY, playerNum == PLAYER1 ? 108 : 268, *progressPalNum);
			}
			else {
				ShowLevel(player, NextSectionLevels[2], levelY, playerNum == PLAYER1 ? 108 : 268, *progressPalNum);
			}
		}
		ShowVersusWins(false);
	}
	else if (GameFlags & GAME_VERSUS) {
		player = Players;
		uint8_t* timePalNum = &statusPalNums[STATUS_TIME];
		uint8_t* progressPalNum = &statusPalNums[STATUS_PROGRESS];
		for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++, player++) {
			SelectPlayerStatusColor(player, statusPalNums);
			ShowNextLabel(player, playerNum == PLAYER1 ? 16 : 212);
			if (!(player->modeFlags & (MODE_CEMENT | MODE_NOITEM))) {
				ShowItemBar(player);
			}
			ShowVersusRounds(player);
			ShowLevelLabel(player, 167, playerNum == PLAYER1 ? 111 : 178);

			if (Demo) {
				ShowLevel(player, NextSectionLevels[2], levelY, playerNum == PLAYER1 ? 111 : 185, *progressPalNum);
			}
			else {
				ShowLevel(player, NextSectionLevels[Settings[SETTING_MAXVERSUSSECTION]], levelY, playerNum == PLAYER1 ? 111 : 185, *progressPalNum);
			}

			ShowItemDescription(player);
		}
		ShowVersusWins(true);

		ShowModeCodes(Players);

		if (Players->clearTime < TIME(0, 20, 0)) {
			uint8_t versusTimePalNum;
			if ((ScreenTime & 4u) || Players->clearTime == 0u) {
				// TODO: Name this palette number.
				versusTimePalNum = 156u;
			}
			else {
				versusTimePalNum = *timePalNum;
			}
			ShowGameTime(Players->clearTime, 104, versusTimePalNum);
		}
		else {
			ShowGameTime(Players->clearTime, 104, *timePalNum);
		}
	}
}

void ShowStatusNumEx(uint32_t num, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, int16_t numDigits, bool zeroPad, NumAlign numAlign) {
	int32_t base10Place = 1;
	for (int16_t i = numDigits - 1; i != 0; i--) {
		base10Place *= 10;
	}

	int32_t digits = num;
	bool displayZeroes = zeroPad;
	int16_t digitX = x;
	const ObjectData** zeroDigitObject = &ObjectTableStatusDigits[0];
	for (int16_t i = numDigits; i != 0; i--, base10Place /= 10) {
		int32_t digit = digits / base10Place;
		digits -= digit * base10Place;
		if (digit > 9) {
			digit %= 10;
		}

		const ObjectData* digitObject;
		if (digit == 0) {
			if (displayZeroes || i == 1) {
				digitObject = *zeroDigitObject;
			}
			else {
				digitObject = NULL;
			}
		}
		else {
			digitObject = ObjectTableStatusDigits[digit];
			displayZeroes = true;
		}

		if (digitObject != NULL) {
			DisplayObject(digitObject, y, digitX, palNum, layer);
			digitX += STATUSDIGIT_WIDTH;
		}
		else if (numAlign == NUMALIGN_RIGHT) {
			digitX += STATUSDIGIT_WIDTH;
		}
	}
}

void ShowStatusNum(uint32_t num, int16_t y, int16_t x, int16_t numDigits, bool zeroPad, NumAlign numAlign) {
	int32_t base10Place = 1;
	for (int16_t i = numDigits - 1; i != 0; i--) {
		base10Place *= 10;
	}

	int32_t digits = num;
	bool displayZeroes = zeroPad;
	int16_t digitX = x;
	const ObjectData** zeroDigitObject = &ObjectTableStatusDigits[0];
	for (int16_t i = numDigits; i != 0; i--, base10Place /= 10) {
		int32_t digit = digits / base10Place;
		digits -= digit * base10Place;

		if (digit > 9) {
			digit %= 10;
		}

		const ObjectData* digitObject;
		if (digit == 0) {
			if (displayZeroes || i == 1) {
				digitObject = *zeroDigitObject;
			}
			else {
				digitObject = NULL;
			}
		}
		else {
			digitObject = ObjectTableStatusDigits[digit];
			displayZeroes = true;
		}

		if (digitObject != NULL) {
			ShowObject(digitObject, y, digitX, PALNUM_OBJECT, LAYER_GAMESTATUS);
			digitX += STATUSDIGIT_WIDTH;
		}
		else if (numAlign == NUMALIGN_RIGHT) {
			digitX += STATUSDIGIT_WIDTH;
		}
	}
}

void ShowTimeNumEx(uint32_t num, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, int16_t numDigits, bool zeroPad, NumAlign numAlign) {
	int16_t digitX = x;
	bool displayZeroes = zeroPad;
	int32_t base10Place = 1;
	for (int16_t i = numDigits - 1; i != 0; i--) {
		base10Place *= 10;
	}

	int32_t digits = num;
	const ObjectData** zeroDigitObject = &ObjectTableGameStatusTimeDigits[0];
	for (int16_t i = numDigits; i != 0; i--, base10Place /= 10) {
		int32_t digit = digits / base10Place;
		digits -= digit * base10Place;

		if (digit > 9) {
			digit %= 10;
		}

		const ObjectData* digitObject;
		if (digit == 0) {
			if (displayZeroes || i == 1) {
				digitObject = *zeroDigitObject;
			}
			else {
				digitObject = NULL;
			}
		}
		else {
			digitObject = ObjectTableGameStatusTimeDigits[digit];
		}

		if (digitObject != NULL) {
			ShowObject(digitObject, y, digitX, palNum, layer);
			digitX += TIMEDIGIT_WIDTH;
		}
		else if (numAlign == NUMALIGN_RIGHT) {
			digitX += TIMEDIGIT_WIDTH;
		}
	}
}
