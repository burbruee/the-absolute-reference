#include "Game/Graphics/ShowVersusReadyGo.h"
#include "Game/Graphics/Entity.h"
#include "Main/GameLoop.h"
#include "Sound/Sound.h"
#include <stdint.h>

typedef struct InstanceData {
	int16_t y;
	uint16_t UNK_2;
	int16_t x;
	uint16_t UNK_6;
	int8_t numVersusRounds;
	int16_t scaleX;
	int16_t scaleY;
} InstanceData;
static void UpdateEntityVersusReadyGo(Entity* entity);

void ShowVersusReadyGo() {
	uint8_t numVersusRoundsSetting = NumVersusRoundsSetting();
	Entity *entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityVersusReadyGo;
		ENTITY_INST_DATA_PTR(InstanceData, data, entity);

		data->numVersusRounds = Game.numVersusRounds;

		if (data->numVersusRounds >= 5) {
			data->numVersusRounds = 5;
		}

		const uint8_t maxVersusRoundWins = numVersusRoundsSetting - 1u;
		if (Game.numVersusRoundWins[PLAYER1] >= maxVersusRoundWins && Game.numVersusRoundWins[PLAYER2] >= maxVersusRoundWins) {
			data->numVersusRounds = 5;
		}
		GameFlags |= GAME_VERSUSREADYGO;
	}
}

typedef struct STRUCT_601FB30 {
	ObjectData* UNK_0;
	ObjectData* UNK_4;
	int8_t UNK_8;
	int8_t UNK_9;
} STRUCT_601FB30;

static const STRUCT_601FB30 UNK_3B4D4[6] = {
	{ OBJECTPTR(0x171), OBJECTPTR(0x172), 144, 32 },
	{ OBJECTPTR(0x171), OBJECTPTR(0x173), 144, 32 },
	{ OBJECTPTR(0x171), OBJECTPTR(0x174), 144, 32 },
	{ OBJECTPTR(0x171), OBJECTPTR(0x175), 144, 32 },
	{ OBJECTPTR(0x171), OBJECTPTR(0x176), 144, 32 },
	{ OBJECTPTR(0x177), OBJECTPTR(0x178), 144, 32 }
};


static void UpdateEntityVersusReadyGo(Entity* entity) {
	int32_t offsetX, offsetY;
	ENTITY_INST_DATA_PTR(InstanceData, data, entity);
	const STRUCT_601FB30* const struct0 = &UNK_3B4D4[data->numVersusRounds];

	entity->values[0]--;
	switch (entity->states[0]) {
	case 0u:
		entity->states[0]++;
		entity->values[0] = 0x3c;
		data->x = 160;
		data->y = 90;
		data->UNK_2 = 0u;
		data->UNK_6 = 0u;
		data->scaleY = 0;
		data->scaleX = 0;
		break;

	case 1u:
		data->scaleX += 4;
		if (data->scaleX > UNSCALED) {
			data->scaleX = UNSCALED;
		}

		DisplayObjectEx(struct0->UNK_0, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(struct0->UNK_4, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);

		if (entity->values[0] != 0) {
			break;
		}
		entity->states[0]++;
		entity->values[0] = 70;
		PlaySoundEffect(SOUNDEFFECT_READY);
		break;

	case 2u:
		data->scaleY += 4;
		if (data->scaleY > UNSCALED) {
			data->scaleY = UNSCALED;
		}

		DisplayObjectEx(struct0->UNK_0, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(struct0->UNK_4, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(OBJECTPTR(0x179), data->y + 32, data->x, 0u, 110u, (SpriteScale)data->scaleY, UNSCALED, false);

		if (entity->values[0] != 0) {
			break;
		}
		entity->states[0]++;
		entity->values[0] = 30;
		PlaySoundEffect(SOUNDEFFECT_GO);
		break;

	case 3u:
		DisplayObjectEx(struct0->UNK_0, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(struct0->UNK_4, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(OBJECTPTR(0x17A), data->y + 32, data->x, 0u, 110u, (SpriteScale)data->scaleY, UNSCALED, false);

		if (entity->values[0] != 0) {
			break;
		}
		entity->states[0]++;
		entity->values[0] = 16;
		break;

	case 4u:
		data->scaleX += 4;
		if (data->scaleX < 0) {
			data->scaleX = 0;
		}

		offsetX = struct0->UNK_8 * (UNSCALED + 1 - data->scaleX);
		if (offsetX < 0) {
			offsetX += UNSCALED;
		}
		DisplayObjectEx(struct0->UNK_0, data->y, data->x + offsetX / (UNSCALED + 1), 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);

		offsetX = struct0->UNK_9 * (UNSCALED + 1 - data->scaleX);
		if (offsetX < 0) {
			offsetX += UNSCALED;
		}
		DisplayObjectEx(struct0->UNK_4, data->y, data->x + offsetX / (UNSCALED + 1), 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);

		offsetY = 0x18 * (UNSCALED + 1 - data->scaleX);
		if (offsetY < 0) {
			offsetY += UNSCALED;
		}
		DisplayObjectEx(OBJECTPTR(0x179), data->y + offsetY / (UNSCALED + 1) + 32, data->x, 0u, 110u, (SpriteScale)data->scaleX, UNSCALED, false);

		if (entity->values[0] != 0) {
			break;
		}
		FreeEntity(entity);
		GameFlags = GameFlags & ~GAME_VERSUSREADYGO;
	}
}
