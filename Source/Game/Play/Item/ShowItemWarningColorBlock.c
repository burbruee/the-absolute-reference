#include "Game/Play/Item/ShowItemWarningColorBlock.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/DisplayObject.h"
#include "Main/Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningColorBlock(Entity* entity);

void ShowItemWarningColorBlock(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningColorBlock;
		entity->data.unionData.player = player;
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
	STATIC_ASSERT(sizeof(BasicEntityInstanceData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->warningFrames], data->y, data->x, (uint8_t)data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}