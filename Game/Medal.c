#include "Medal.h"
#include "Player.h"
#include "HwInput.h"
#include "HwSprite.h"
#include "Object.h"
#include "DisplayObject.h"
#include "Video.h"
#include "Pal.h"
#include "Sound.h"
#include "PlatformTypes.h"

typedef enum MedalState {
	MEDALSTATE_NULL,
	MEDALSTATE_SCALINGSTARTED, // TODO: MEDALSTATE_AWARDED?
	MEDALSTATE_SCALING,
	MEDALSTATE_SCALINGFINISHED // TODO: MEDALSTATE_NORMAL?
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
			if (player->medals[medalType] != MEDALCOLOR_NULL) {
				medal->state++;
			}

		case MEDALSTATE_SCALINGSTARTED:
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
				medal->color = player->medals[medalType];
			}
		}
			break;

		case MEDALSTATE_SCALINGFINISHED:
			DisplayObject(ObjectTableMedals[medalType], medal->y, medal->x, MedalPalAnims[medal->color].palNumEnd, 40u);
			break;
		}
	}
}

void _0x6020ECE() {
	NewPalCycle(211u, PALPTR(0xF5), PALPTR(0x20B), 2, PALCYCLETYPE_BOUNCE, 1u, 32u);
	NewPalCycle(213u, PALPTR(0x20A), PALPTR(0x20D), 2, PALCYCLETYPE_BOUNCE, 1u, 32u);
	NewPalCycle(212u, PALPTR(0x208), PALPTR(0x20C), 2, PALCYCLETYPE_BOUNCE, 1u, 32u);
}

void _0x6020F50(uint16_t medals, int16_t y, int16_t x) {
	int32_t medalColorBit = 0;
	for (MedalType medalType = 0u; medalType < NUMMEDALTYPES; medalType++, medalColorBit += 2, x += 48) {
		MedalColor medalColor = (medals >> medalColorBit) & 3;
		DisplayObject(ObjectTableMedals[medalType], y, x, MedalPalAnims[medalColor].palNumCycle, 110u);
	}
}

void UpdateSkillMedal(Player* player);
void UpdateComboMedal(Player* player);
void UpdateSectionTimeMedal(Player* player, uint32_t sectionTime, uint32_t bestSectionTime);
void UpdateAllClearMedal(Player* player);
void UpdateRecoverMedal(Player* player);
void UpdateRotateMedal(Player* player);
