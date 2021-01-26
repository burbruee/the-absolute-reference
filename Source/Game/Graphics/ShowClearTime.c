#include "Game/Graphics/ShowClearTime.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/DisplayObject.h"
#include "Game/Play/Ranking.h"
#include "Video/Object.h"

static void UpdateEntityClearTime(Entity* entity);

typedef struct ClearTimeData {
	int16_t y, x;
} ClearTimeData;

void ShowClearTime(uint32_t time, int16_t y, int16_t x) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityClearTime;
		entity->values[0] = 1;

		uint8_t minutes = time / TIME(1, 0, 0);
		time -= minutes * TIME(1, 0, 0);
		uint8_t seconds = time / TIME(0, 1, 0);
		uint8_t centiseconds = ((time - seconds * TIME(0, 1, 0)) * 100u) / TIME(0, 1, 0);
		entity->data.unionData.minutes = minutes;
		entity->data.unionData.seconds = seconds;
		entity->data.unionData.centiseconds = centiseconds;

		ENTITY_INST_DATA_PTR(ClearTimeData, data, entity);
		data->y = y;
		data->x = x;
	}
}

void UpdateEntityClearTime(Entity* entity) {
	STATIC_ASSERT(sizeof(ClearTimeData) <= sizeoffield(Entity, buffer));

	ENTITY_INST_DATA_PTR(ClearTimeData, data, entity);
	int16_t y = data->y;
	int16_t x = data->x;
	uint8_t minutes = entity->data.unionData.minutes;
	uint8_t seconds = entity->data.unionData.seconds;
	uint8_t centiseconds = entity->data.unionData.centiseconds;

	DisplayObject(ObjectTableRankingDigits[minutes / 10u], y, x, 0u, 110u); // 11 wide.
	DisplayObject(ObjectTableRankingDigits[minutes % 10u], y, x + 11, 0u, 110u); // 9 wide.

	DisplayObject(OBJECT_RANKINGTIMECOLON, y, x + 22, 0u, 110u); // 7 wide.

	DisplayObject(ObjectTableRankingDigits[seconds / 10u], y, x + 27, 0u, 110u); // 11 wide.
	DisplayObject(ObjectTableRankingDigits[seconds % 10u], y, x + 38, 0u, 110u); // 11 wide.

	DisplayObject(OBJECT_RANKINGTIMECOLON, y, x + 49, 0u, 110u); // 5 wide.

	DisplayObject(ObjectTableRankingDigits[centiseconds / 10u], y, x + 54, 0u, 110u); // 11 wide.
	DisplayObject(ObjectTableRankingDigits[centiseconds % 10u], y, x + 65, 0u, 110u);

	FreeEntity(entity);
}
