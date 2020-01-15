#include "ShowItemWarningXRay.h"
#include "Graphics.h"
#include "Object.h"
#include "Frame.h"

#define warningFrames values[0]

void UpdateEntityItemWarningXRay(Entity* entity);

void ShowItemWarningXRay(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningXRay;
		ENTITY_DATA(entity).player = player;
		entity->warningFrames = 0;
		ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = 4;
		data->x = player->screenPos[0] + player->screenOffset[0] - (player->matrixWidth / 2) * 8 + (data->x + 2) * 8;
		data->y = player->screenPos[1] + player->screenOffset[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGXRAY;
		data->palNum = 194u;
	}
}

void UpdateEntityItemWarningXRay(Entity* entity) {
	ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->values[0]], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}