#include "ShowItemWarningDeathBlock.h"
#include "Graphics.h"
#include "DisplayObject.h"
#include "Object.h"
#include "Layer.h"
#include "GamePalNum.h"
#include "Pause.h"

void ShowItemWarningDeathBlock(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity())) {
		entity->update = UpdateEntityItemWarningDeathBlock;
		ENTITY_DATA(entity)->player = player;
		entity->values[0] = 0;
		ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, instanceData, entity);
		instanceData->x = 4;
		instanceData->x =
			player->fieldPos[0] + player->fieldShift[0] +
			(instanceData->x + 2) * 8 -
			(player->matrixWidth / 2) * 8;
		instanceData->y = player->fieldPos[1] + player->fieldShift[1] - 96;
		instanceData->objectTable = OBJECTTABLE_ITEMWARNINGDEATHBLOCK;
		instanceData->palNum = 187u;
	}
}

void UpdateEntityItemWarningDeathBlock(Entity* entity) {
	ENTITY_INSTANCE_DATA_PTR(BasicEntityInstanceData, instanceData, entity);
	DisplayObject(instanceData->objectTable[entity->values[0]], instanceData->y, instanceData->x, instanceData->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->values[0] >= 64) {
		FreeEntity(entity);
	}
}
