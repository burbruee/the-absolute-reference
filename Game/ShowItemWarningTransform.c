#include "ShowItemWarningTransform.h"
#include "Entity.h"
#include "DisplayObject.h"
#include "Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningTransform(Entity* entity);

void ShowItemWarningTransform(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningTransform;
		ENTITY_DATA(entity).player = player;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		entity->warningFrames = 0;
		data->x = 4;
		data->x =
			player->screenPos[0] + player->screenOffset[0] -
			((player->matrixWidth / 2) * 8 + (data->x + 2) * 8);
		data->y = player->screenPos[1] + player->screenOffset[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGTRANSFORM;
		data->palNum = 183u;
	}
}

static void UpdateEntityItemWarningTransform(Entity* entity) {
	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(data->objectTable[entity->warningFrames], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}