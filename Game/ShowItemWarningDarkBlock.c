#include "ShowItemWarningDarkBlock.h"
#include "Graphics.h"
#include "Object.h"
#include "Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningDarkBlock(Entity* entity);

void ShowItemWarningDarkBlock(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningDarkBlock;
		ENTITY_DATA(entity).player = player;
		ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, data, entity);
		entity->warningFrames = 0;
		data->x = 4;
		data->x = player->fieldPos[0] + player->fieldShift[0] - (player->matrixWidth / 2) * 8;
		data->y = player->fieldPos[1] + player->fieldShift[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGDARKBLOCK;
		data->palNum = 190u;
	}
}

static void UpdateEntityItemWarningDarkBlock(Entity* entity) {
	ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->warningFrames], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}