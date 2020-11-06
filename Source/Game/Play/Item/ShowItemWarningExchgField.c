#include "Game/Play/Item/ShowItemWarningExchgField.h"
#include "Game/Graphics/Entity.h"
#include "Video/Object.h"
#include "Main/Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningExchgField(Entity* entity);

void ShowItemWarningExchgField(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningExchgField;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		entity->data.unionData.player = player;
		entity->warningFrames = 0;
		data->x = 4;
		data->x = player->screenPos[0] + player->screenOffset[0] - (player->matrixWidth / 2) * 8 + (data->x + 2) * 8;
		data->y = player->screenPos[1] + player->screenOffset[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGEXCHGFIELD;
		data->palNum = 196u;
	}
}

static void UpdateEntityItemWarningExchgField(Entity* entity) {
	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->warningFrames], data->y, data->x, data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}