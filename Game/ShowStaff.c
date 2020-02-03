#include "ShowStaff.h"
#include "Graphics.h"
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
static void UpdateStaffFireworks(Entity* staffFireworks, Player* player, uint16_t delay);
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

#define NUMSTAFFMASTERGMLINES 32u

static const ObjectData* ObjectTableBgDesignStaff[13];
static const ObjectData* ObjectTableEffectDesignStaff[2];
static const ObjectData* ObjectTableVisualDesignStaff[2];
static const ObjectData* ObjectTableProgrammerStaff[4];
static const ObjectData* ObjectTableSoundStaff[3];
static const ObjectData* ObjectTableEndStaff[14];
static const int8_t StaffSections[6];
static const int16_t EndStaffRows[13];

typedef struct StaffData {
	uint16_t scrollRate;
	int16_t scrollDivisor;
	int16_t pixelX[NUMSTAFFOBJECTS];
	int16_t pixelY[NUMSTAFFOBJECTS];
	int16_t bottomPixelY;
	int16_t topPixelY;
	int16_t completePixelY;
	bool skip;
} StaffData;

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
	#define STAFF_OBJECTS(section, objectTable) \
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
		STAFF_OBJECTS(STAFF_BGDESIGN, objectTableBgDesignStaff);
		STAFF_OBJECTS(STAFF_EFFECTDESIGN, objectTableEffectDesignStaff);
		STAFF_OBJECTS(STAFF_VISUALDESIGN, objectTableVisualDesignStaff);
		STAFF_OBJECTS(STAFF_PROGRAMMER, objectTableProgrammerStaff);
		STAFF_OBJECTS(STAFF_SOUND, objectTableSoundStaff);

		default:
			break;
		}
	}
	#undef STAFF_OBJECTS

	for (int16_t i = 0; i < lengthof(objectTableEndStaff); i++, staffObjectIndex++) {
		staffRows[staffObjectIndex] = endStaffRows[i] + staffRow;
		ObjectTableStaff[player->num][staffObjectIndex] = objectTableEndStaff[i + 1];
	}

	Entity* staffEntity;
	if ((staffEntity = AllocEntity()) != NULL) {
		staffEntity->update = UpdateEntityStaff;
		ENTITY_DATA(staffEntity).player = player;
		StaffFireworkFrames[player->num] = 15u;
		// TODO: Name these values.
		staffEntity->values[0] = 0;
		staffEntity->values[1] = 0;
		staffEntity->values[2] = 240;
		staffEntity->values[3] = 1;
		ENTITY_INSTANCE_DATA_PTR(StaffData, data, staffEntity);
		data->skip = false;
		data->bottomPixelY = player->screenPos[1] + (player->matrixHeight - 1) * 8 - (player->matrixHeight - 1) * 8 - 6;
		data->topPixelY = player->screenPos[1] + (player->matrixHeight - MATRIX_HEIGHT) * 8 - (player->matrixHeight - 1) * 8 - 6;
		data->completePixelY = player->screenPos[1] + (player->matrixHeight - MATRIX_HEIGHT + 8) * 8 - (player->matrixHeight - 1) * 8 - 6;
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
	Player* player = ENTITY_DATA(entity).player;

	if ((GameFlags & GAME_TWIN) && GameButtonsDown[player->num] == (BUTTON_3 | BUTTON_2 | BUTTON_1)) {
		player->nowFlags |= NOW_SHOWRANKINGCODE;
	}
	if (player->modeFlags & (MODE_20G | MODE_BIG | MODE_ITEM | MODE_TLS)) {
		player->nowFlags &= ~NOW_SHOWRANKINGCODE;
	}

	ENTITY_INSTANCE_DATA_PTR(StaffData, data, entity);
	if (!(player->modeFlags & (MODE_NORMAL | MODE_TGMPLUS | MODE_TADEATH)) || !(GameButtonsDown[player->num] & BUTTON_START)) {
		if ((player->modeFlags & MODE_DOUBLES) && ((GameButtonsDown[PLAYER1] & BUTTON_START) || (GameButtonsDown[PLAYER2] & BUTTON_START))) {
			data->scrollRate = 3u;
			data->scrollDivisor = 1;
		}
		else {
			int16_t divisor;
			if (player->modeFlags & MODE_MASTER) {
				data->scrollRate = 1u;
				divisor = 4;
			}
			else {
				data->scrollRate = 1u;
				divisor = 2;
			}
			data->scrollDivisor = divisor;
		}
	}
	else {
		data->scrollRate = 3u;
		data->scrollDivisor = 1u;
	}

	// TODO
}
// TODO

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