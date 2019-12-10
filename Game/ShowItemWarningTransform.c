#include "ShowItemWarningTransform.h"
#include "Graphics.h"
#include "Object.h"
#include "DisplayObject.h"
#include "Frame.h"

#define warningFrames values[0]

void ShowItemWarningTransform(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningTransform;
		ENTITY_DATA(entity).player = player;
		ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, data, entity);
		entity->warningFrames = 0;
		data->x = 4;
		data->x =
			player->fieldPos[0] + player->fieldShift[0] -
			((player->matrixWidth / 2) * 8 + (data->x + 2) * 8);
		data->y = player->fieldPos[1] + player->fieldShift[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGTRANSFORM;
		data->palNum = 183u;
	}
}

void UpdateEntityItemWarningTransform(Entity* entity) {
	ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(data->objectTable[entity->warningFrames], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}