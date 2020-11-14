#include "Game/Graphics/ShowObject.h"
#include "Game/Graphics/Entity.h"

static void UpdateEntityObject(Entity* entity);

void ShowObject(const ObjectData* object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer) {
	Entity* entity;
	if ((entity = AllocEntity())) {
		entity->update = UpdateEntityObject;
		entity->values[0] = 1;
		ENTITY_BUFFER_PTR(BasicEntityInstanceData, data, entity);
		data->objectTable = object;
		data->y = y;
		data->x = x;
		data->palNum = palNum;
		data->layer = layer;
	}
}

static void UpdateEntityObject(Entity* entity) {
	ENTITY_BUFFER_PTR(BasicEntityInstanceData, data, entity);
	DisplayObject(data->objectTable, data->y, data->x, (uint8_t)data->palNum, data->layer);
	FreeEntity(entity);
}

static void UpdateEntityObjectEx(Entity* entity);

void ShowObjectEx(const ObjectData* object, int16_t y, int16_t x, uint8_t palNum, uint16_t layer, SpriteScale scaleY, SpriteScale scaleX, bool alpha) {
	Entity* entity;
	if ((entity = AllocEntity())) {
		entity->update = UpdateEntityObjectEx;
		entity->values[0] = 1;
		ENTITY_BUFFER_PTR(BasicEntityInstanceData, data, entity);
		data->objectTable = object;
		data->y = y;
		data->x = x;
		data->palNum = palNum;
		data->layer = layer;
		data->scaleY = scaleY;
		data->scaleX = scaleX;
		data->alpha = alpha;
	}
}

static void UpdateEntityObjectEx(Entity* entity) {
	ENTITY_BUFFER_PTR(BasicEntityInstanceData, data, entity);
	DisplayObjectEx(data->objectTable, data->y, data->x, (uint8_t)data->palNum, data->layer, (SpriteScale)data->scaleY, (SpriteScale)data->scaleX, data->alpha);
	FreeEntity(entity);
}
