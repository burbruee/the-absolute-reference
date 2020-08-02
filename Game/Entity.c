#include "Entity.h"
#include "Macros.h"

static Entity EntityHeap[0x100];

static inline void InitEntity(Entity* entity) {
	entity->values[3] = 0;
	entity->values[2] = 0;
	entity->values[1] = 0;
	entity->values[0] = 0;

	entity->states[3] = 0u;
	entity->states[2] = 0u;
	entity->states[1] = 0u;
	entity->states[0] = 0u;

	entity->allocated = false;

	entity->update = NULL;

	for (size_t i = 0; i < lengthof(entity->buffer); i++) {
		entity->buffer[i] = 0u;
	}
}

void InitEntities() {
	Entity* entity = EntityHeap;
	for (size_t i = 0; i < lengthof(EntityHeap); i++, entity++) {
		InitEntity(entity);
	}
}

Entity* AllocEntity() {
	Entity* entity = EntityHeap;
	for (size_t i = 0; i < lengthof(EntityHeap); i++, entity++) {
		if (!entity->allocated) {
			entity->allocated = true;
			return entity;
		}
	}

	return NULL;
}

void FreeEntity(Entity* entity) {
	InitEntity(entity);
}

void UpdateEntities() {
	Entity* entity = EntityHeap;
	for (size_t i = 0; i < lengthof(EntityHeap); i++, entity++) {
		if (entity->allocated) {
			entity->update(entity);
		}
	}
}
