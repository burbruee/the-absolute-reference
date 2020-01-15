#include "ShowEffect.h"
#include "Graphics.h"
#include "Loop.h"
#include "DisplayObject.h"
#include "Pause.h"
#include "Block.h"
#include "Object.h"
#include "ShowBlockField.h"

typedef struct EntityInstanceDataSquareExplosion {
	ObjectData* objectTable;
	uint16_t palNum;
	int16_t y;
	int16_t x;
} EntityInstanceDataSquareExplosion;

static const ObjectData* ObjectTablesSquareExplosions[NUMSQUAREEXPLOSIONTYPES] = {
	// TODO: Give these names based on their colors. They don't actually
	// correspond to block colors, because they're randomly selected.
	&OBJECTTABLES_SQUAREEXPLOSIONS[0 * NUMSQUAREEXPLOSIONS],
	&OBJECTTABLES_SQUAREEXPLOSIONS[1 * NUMSQUAREEXPLOSIONS],
	&OBJECTTABLES_SQUAREEXPLOSIONS[2 * NUMSQUAREEXPLOSIONS],
	&OBJECTTABLES_SQUAREEXPLOSIONS[3 * NUMSQUAREEXPLOSIONS],
	&OBJECTTABLES_SQUAREEXPLOSIONS[4 * NUMSQUAREEXPLOSIONS],
	&OBJECTTABLES_SQUAREEXPLOSIONS[5 * NUMSQUAREEXPLOSIONS],
	&OBJECTTABLES_SQUAREEXPLOSIONS[6 * NUMSQUAREEXPLOSIONS],
	&OBJECTTABLES_SQUAREEXPLOSIONS[7 * NUMSQUAREEXPLOSIONS]
};

static void UpdateEntitySquareExplosion(Entity* entity);
#define SQUAREEXPLOSION_FRAME 0
void ShowSquareExplosion(Player* player, int16_t row, int16_t col) {
	Entity *entity;
	if (entity = AllocEntity()) {
		entity->update = UpdateEntitySquareExplosion;

		ENTITY_INSTANCE_DATA_PTR(EntityInstanceDataSquareExplosion, instanceData, entity);
		ENTITY_DATA_PTR(data, entity);

		data->player = player;

		entity->values[SQUAREEXPLOSION_FRAME] = 0;

		instanceData->x = player->screenPos[0] + 8 * col - 8 * ((player->matrixWidth + (player->matrixWidth < 0)) / 2);
		instanceData->y = player->screenPos[1] - 8 * row - 6;
		instanceData->objectTable = ObjectTablesSquareExplosions[Rand(8u) % 8];
		instanceData->palNum = PalNumTableNormalBlocks[TOBLOCKNUM(MATRIX(player, row, col).block & BLOCK_TYPE)];
	}
}

static void UpdateEntitySquareExplosion(Entity* entity) {
	ENTITY_INSTANCE_DATA_PTR(EntityInstanceDataSquareExplosion, data, entity);
	DisplayObject(&data->objectTable[entity->values[SQUAREEXPLOSION_FRAME]], data->y, data->x, data->palNum + 9u, 115u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->values[SQUAREEXPLOSION_FRAME] < 32) {
		FreeEntity(entity);
	}
}

void ShowFireworks(Player *player, int16_t row, int16_t col, uint32_t seed) {
		// TODO
}

void ShowAllClear(Player *player, int16_t row, int16_t col) {
		// TODO
}

// _0x60173B4; unused.

void ShowClear(Player *player, int16_t row) {
		// TODO
}

void ShowSingleRowClear(Player *player, int16_t row) {
	// TODO
}

void ShowThrownOutActiveBlock(Player *player) {
	// TODO
}
