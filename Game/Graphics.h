#pragma once

// Higher level graphics functions, intended for direct use by the game code.

#include "Player.h"
#include "Graphics.h"
#include "DisplayObject.h"
#include "Fixed.h"
#include "Macros.h"
#include <stdint.h>
#include <stdbool.h>

// TODO: BG code: 0x60169DC ... 0x6016CC6

// Entities can do anything over one or more frames, but are generally only
// used to handle visuals and occasionally sounds associated with visuals.
//
// Updating of all entities occurs after updating the players' gameplay.

// Use if the union applies, otherwise use a custom type in Entity::data.
typedef struct EntityData {
	union {
		Player* player;
		const ObjectData* objectTable;
		struct {
			uint8_t minutes, seconds, centiseconds;
		};
		Fixed32 scale;
		// TODO: Remaining fields.
	};
	DATA(instanceData, 1);
} EntityData;

// Common type useful for most basic types of entities, that don't need
// anything more than the minimum data required to display an object per frame.
typedef struct BasicEntityInstanceData {
	const ObjectData* objectTable;
	uint16_t palNum;
	int16_t y, x;
} BasicEntityInstanceData;

typedef struct Entity Entity;
struct Entity {
	bool allocated;
	uint8_t states[3];
	int16_t values[4];
	void (*update)(Entity* entity);
	DATA(data, 0x100);
};

// Use ENTITY_CUSTOM_DATA_PTR when storing a non-EntityData struct in
// Entity::data.
//
// Use ENTITY_DATA or ENTITY_DATA_PTR when accessing a union field in an
// EntityData struct stored in Entity::data.
//
// For extra data not in the EntityData union, declare a struct for your
// entity, and access the struct using ENTITY_INSTANCE_DATA_PTR.
#define ENTITY_CUSTOM_DATA_PTR(type, ptr, entity) type* ptr = (type*)(entity)->data
#define ENTITY_DATA(entity) (*(EntityData*)entity->data)
#define ENTITY_DATA_PTR(ptr, entity) ENTITY_CUSTOM_DATA_PTR(EntityData, ptr, entity)
#define ENTITY_INSTANCE_DATA_PTR(type, ptr, entity) type* ptr = (type*)((EntityData*)(entity)->data)->instanceData

void InitEntities();
Entity* AllocEntity();
void FreeEntity(Entity* entity);
void UpdateEntities();
