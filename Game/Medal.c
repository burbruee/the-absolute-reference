#include "Medal.h"
#include "Player.h"
#include "HwInput.h"
#include "HwSprite.h"
#include "Object.h"
#include "DisplayObject.h"
#include "Video.h"
#include "Pal.h"
#include "Sound.h"
#include "Save.h"
#include "PlatformTypes.h"

typedef enum MedalState {
	MEDALSTATE_NULL,
	MEDALSTATE_AWARDED,
	MEDALSTATE_SCALING,
	MEDALSTATE_NORMAL
} MedalState;

typedef struct Medal {
	MedalState state;
	MedalType type;
	int8_t animStep;
	MedalColor color;
	uint16_t scaleFrames;
	int16_t y;
	int16_t x;
} Medal;

typedef struct MedalPalAnim {
	Color* palStart, * palEnd;
	uint8_t palNum, palNumCycle, palNumEnd;
} MedalPalAnim;

static const MedalPalAnim MedalPalAnims[NUMMEDALCOLORS + 1] = {
	{
		PALPTR(0x20C), PALPTR(0x208),
		209u, 151u, 151u
	},
	{
		PALPTR(0x20C), PALPTR(0x208),
		209u, 212u, 150u
	},
	{
		PALPTR(0x20D), PALPTR(0x20A),
		210u, 213u, 152u
	},
	{
		PALPTR(0x20B), PALPTR(0xF5),
		208u, 211u, 11u
	}
};

static const int16_t MedalOffsets[NUMMEDALTYPES][2] = {
	{ 0,  0},
	{16,  0},
	{ 0, 16},
	{16, 16},
	{ 0, 32},
	{16, 32}
};

static Medal Medals[NUMPLAYERS][NUMMEDALTYPES];

static const ObjectData* ObjectTableMedals[NUMMEDALTYPES] = {
	&OBJECTTABLE_MEDALS[MEDALTYPE_AC],
	&OBJECTTABLE_MEDALS[MEDALTYPE_ST],
	&OBJECTTABLE_MEDALS[MEDALTYPE_SK],
	&OBJECTTABLE_MEDALS[MEDALTYPE_RE],
	&OBJECTTABLE_MEDALS[MEDALTYPE_RO],
	&OBJECTTABLE_MEDALS[MEDALTYPE_CO],
};

void InitMedals(Player* player, int16_t medalsY, int16_t medalsX) {
	Medal* medal = Medals[player->num];
	for (int16_t medalType = 0; medalType < NUMMEDALTYPES; medalType++) {
		medal->state = MEDALSTATE_NULL;
		medal->type = medalType;
		medal->animStep = 0;
		medal->color = MEDALCOLOR_NULL;
		medal->x = medalsX + MedalOffsets[medalType][0];
		medal->y = medalsY + MedalOffsets[medalType][1];
	}
}

void ShowMedals(Player* player) {
	Medal* medal = Medals;
	for (MedalType medalType = 0u; medalType < NUMMEDALTYPES; medalType++, medal++) {
		switch (medal->state) {
		case MEDALSTATE_NULL:
			if (player->medalColors[medalType] != MEDALCOLOR_NULL) {
				medal->state++;
			}

		case MEDALSTATE_AWARDED:
			medal->state++;
			medal->scaleFrames = 64u;
			medal->animStep = 64;
			PlaySoundEffect(SOUNDEFFECT_MEDALUP);
			NewPalCycle(MedalPalAnims[medal->color].palNum, MedalPalAnims[medal->color].palStart, MedalPalAnims[medal->color].palEnd, 1, PALCYCLETYPE_UPSTOP, 1u, 63u);

		case MEDALSTATE_SCALING: {
			int16_t medalOffset = medal->animStep >> 3;
			SpriteScale medalScale = SPRITESCALE(medal->animStep);
			DisplayObjectEx(ObjectTableMedals[medalType], medal->y - medalOffset, medal->x - medalOffset, MedalPalAnims[medal->color].palNum, 40u, medalScale, medalScale, false);
			if (medal->animStep != 0 && (medal->animStep -= 3) < 0) {
				medal->animStep = 0;
			}
			if (--medal->scaleFrames == 0u) {
				medal->state++;
				medal->color = player->medalColors[medalType];
			}
		}
			break;

		case MEDALSTATE_NORMAL:
			DisplayObject(ObjectTableMedals[medalType], medal->y, medal->x, MedalPalAnims[medal->color].palNumEnd, 40u);
			break;
		}
	}
}

void _0x6020ECE() {
	NewPalCycle(211u, PALPTR(0xF5), PALPTR(0x20B), 2, PALCYCLETYPE_BOUNCE, 1u, 32u);  // Gold.
	NewPalCycle(213u, PALPTR(0x20A), PALPTR(0x20D), 2, PALCYCLETYPE_BOUNCE, 1u, 32u); // Silver.
	NewPalCycle(212u, PALPTR(0x208), PALPTR(0x20C), 2, PALCYCLETYPE_BOUNCE, 1u, 32u); // Bronze.
}

void _0x6020F50(uint16_t medals, int16_t y, int16_t x) {
	int32_t medalColorBit = 0;
	for (MedalType medalType = 0u; medalType < NUMMEDALTYPES; medalType++, medalColorBit += 2, x += 48) {
		MedalColor medalColor = (medals >> medalColorBit) & 3;
		DisplayObject(ObjectTableMedals[medalType], y, x, MedalPalAnims[medalColor].palNumCycle, 110u);
	}
}

static const uint8_t RequiredSkillMedalNormalSkillClears[NUMMEDALCOLORS] = { 10u, 20u, 35u };
static const uint8_t RequiredSkillMedalBigSkillClears[NUMMEDALCOLORS] = { 1u, 2u, 4u };
void UpdateSkillMedal(Player* player) {
	player->numSkillClears++;
	if (player->medalColors[MEDALTYPE_SK] < NUMMEDALCOLORS) {
		uint8_t requiredSkillClears;
		if (player->modeFlags & MODE_BIG) {
			requiredSkillClears = RequiredSkillMedalBigSkillClears[player->medalColors[MEDALTYPE_SK]];
		}
		else if (player->modeFlags & MODE_TADEATH) {
			requiredSkillClears = RequiredSkillMedalNormalSkillClears[player->medalColors[MEDALTYPE_SK]] / 2;
		}
		else {
			requiredSkillClears = RequiredSkillMedalNormalSkillClears[player->medalColors[MEDALTYPE_SK]];
		}
		if (player->numSkillClears >= requiredSkillClears) {
			player->medalColors[MEDALTYPE_SK]++;
			Medals[player->num][MEDALTYPE_SK].color = player->medalColors[MEDALTYPE_SK];
			Medals[player->num][MEDALTYPE_SK].state = MEDALSTATE_AWARDED;
		}
	}
}

static const uint8_t RequiredComboMedalNormalComboClears[NUMMEDALCOLORS] = { 4u, 5u, 7u };
static const uint8_t RequiredComboMedalBigComboClears[NUMMEDALCOLORS] = { 2u, 3u, 4u };
void UpdateComboMedal(Player* player) {
	MedalColor awardedMedalColor;

	for (awardedMedalColor = MEDALCOLOR_NULL; awardedMedalColor < NUMMEDALCOLORS; awardedMedalColor++) {
		if (player->modeFlags & MODE_BIG) {
			if (RequiredComboMedalBigComboClears[awardedMedalColor] == player->numComboClears) {
				break;
			}
		}
		else {
			if (RequiredComboMedalNormalComboClears[awardedMedalColor] == player->numComboClears) {
				break;
			}
		}
	}
	if (awardedMedalColor < NUMMEDALCOLORS) {
		awardedMedalColor++;
		Medals[player->num][MEDALTYPE_CO].color = awardedMedalColor;
		Medals[player->num][MEDALTYPE_CO].state = MEDALSTATE_AWARDED;
		if (player->medalColors[MEDALTYPE_CO] < awardedMedalColor) {
			player->medalColors[MEDALTYPE_CO] = awardedMedalColor;
		}
	}
}

void UpdateSectionTimeMedal(Player* player, uint32_t sectionTime, uint32_t bestSectionTime) {
	MedalColor awardedMedalColor = MEDALCOLOR_NULL;
	for (
		uint32_t currentSectionTime = bestSectionTime + TIME(0, 10, 0);
		currentSectionTime <= sectionTime;
		awardedMedalColor++, currentSectionTime += TIME(0, 5, 0)
	);
	if (awardedMedalColor != MEDALCOLOR_NULL) {
		Medals[player->num][MEDALTYPE_ST].color = awardedMedalColor;
		Medals[player->num][MEDALTYPE_ST].state = MEDALSTATE_AWARDED;
		if (player->medalColors[MEDALTYPE_ST] < awardedMedalColor) {
			player->medalColors[MEDALTYPE_ST] = awardedMedalColor;
		}
		if (awardedMedalColor == NUMMEDALCOLORS) {
			if (player->modeFlags == MODE_MASTER) {
				BestMasterSectionTimes[player->section] = sectionTime;
			}
			if (player->modeFlags == MODE_TADEATH) {
				BestTaDeathSectionTimes[player->section] = sectionTime;
			}
		}
	}
}

void UpdateAllClearMedal(Player* player) {
	if (player->medalColors[MEDALTYPE_AC] < NUMMEDALCOLORS) {
		player->medalColors[MEDALTYPE_AC]++;
		Medals[player->num][MEDALTYPE_AC].color = player->medalColors[MEDALTYPE_AC];
		Medals[player->num][MEDALTYPE_AC].state = MEDALSTATE_AWARDED;
	}
}

static const uint8_t RequiredRecoveryMedalRecoveries[NUMMEDALCOLORS] = { 1u, 2u, 4u };
void UpdateRecoverMedal(Player* player) {
	if (player->medalColors[MEDALTYPE_RE] < NUMMEDALCOLORS) {
		if (player->miscFlags & MISC_RECOVERING && player->numFieldBlocks <= 70u) {
			player->numRecoveries++;
			player->miscFlags &= ~MISC_RECOVERING;
			if (player->numRecoveries >= RequiredRecoveryMedalRecoveries[player->medalColors[MEDALTYPE_RE]]) {
				player->medalColors[MEDALTYPE_RE]++;
				Medals[player->num][MEDALTYPE_RE].color = player->medalColors[MEDALTYPE_RE];
				Medals[player->num][MEDALTYPE_RE].state = MEDALSTATE_AWARDED;
			}
		}
		else if (player->numFieldBlocks >= 150u) {
			player->miscFlags |= MISC_RECOVERING;
		}
	}
}

static const uint16_t RequiredRotateMedalLevel[NUMMEDALCOLORS] = { 300u, 700u, 999u };
static const uint8_t RequiredRotateMedalRotationVs[NUMMEDALCOLORS] = { 12u, 12u, 12u };
void UpdateRotateMedal(Player* player) {
	if (player->medalColors[MEDALTYPE_RO] < NUMMEDALCOLORS) {
		if (player->numActiveRotations > 4u) {
			player->numActiveRotations = 4u;
		}
		if (player->numRotations < 3000u) {
			player->numRotations += player->numActiveRotations;
		}
		player->numRotateBlocks++;
		if (player->level >= RequiredRotateMedalLevel[player->nextRotateMedalColor]) {
			uint32_t rotationsV = (player->numRotations * 10u) / player->numRotateBlocks;
			if (rotationsV >= RequiredRotateMedalRotationVs[player->nextRotateMedalColor++]) {
				player->medalColors[MEDALTYPE_RO] = player->nextRotateMedalColor;
				Medals[player->num][MEDALTYPE_RO].color = player->medalColors[MEDALTYPE_RO];
				Medals[player->num][MEDALTYPE_RO].state = MEDALSTATE_AWARDED;
			}
			player->numRotateBlocks = 0u;
			player->numRotations = 0u;
		}
	}
	player->numActiveRotations = 0u;
}

