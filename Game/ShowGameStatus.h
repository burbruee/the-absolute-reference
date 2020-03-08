#pragma once

// TODO: Consider renaming ShowGameStatus.* to ShowPlayersStatus.*; if only ShowPlayersStatus is used outside of ShowGameStatus.c, then for sure rename.

#include "Player.h"
#include "ShowText.h"
#include "DisplayObject.h"
#include <stdint.h>
#include <stdbool.h>

void ShowNextLabel(Player* player, int16_t x);
void ShowScoreLabel(Player* player);
void ShowLevelLabel(Player* player, int16_t y, int16_t x);

void ShowLevel(Player* player, int16_t nextSectionLevel, int16_t y, int16_t x, uint8_t palNum);

void ShowGameTime(uint32_t time, int16_t x, uint8_t palNum);

extern const ObjectData* ObjectTableGrades[NUMPLAYERGRADES];
void ShowGrade(Player* player, uint8_t palNum);

void ShowItemBar(Player* player);
void ShowVersusRounds(Player* player);
void ShowVersusWins(bool versus);
void ShowItemDescription(Player* player);
// NOTE: It appears this is an unnecessary relic left in. Due to the manner in
// which items are handled, this function never shows anything, because there's
// never three or more items simultaneously active. The call of this function
// can be safely removed.
void ShowOtherItemDescriptions(Item* item);

typedef enum Started {
	STARTED_NONE = 0,
	STARTED_PLAYER1 = 1 << PLAYER1,
	STARTED_PLAYER2 = 1 << PLAYER2
} Started;
#define STARTED_ALL (STARTED_PLAYER1 | STARTED_PLAYER2)
void _0x600FC50();
void ShowStartRequirement(Player* player);
void ShowFree(Started started, int16_t y);
void ShowNumServices(uint8_t numServices, int16_t x, int16_t y);
void ShowNumCredits(uint8_t numCredits, int16_t x, int16_t y);
void ShowNumCoins(uint8_t numCoins, uint8_t price, int16_t x, int16_t y, bool last);
void ShowCredits();

void ShowModeCodes(Player* player);
void ShowChallengerMode(Player* player);

typedef enum Status {
	STATUS_GRADE,
	STATUS_TIME,
	STATUS_PROGRESS,
	NUMSTATUSES
} Status;
void SelectPlayerStatusColor(Player* player, uint8_t* statusPalNums);
void ShowPlayersStatus();

const ObjectData* ObjectTableStatusDigits[10];
void ShowStatusNumEx(uint32_t num, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, int16_t numDigits, bool zeroPad, NumAlign numAlign);
void ShowStatusNum(uint32_t num, int16_t y, int16_t x, int16_t numDigits, bool zeroPad, NumAlign numAlign);

void ShowTimeNumEx(uint32_t num, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, int16_t numDigits, bool zeroPad, NumAlign numAlign);
