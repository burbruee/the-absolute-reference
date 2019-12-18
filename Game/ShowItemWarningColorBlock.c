#include "ShowItemWarningColorBlock.h"
#include "Graphics.h"
#include "DisplayObject.h"
#include "Object.h"
#include "Frame.h"

void ShowItemWarningColorBlock(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningColorBlock;
		ENTITY_DATA(entity).player = player;
		entity->values[0] = 0;
		ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = 4;
		data->x = player->fieldPos[0] + player->fieldShift[0] + (data->x + 2) * 8 - (player->matrixWidth / 2) * 8;
		data->y = player->fieldPos[1] + player->fieldShift[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGCOLORBLOCK;
		data->palNum = 189u;
	}
}

void UpdateEntityItemWarningColorBlock(Entity* entity) {
	ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->values[0]], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && entity->values[0] >= 64) {
		FreeEntity(entity);
	}
}