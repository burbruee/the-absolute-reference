#pragma once

#include <stdint.h>
struct Player;

typedef enum MedalType {
	MEDALTYPE_AC,
	MEDALTYPE_ST,
	MEDALTYPE_SK,
	MEDALTYPE_RE,
	MEDALTYPE_RO,
	MEDALTYPE_CO,
	NUMMEDALTYPES
} MedalType;

typedef enum MedalColor {
	MEDALCOLOR_NULL,
	MEDALCOLOR_BRONZE,
	MEDALCOLOR_SILVER,
	MEDALCOLOR_GOLD,
	NUMMEDALCOLORS = MEDALCOLOR_GOLD
} MedalColor;

void InitMedals(struct Player* player, int16_t y, int16_t x);
void ShowMedals(struct Player* player);
void UNK_6020ECE();
void ShowRankingMedals(uint16_t arg0, int16_t y, int16_t x);
void UpdateSkillMedal(struct Player* player);
void UpdateComboMedal(struct Player* player);
void UpdateSectionTimeMedal(struct Player* player, uint32_t sectionTime, uint32_t bestSectionTime);
void UpdateAllClearMedal(struct Player* player);
void UpdateRecoverMedal(struct Player* player);
void UpdateRotateMedal(struct Player* player);
