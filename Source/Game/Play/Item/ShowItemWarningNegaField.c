#include "Game/Play/Item/ShowItemWarningNegaField.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/DisplayObject.h"
#include "Main/Frame.h"

#define warningFrames values[0]

static void UpdateEntityItemWarningNegaField(Entity* entity);

void ShowItemWarningNegaField(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityItemWarningNegaField;
		entity->data.unionData.player = player;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		entity->warningFrames = 0;
		data->x = 4;
		data->x =
			player->screenPos[0] + player->screenOffset[0] +
			(data->x + 2) * 8 - (player->matrixWidth / 2) * 8;
		data->y = player->screenPos[1] + player->screenOffset[1] - 96;
		data->objectTable = OBJECTTABLE_ITEMWARNINGNEGAFIELD;
		data->palNum = 195u;
	}
}

static void UpdateEntityItemWarningNegaField(Entity* entity) {
	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->warningFrames], data->y, data->x, (uint8_t)data->palNum, 124u);

	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->warningFrames >= 64) {
		FreeEntity(entity);
	}
}
