#include "Game/Play/Item/ShowItemEffect.h"
#include "Game/Graphics/Entity.h"
#include "Game/PalNum.h"
#include "Main/Frame.h"

static void UpdateEntityItemEffect(Entity* entity);

#define frame values[0]

void ShowItemEffect(Player* player, int16_t row, int16_t col) {
	Entity* entity;

	if ((entity = AllocEntity())) {
		entity->update = UpdateEntityItemEffect;
		entity->data.unionData.player = player;
		entity->frame = 0;
		ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
		data->x = player->screenPos[0] + 8 * col;
		data->y = player->screenPos[1] - 8 * row - 6;
		data->objectTable = OBJECTTABLE_ITEMBLOCKEXPLOSION;
		data->palNum = PALNUM_ITEMBLOCKEXPLOSION;
	}
}

static void UpdateEntityItemEffect(Entity* entity) {
	STATIC_ASSERT(sizeof(BasicEntityInstanceData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(&data->objectTable[entity->frame], data->y, data->x, (uint8_t)data->palNum, 115u);
	if (CurrentPauseMode < PAUSEMODE_GAME && ++entity->frame >= 14) {
		FreeEntity(entity);
	}
}
