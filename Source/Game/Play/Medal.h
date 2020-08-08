#pragma once

#include <stdint.h>

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

struct Player;
typedef struct Player Player;

void InitMedals(Player* player, int16_t y, int16_t x);
void ShowMedals(Player* player);
void UNK_6020ECE();
void UNK_6020F50(uint16_t arg0, int16_t y, int16_t x);
void UpdateSkillMedal(Player* player);
void UpdateComboMedal(Player* player);
void UpdateSectionTimeMedal(Player* player, uint32_t sectionTime, uint32_t bestSectionTime);
void UpdateAllClearMedal(Player* player);
void UpdateRecoverMedal(Player* player);
void UpdateRotateMedal(Player* player);
