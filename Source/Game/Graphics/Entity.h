#pragma once

#include "Game/Play/Player.h"
#include "Game/Graphics/DisplayObject.h"
#include "Lib/Fixed.h"
#include "Lib/Macros.h"
#include <stdint.h>
#include <stdbool.h>

// Entities can do anything over one or more frames, but are generally only
// used to handle visuals and occasionally sounds associated with visuals.
//
// Updating of all entities occurs after updating the players' gameplay.

#define ENTITY_BUFFERSIZE (0x40 * sizeof(void*))

// Use if the union applies, otherwise use a custom type in Entity::data.
typedef union EntityInfo {
	Player* player;
	const ObjectData* objectTable;
	struct {
		uint8_t minutes, seconds, centiseconds;
	};
	Fixed32 scale;
	// TODO: Remaining fields.
} EntityInfo;

typedef struct EntityData {
	EntityInfo info;
	DATA(instanceData, ENTITY_BUFFERSIZE - sizeof(EntityInfo));
} EntityData;

// Common type useful for most basic types of entities, that don't need
// anything more than the minimum data required to display an object per frame.
typedef struct BasicEntityInstanceData {
	const ObjectData* objectTable;
	uint16_t palNum;
	int16_t y, x;
	uint16_t layer;
	uint16_t scaleY;
	uint16_t scaleX;
	bool alpha;
} BasicEntityInstanceData;

typedef struct Entity Entity;
struct Entity {
	bool allocated;
	uint8_t states[4];
	int16_t values[4];
	void (*update)(Entity* entity);
	union {
		EntityData data;
		DATA(buffer, ENTITY_BUFFERSIZE);
	};
};

// Use ENTITY_BUFFER_PTR when storing a non-EntityData struct in
// Entity::data.
//
// Use ENTITY_DATA or ENTITY_DATA_PTR when accessing a union field in an
// EntityData struct stored in Entity::data.
//
// For extra data not in the EntityData union, declare a struct for your
// entity, and access the struct using ENTITY_INST_DATA_PTR.
#define ENTITY_BUFFER_PTR(type, ptr, entity) type* ptr = (type*)(entity)->buffer
#define ENTITY_DATA(entity) (entity->data.info)
#define ENTITY_INST_DATA_PTR(type, ptr, entity) type* ptr = (type*)(entity)->data.instanceData

void InitEntities();
Entity* AllocEntity();
void FreeEntity(Entity* entity);
void UpdateEntities();
