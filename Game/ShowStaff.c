#include "ShowStaff.h"
#include "ShowEffect.h"
#include "ShowText.h"
#include "Frame.h"
#include "Entity.h"
#include "Object.h"
#include "Video.h"
#include "Pal.h"
#include "Sound.h"
#include "Button.h"
#include "Math.h"
#include "Macros.h"
#include "LibC.h"
#include <string.h>

static void UpdateEntityStaff(Entity* entity);
static void ShowGrandMasterCongratulations(Player* player);
static void UpdateEntityGrandMasterCongratulations(Entity* entity);
static void ShowRetryForGrandMaster(Player* player);
static void UpdateEntityRetryForGrandMaster(Entity* entity);
static void UpdateEntityNormalComplete(Entity* entity);
static void UpdateEntityTgmPlusComplete(Entity* entity);
static void UpdateEntityDoublesComplete(Entity* entity);
static void UpdateStaffFireworks(Entity* entity, Player* player, uint16_t delay);
static void UpdateEntityDeathComplete(Entity* entity);
static void UpdateEntityDeathIncomplete(Entity* entity);

static uint16_t StaffFireworkFrames[NUMPLAYERS];

#define NUMSTAFFOBJECTS 38
static ObjectData* ObjectTableStaff[NUMPLAYERS][NUMSTAFFOBJECTS];

enum StaffSection {
	STAFF_BGDESIGN = 1,
	STAFF_EFFECTDESIGN,
	STAFF_VISUALDESIGN,
	STAFF_PROGRAMMER,
	STAFF_SOUND,
	NUMSTAFFSECTIONS = STAFF_SOUND
};

#define NUMSTAFFGMLINES 32u

static const ObjectData* ObjectTableBgDesignStaff[13];
static const ObjectData* ObjectTableEffectDesignStaff[2];
static const ObjectData* ObjectTableVisualDesignStaff[2];
static const ObjectData* ObjectTableProgrammerStaff[4];
static const ObjectData* ObjectTableSoundStaff[3];
static const ObjectData* ObjectTableEndStaff[14];
static const int8_t StaffSections[6];
static const int16_t EndStaffRows[13];

typedef enum ScrollRate {
	SCROLLRATE_NORMAL = 1,
	SCROLLRATE_FAST = 3
} ScrollRate;

typedef struct StaffData {
	ScrollRate scrollRate;
	int16_t scrollDivisor;
	int16_t pixelX[NUMSTAFFOBJECTS];
	int16_t pixelY[NUMSTAFFOBJECTS];
	int16_t bottomPixelY;
	int16_t topPixelY;
	int16_t exitPixelY;
	bool skip;
} StaffData;

#define scrollY values[0]
#define scrollPixels values[2]
#define fireworksCycleFrames values[3]

void ShowStaff(Player* player) {
	ObjectData* objectTableBgDesignStaff[lengthof(ObjectTableBgDesignStaff)];
	ObjectData* objectTableEffectDesignStaff[lengthof(ObjectTableEffectDesignStaff)];
	ObjectData* objectTableVisualDesignStaff[lengthof(ObjectTableVisualDesignStaff)];
	ObjectData* objectTableProgrammerStaff[lengthof(ObjectTableProgrammerStaff)];
	ObjectData* objectTableSoundStaff[lengthof(ObjectTableSoundStaff)];
	ObjectData* objectTableEndStaff[lengthof(ObjectTableEndStaff)];
	int8_t staffSections[lengthof(StaffSections)];
	int16_t endStaffRows[lengthof(EndStaffRows)];
	memcpy(objectTableBgDesignStaff, ObjectTableBgDesignStaff, sizeof(ObjectTableBgDesignStaff));
	memcpy(objectTableEffectDesignStaff, ObjectTableEffectDesignStaff, sizeof(ObjectTableEffectDesignStaff));
	memcpy(objectTableVisualDesignStaff, ObjectTableVisualDesignStaff, sizeof(ObjectTableVisualDesignStaff));
	memcpy(objectTableProgrammerStaff, ObjectTableProgrammerStaff, sizeof(ObjectTableProgrammerStaff));
	memcpy(objectTableSoundStaff, ObjectTableSoundStaff, sizeof(ObjectTableSoundStaff));
	memcpy(objectTableEndStaff, ObjectTableEndStaff, sizeof(ObjectTableEndStaff));
	memcpy(staffSections, StaffSections, sizeof(StaffSections));
	memcpy(endStaffRows, EndStaffRows, sizeof(EndStaffRows));

	if ((player->modeFlags & MODE_DOUBLES) || ((player->modeFlags & (MODE_NORMAL | MODE_TGMPLUS | MODE_TADEATH)) && (GameFlags & GAME_TWIN))) {
		PlaySoundEffect(SOUNDEFFECT_GRANDMASTER);
	}

	for (int16_t swapTries = 0; swapTries < NUMSTAFFOBJECTS * 2; swapTries++) {
		int16_t i = NUMSTAFFOBJECTS % Rand(64u);
		int16_t j = NUMSTAFFOBJECTS % Rand(64u);
		if (i != j) {
			int8_t tempSection = staffSections[i];
			staffSections[i] = staffSections[j];
			staffSections[j] = tempSection;
		}
	}

	int16_t staffObjectIndex = 1;
	int16_t staffRows[NUMSTAFFOBJECTS] = { 1 };
	int16_t staffRow;
	#define SET_STAFF_OBJECTS(section, objectTable) \
		case section: \
			for (int16_t j = 0; j < lengthof(objectTable); j++) { \
				staffRows[staffObjectIndex] = staffRow; \
				staffRow += 2; \
				ObjectTableStaff[player->num][staffObjectIndex] = objectTable[j]; \
			} \
			staffRow += 3; \
			break
	for (int16_t i = 0; i < NUMSTAFFSECTIONS; i++) {
		switch (staffSections[i]) {
		SET_STAFF_OBJECTS(STAFF_BGDESIGN, objectTableBgDesignStaff);
		SET_STAFF_OBJECTS(STAFF_EFFECTDESIGN, objectTableEffectDesignStaff);
		SET_STAFF_OBJECTS(STAFF_VISUALDESIGN, objectTableVisualDesignStaff);
		SET_STAFF_OBJECTS(STAFF_PROGRAMMER, objectTableProgrammerStaff);
		SET_STAFF_OBJECTS(STAFF_SOUND, objectTableSoundStaff);

		default:
			break;
		}
	}
	#undef SET_STAFF_OBJECTS

	for (int16_t i = 0; i < lengthof(objectTableEndStaff); i++, staffObjectIndex++) {
		staffRows[staffObjectIndex] = endStaffRows[i] + staffRow;
		ObjectTableStaff[player->num][staffObjectIndex] = objectTableEndStaff[i + 1];
	}

	Entity* staffEntity;
	if (staffEntity = AllocEntity()) {
		staffEntity->update = UpdateEntityStaff;
		staffEntity->data.info.player = player;
		StaffFireworkFrames[player->num] = 15u;
		staffEntity->scrollY = 0;
		staffEntity->values[1] = 0;
		staffEntity->scrollPixels = VIDEO_HEIGHT;
		staffEntity->fireworksCycleFrames = 1;
		ENTITY_INST_DATA_PTR(StaffData, data, staffEntity);
		data->skip = false;
		data->bottomPixelY = player->screenPos[1] + (player->matrixHeight - 1) * 8 - (player->matrixHeight - 1) * 8 - 6;
		data->topPixelY = player->screenPos[1] + (player->matrixHeight - MATRIX_HEIGHT) * 8 - (player->matrixHeight - 1) * 8 - 6;
		data->exitPixelY = player->screenPos[1] + (player->matrixHeight - MATRIX_HEIGHT + 8) * 8 - (player->matrixHeight - 1) * 8 - 6;
		for (int16_t i = 0; i < NUMSTAFFOBJECTS; i++) {
			if (player->modeFlags & MODE_DOUBLES) {
				data->pixelX[i] = player->screenPos[0] - (player->matrixWidth / 2) * 8 + 24;
			}
			else {
				data->pixelX[i] = player->screenPos[0] - (player->matrixWidth / 2) * 8 + 8;
			}
			data->pixelY[i] = player->screenPos[1] + (staffRows[i] + player->matrixHeight - 1) * 8 - (player->matrixHeight - 1) * 8 - 6;
		}
		SetPal(178u, 1u, PAL_STAFF);
		SetPal(179u, 1u, PAL_GRANDMASTER);
	}
}

static uint16_t _0x6035670[NUMSTAFFOBJECTS];
void _0x6021E64() {
	uint16_t var0[NUMSTAFFOBJECTS];
	MemCopy(sizeof(var0), var0, _0x6035670);
}

static void UpdateEntityStaff(Entity* entity) {
	Player* player = entity->data.info.player;

	if ((GameFlags & GAME_TWIN) && GameButtonsDown[player->num] == (BUTTON_3 | BUTTON_2 | BUTTON_1)) {
		player->nowFlags |= NOW_SHOWRANKINGCODE;
	}
	if (player->modeFlags & (MODE_20G | MODE_BIG | MODE_ITEM | MODE_TLS)) {
		player->nowFlags &= ~NOW_SHOWRANKINGCODE;
	}

	ENTITY_INST_DATA_PTR(StaffData, data, entity);
	if (!(player->modeFlags & (MODE_NORMAL | MODE_TGMPLUS | MODE_TADEATH)) || !(GameButtonsDown[player->num] & BUTTON_START)) {
		if ((player->modeFlags & MODE_DOUBLES) && ((GameButtonsDown[PLAYER1] & BUTTON_START) || (GameButtonsDown[PLAYER2] & BUTTON_START))) {
			data->scrollRate = SCROLLRATE_FAST;
			data->scrollDivisor = 1;
		}
		else {
			int16_t divisor;
			if (player->modeFlags & MODE_MASTER) {
				data->scrollRate = SCROLLRATE_NORMAL;
				divisor = 4;
			}
			else {
				data->scrollRate = SCROLLRATE_NORMAL;
				divisor = 2;
			}
			data->scrollDivisor = divisor;
		}
	}
	else {
		data->scrollRate = SCROLLRATE_FAST;
		data->scrollDivisor = 1u;
	}

	for (int16_t i = 0; i < NUMSTAFFOBJECTS; i++) {
		if (
			data->pixelY[i] > data->topPixelY && !(entity->scrollY % data->scrollDivisor) &&
			(
				i < NUMSTAFFOBJECTS - 2 ||
				(i == NUMSTAFFOBJECTS - 2 && data->pixelY[i] > data->exitPixelY) ||
				(i == NUMSTAFFOBJECTS - 1 && data->pixelY[i] > data->exitPixelY + 16)
				)
			) {
			data->pixelY[i] -= data->scrollRate;
		}

		if (entity->scrollPixels > 0 && !data->skip && data->pixelY[i] > data->topPixelY && data->pixelY[i] < data->bottomPixelY) {
			if (data->pixelY[i] > data->bottomPixelY - 8) {
				DisplayObjectEx(
					ObjectTableStaff[i],
					data->pixelY[i] + player->screenOffset[1],
					data->pixelX + player->screenOffset[0],
					178u,
					70u,
					(data->pixelY[i] - data->bottomPixelY + 8) * -5 * UNSCALED,
					UNSCALED,
					false
				);
			}
			else if (data->pixelY[i] < data->topPixelY + 8) {
				DisplayObjectEx(
					ObjectTableStaff[i],
					data->pixelY[i] + player->screenOffset[1],
					data->pixelX + player->screenOffset[0],
					178u,
					70u,
					(data->pixelY[i] - data->topPixelY) * 5 + 0x17,
					UNSCALED,
					false
				);
			}
			else {
				DisplayObjectEx(
					ObjectTableStaff[i],
					data->pixelY[i] + player->screenOffset[1],
					data->pixelX + player->screenOffset[0],
					178u,
					70u,
					UNSCALED,
					UNSCALED,
					false
				);
			}
		}
	}

	if (player->modeFlags & MODE_MASTER) {
		if (entity->scrollY > 3700) {
			data->skip = true;
			player->miscFlags |= MISC_ORANGELINE;
			player->nowFlags |= NOW_NOUPDATE;
			if ((player->mGradeFlags & MGRADE_QUALIFIED) == MGRADE_QUALIFIED && !(player->modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS | MODE_INVISIBLE))) {
				// Qualified for grade M in normal, master, and doubles.
				// Invisible mode might be enabled. Also qualified in versus.
				// No other mode settings allowed (such as item mode).
				if (player->numMGradeLines >= NUMSTAFFGMLINES) {
					player->grade = PLAYERGRADE_GM;
					PlaySoundEffect(SOUNDEFFECT_LEVELUP);
					ShowGrandMasterCongratulations(player);
				}
				else {
					player->grade = PLAYERGRADE_GM;
					PlaySoundEffect(SOUNDEFFECT_LEVELUP);
					ShowGrandMasterCongratulations(player);
				}
			}
			else {
				ShowRetryForGrandMaster(player);
			}
			LockActiveBlock(player, LOCKTYPE_GAMEOVER);

			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					MATRIX(player, row, col).block &= ~(BLOCK_FADING | BLOCK_INVISIBLE);
				}
			}
			FreeEntity(entity);
		}
		else if (player->nowFlags & NOW_STOPPED) {
			if ((player->mGradeFlags & MGRADE_QUALIFIED) == MGRADE_QUALIFIED && !(player->modeFlags & ~(MODE_NORMAL | MODE_MASTER | MODE_DOUBLES | MODE_VERSUS | MODE_INVISIBLE))) {
				PlaySoundEffect(SOUNDEFFECT_LEVELUP);
				player->grade = PLAYERGRADE_M;
			}
			FreeEntity(entity);
		}
	}
	else if (entity->scrollY >= 2220) {
		player->nowFlags &= NOW_NOUPDATE;
		if (!(player->nowFlags & NOW_STOPPED)) {
			player->miscFlags |= MISC_ORANGELINE;
			LockActiveBlock(player, LOCKTYPE_GAMEOVER);
			if (GameFlags & GAME_DOUBLES) {
				LockActiveBlock(player->otherPlayer, LOCKTYPE_GAMEOVER);
			}
		}
		NextPlayGameOver(player);
		FreeEntity(entity);
	}
	else if (entity->scrollPixels <= 0) {
		data->skip = true;
		if (player->modeFlags & MODE_TADEATH) {
			if (player->level >= 999u) {
				UpdateEntityDeathComplete(entity);
			}
			else {
				UpdateEntityDeathIncomplete(entity);
			}
		}
		else if (player->modeFlags & MODE_TGMPLUS) {
			UpdateEntityTgmPlusComplete(entity);
		}
		else if (player->modeFlags & MODE_NORMAL) {
			UpdateEntityNormalComplete(entity);
		}
		else {
			UpdateEntityDoublesComplete(entity);
		}
	}

	if (CurrentPauseMode < PAUSEMODE_GAME) {
		entity->scrollY += data->scrollRate == SCROLLRATE_FAST ? data->scrollRate * 2u : data->scrollRate;

		if (data->pixelY[NUMSTAFFOBJECTS - 2] <= data->exitPixelY) {
			entity->scrollPixels -= data->scrollRate == SCROLLRATE_FAST ? data->scrollRate * 2 : data->scrollRate;
		}
	}
}

#undef scrollY
#undef scrollPixels

#define frames values[0]
#define scale values[0]

static void ShowGrandMasterCongratulations(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityGrandMasterCongratulations;
		entity->frames = 600;
		entity->values[1] = 1;
		entity->data.info.player = player;
		player->scale = 123;
	}
}

static void UpdateEntityGrandMasterCongratulations(Entity* entity) {
	Player* player = entity->data.info.player;

	if ((GameFlags & GAME_TWIN) && GameButtonsDown[player->num] == (BUTTON_3 | BUTTON_2 | BUTTON_1)) {
		player->nowFlags |= NOW_SHOWRANKINGCODE;
	}
	if (player->modeFlags & (MODE_20G | MODE_BIG | MODE_ITEM | MODE_TLS)) {
		player->nowFlags &= NOW_SHOWRANKINGCODE;
	}

	player->nowFlags |= NOW_NOUPDATE;

	if (player->scale > UNSCALED) {
		player->scale -= 0x02;
	}
	else {
		player->scale = UNSCALED;
	}

	int32_t scalePixelOffset = ((player->scale - UNSCALED) << 10) / UNSCALED;
	DisplayObjectEx(
		OBJECT_GRANDMASTERCONGRATULATIONS,
		120 + (scalePixelOffset * -12) >> 10,
		player->screenPos[0] + (scalePixelOffset * -40) >> 10,
		179u,
		125u,
		player->scale,
		player->scale,
		false
	);

	if (CurrentPauseMode < PAUSEMODE_GAME) {
		UpdateStaffFireworks(entity, player, 15u);
		if (--entity->frames == 0) {
			player->nowFlags &= ~NOW_NOUPDATE;
			NextPlayGameOver(player);
			FreeEntity(entity);
		}
	}
}

#undef scale

static void ShowRetryForGrandMaster(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityRetryForGrandMaster;
		entity->frames = 600;
		entity->values[1] = 1;
		entity->data.info.player = player;
	}
}

static void UpdateEntityRetryForGrandMaster(Entity* entity) {
	Player* player = entity->data.info.player;

	if ((GameFlags & GAME_TWIN) && GameButtonsDown[player->num] == (BUTTON_3 | BUTTON_2 | BUTTON_1)) {
		player->nowFlags |= NOW_SHOWRANKINGCODE;
	}
	if (player->modeFlags & (MODE_20G | MODE_BIG | MODE_ITEM | MODE_TLS)) {
		player->nowFlags |= ~NOW_SHOWRANKINGCODE;
	}

	DisplayObject(OBJECTPTR(0x2FB), 100, player->screenPos[0], 0u, 125u);
	DisplayObject(OBJECT_RETRYFORGRANDMASTER, 140, player->screenPos[0], 0u, 125u);

	if (CurrentPauseMode < PAUSEMODE_GAME && --entity->frames == 0) {
		player->nowFlags &= ~NOW_NOUPDATE;
		NextPlayGameOver(player);
		FreeEntity(entity);
	}
}

static void UpdateEntityNormalComplete(Entity* entity) {
	Player* player = entity->data.info.player;

	player->nowFlags |= NOW_NOUPDATE;


	UpdateStaffFireworks(entity, player, 15u);

	DisplayObject(OBJECTPTR(0x2F9), 120, player->screenPos[0], 0u, 125u);
}

static void UpdateEntityTgmPlusComplete(Entity* entity) {
	Player* player = entity->data.info.player;

	player->nowFlags |= NOW_NOUPDATE;

	UpdateStaffFireworks(entity, player, 15u);

	ShowText(player->screenPos[0] - TextWidth("EXCELLENT!") / 2, 120, "EXCELLENT!", 15u, false);
}

static void UpdateEntityDoublesComplete(Entity* entity) {
	Player* player = entity->data.info.player;

	player->nowFlags |= NOW_NOUPDATE;
	player->otherPlayer->nowFlags |= NOW_NOUPDATE;

	UpdateStaffFireworks(entity, player, 15u);

	DisplayObject(OBJECTPTR(0x2FD), 120, player->screenPos[0], 0u, 125u);
}

static void UpdateStaffFireworks(Entity* entity, Player* player, uint16_t delay) {
	if (GameButtonsNew[player->num] & ~BUTTON_START) {
		entity->fireworksCycleFrames++;
	}

	if (StaffFireworkFrames[player->num]-- == 0 || entity->fireworksCycleFrames % 10 == 0) {
		StaffFireworkFrames[player->num] = delay;
		entity->fireworksCycleFrames = 1;
		uint32_t seed = Rand(10u);
		int16_t col = Rand(10u);
		int16_t row = Rand(5u) + 13u;
		ShowFireworks(player, row, col, seed);
		PlaySoundEffect(SOUNDEFFECT_SHOTGUN);
	}
}

static void UpdateEntityDeathComplete(Entity* entity) {
	Player* player = entity->data.info.player;

	DisplayObjectEx(OBJECT_RETRYFORGRANDMASTER, 100, player->screenPos[0], 179u, 125u, 0x3F, 0x3F, false);

	UpdateStaffFireworks(entity, player, 15u);
}

static void UpdateEntityDeathIncomplete(Entity* entity) {
	Player* player = entity->data.info.player;

	player->nowFlags |= NOW_NOUPDATE;

	ShowText(player->screenPos[0] - TextWidth("EXCELLENT!") / 2, 120, "EXCELLENT!", 15u, false);

	UpdateStaffFireworks(entity, player, 30u);
}

static const ObjectData* ObjectTableBgDesignStaff[13] = {
	// TODO: Name these.
	OBJECTPTR(0x671), OBJECTPTR(0x672), OBJECTPTR(0x673), OBJECTPTR(0x674), OBJECTPTR(0x675),
	OBJECTPTR(0x676), OBJECTPTR(0x677), OBJECTPTR(0x678), OBJECTPTR(0x679), OBJECTPTR(0x67A),
	OBJECTPTR(0x67B), OBJECTPTR(0x67C), OBJECTPTR(0x67D)
};

static const ObjectData* ObjectTableEffectDesignStaff[2] = {
	// TODO: Name these.
	OBJECTPTR(0x67E), OBJECTPTR(0x67F)
};

static const ObjectData* ObjectTableVisualDesignStaff[2] = {
	// TODO: Name these.
	OBJECTPTR(0x680), OBJECTPTR(0x681)
};

static const ObjectData* ObjectTableProgrammerStaff[4] = {
	// TODO: Name these.
	OBJECTPTR(0x682), OBJECTPTR(0x683), OBJECTPTR(0x684)
};

static const ObjectData* ObjectTableSoundStaff[3] = {
	OBJECTPTR(0x686), OBJECTPTR(0x687), OBJECTPTR(0x688)
};

static const ObjectData* ObjectTableEndStaff[14] = {
	// TODO: Name these.
	OBJECTPTR(0x66F),

	OBJECTPTR(0x689), OBJECTPTR(0x68a), OBJECTPTR(0x68b), OBJECTPTR(0x68c), OBJECTPTR(0x68d),
	OBJECTPTR(0x68e), OBJECTPTR(0x68f), OBJECTPTR(0x690), OBJECTPTR(0x691), OBJECTPTR(0x692),
	OBJECTPTR(0x693), OBJECTPTR(0x694), OBJECTPTR(0x695)
};

static const int8_t StaffSections[6] = { 1, 2, 3, 4, 5, -1 };
static const int16_t EndStaffRows[13] = { 0, 2, 3, 4, 6, 7, 12, 14, 19, 20, 22, 27, 29 };