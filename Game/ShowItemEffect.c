#include "ShowItemEffect.h"
#include "Entity.h"

static void UpdateEntityItemEffect(Entity* entity);

void ShowItemEffect(Player* player, int16_t row, int16_t col) {
	Entity* entity;

	if (entity = AllocEntity()) {
		entity->update = UpdateEntityItemEffect;
		entity->data.info.player = player;
		entity->values[0] = 0u;
	}
	// TODO
}

static void UpdateEntityItemEffect(Entity* entity) {
	// TODO
}
