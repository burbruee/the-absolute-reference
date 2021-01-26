#include "Game/Play/Item/ShowItemWarningHardBlock.h"
#include "Game/Graphics/Entity.h"
#include "Video/Object.h"
#include "Main/Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningHardBlock(Entity* entity);

void ShowItemWarningHardBlock(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningHardBlock;
		entity->data.unionData.player = player;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		entity->warningFrames = 0;
		data->x = 4;
		data->x = player->screenPos[0] + player->screenOffset[0] - (player->matrixWidth / 2) * 8 + (data->x + 2) * 8;
		data->y = player->screenPos[1] + player->screenOffset[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGHARDBLOCK;
		data->palNum = 185u;
	}
}

static void UpdateEntityItemWarningHardBlock(Entity* entity) {
	STATIC_ASSERT(sizeof(BasicEntityInstanceData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->warningFrames], data->y, data->x, (uint8_t)data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}