#include "ShowItemWarningColorBlock.h"
#include "Entity.h"
#include "DisplayObject.h"
#include "Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningColorBlock(Entity* entity);

void ShowItemWarningColorBlock(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningColorBlock;
		entity->data.info.player = player;
		entity->warningFrames = 0;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = 4;
		data->x = player->screenPos[0] + player->screenOffset[0] + (data->x + 2) * 8 - (player->matrixWidth / 2) * 8;
		data->y = player->screenPos[1] + player->screenOffset[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGCOLORBLOCK;
		data->palNum = 189u;
	}
}

static void UpdateEntityItemWarningColorBlock(Entity* entity) {
	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->warningFrames], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}