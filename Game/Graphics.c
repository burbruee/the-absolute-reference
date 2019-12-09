#include "Graphics.h"
#include "Macros.h"

// TODO: BG data at 0x6066188

static Entity EntityHeap[0x100];

static inline void InitEntity(Entity* entity) {
	entity->values[3] = 0;
	entity->values[2] = 0;
	entity->values[1] = 0;
	entity->values[0] = 0;

	entity->states[2] = 0u;
	entity->states[1] = 0u;
	entity->states[0] = 0u;

	entity->allocated = false;

	entity->update = NULL;

	for (size_t i = 0; i < lengthof(entity->data); i++) {
		entity->data[i] = 0u;
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
