#include "Medal.h"
#include "Player.h"
#include "HwInput.h"

typedef enum MedalState {
	MEDALSTATE_NULL,
	MEDALSTATE_SCALINGSTARTED,
	MEDALSTATE_SCALING,
	MEDALSTATE_SCALINGFINISHED
} MedalState;

typedef struct Medal {
	MedalState state;
	MedalType type;
	uint8_t scale;
	MedalColor color;
	uint16_t numScaleFrames;
	int16_t y;
	int16_t x;
} Medal;

static const int16_t MedalPosOffsets[NUMMEDALTYPES][2] = {
	{ 0,  0},
	{16,  0},
	{ 0, 16},
	{16, 16},
	{ 0, 32},
	{16, 32}
};

static Medal Medals[NUMPLAYERS][NUMMEDALTYPES];

void InitMedals(Player* player, int16_t medalsY, int16_t medalsX) {
	Medal* medal = Medals[player->num];
	for (int16_t medalType = 0; medalType < NUMMEDALTYPES; medalType++) {
		medal->state = MEDALSTATE_NULL;
		medal->type = medalType;
		medal->scale = 0x00;
		medal->color = MEDALCOLOR_NULL;
		medal->x = medalsX + MedalPosOffsets[medalType][0];
		medal->y = medalsY + MedalPosOffsets[medalType][1];
	}
}

void ShowMedals(Player* player);
void _0x6020ECE();
void _0x6020F50(uint16_t arg0, int16_t y, int16_t x);
void UpdateSkillMedal(Player* player);
void UpdateComboMedal(Player* player);
void UpdateSectionTimeMedal(Player* player, uint32_t sectionTime, uint32_t bestSectionTime);
void UpdateAllClearMedal(Player* player);
void UpdateRecoverMedal(Player* player);
void UpdateRotateMedal(Player* player);
