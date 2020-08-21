#include "Game/Play/Item/ShowItemWarningDeathBlock.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/DisplayObject.h"
#include "Main/Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningDeathBlock(Entity* entity);

void ShowItemWarningDeathBlock(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity())) {
		entity->update = UpdateEntityItemWarningDeathBlock;
		entity->data.info.player = player;
		entity->warningFrames = 0;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = 4;
		data->x =
			player->screenPos[0] + player->screenOffset[0] +
			(data->x + 2) * 8 - (player->matrixWidth / 2) * 8;
		data->y = player->screenPos[1] + player->screenOffset[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGDEATHBLOCK;
		data->palNum = 187u;
	}
}

static void UpdateEntityItemWarningDeathBlock(Entity* entity) {
	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->warningFrames], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}
