#include "Game/Play/Item/ShowItemWarningRollRoll.h"
#include "Game/Graphics/Entity.h"
#include "Video/Object.h"
#include "Main/Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningRollRoll(Entity* entity);

void ShowItemWarningRollRoll(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningRollRoll;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		entity->data.unionData.player = player;
		entity->warningFrames = 0;
		data->x = 4;
		data->x = player->screenPos[0] + player->screenOffset[0] - (player->matrixWidth / 2) * 8 + (data->x + 2) * 8;
		data->y = player->screenPos[1] + player->screenOffset[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGROLLROLL;
		data->palNum = 182u;
	}
}

static void UpdateEntityItemWarningRollRoll(Entity* entity) {
	STATIC_ASSERT(sizeof(BasicEntityInstanceData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->warningFrames], data->y, data->x, (uint8_t)data->palNum, 124u);
	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}