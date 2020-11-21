#include "Game/Graphics/ShowVersusReadyGo.h"
#include "Game/Graphics/Entity.h"
#include "Main/GameLoop.h"
#include "Sound/Sound.h"
#include <stdint.h>

typedef enum VersusReadyGoState {
	STATE_INIT,
	STATE_START,
	STATE_READY,
	STATE_GO,
	STATE_END
} VersusReadyGoState;

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

typedef struct VersusReadyGoDisplay {
	ObjectData* roundLabelObject;
	ObjectData* roundNumberObject;
	uint8_t roundLabelX;
	uint8_t roundNumberX;
} VersusReadyGoDisplay;

static const VersusReadyGoDisplay VersusReadyGoDisplays[6] = {
	{ &OBJECTTABLE_VERSUSREADYGO[0], &OBJECTTABLE_VERSUSREADYGO[1], 144u,  32u },
	{ &OBJECTTABLE_VERSUSREADYGO[0], &OBJECTTABLE_VERSUSREADYGO[2], 144u,  32u },
	{ &OBJECTTABLE_VERSUSREADYGO[0], &OBJECTTABLE_VERSUSREADYGO[3], 144u,  32u },
	{ &OBJECTTABLE_VERSUSREADYGO[0], &OBJECTTABLE_VERSUSREADYGO[4], 144u,  32u },
	{ &OBJECTTABLE_VERSUSREADYGO[0], &OBJECTTABLE_VERSUSREADYGO[5], 144u,  32u },
	{ &OBJECTTABLE_VERSUSREADYGO[6], &OBJECTTABLE_VERSUSREADYGO[7], 144u, 112u }
};

#define frames values[0]

static void UpdateEntityVersusReadyGo(Entity* entity) {
	int32_t offsetX, offsetY;
	ENTITY_INST_DATA_PTR(InstanceData, data, entity);
	const VersusReadyGoDisplay* const display = &VersusReadyGoDisplays[data->numVersusRounds];

	entity->frames--;
	switch (entity->states[0]) {
	case STATE_INIT:
		entity->states[0]++;
		entity->frames = 60;
		data->x = 160;
		data->y = 90;
		data->UNK_2 = 0u;
		data->UNK_6 = 0u;
		data->scaleY = 0;
		data->scaleX = 0;
		break;

	case STATE_START:
		data->scaleX += 4;
		if (data->scaleX > UNSCALED) {
			data->scaleX = UNSCALED;
		}

		DisplayObjectEx(display->roundLabelObject, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(display->roundNumberObject, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);

		if (entity->frames == 0) {
			entity->states[0]++;
			entity->frames = 70;
			PlaySoundEffect(SOUNDEFFECT_READY);
		}
		break;

	case STATE_READY:
		data->scaleY += 4;
		if (data->scaleY > UNSCALED) {
			data->scaleY = UNSCALED;
		}

		DisplayObjectEx(display->roundLabelObject, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(display->roundNumberObject, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(&OBJECTTABLE_VERSUSREADYGO[8], data->y + 32, data->x, 0u, 110u, (SpriteScale)data->scaleY, UNSCALED, false);

		if (entity->frames == 0) {
			entity->states[0]++;
			entity->frames = 30;
			PlaySoundEffect(SOUNDEFFECT_GO);
		}
		break;

	case STATE_GO:
		DisplayObjectEx(display->roundLabelObject, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(display->roundNumberObject, data->y, data->x, 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);
		DisplayObjectEx(&OBJECTTABLE_VERSUSREADYGO[9], data->y + 32, data->x, 0u, 110u, (SpriteScale)data->scaleY, UNSCALED, false);

		if (entity->frames == 0) {
			entity->states[0]++;
			entity->frames = 16;
		}
		break;

	case STATE_END:
		data->scaleX -= 4;
		if (data->scaleX < 0) {
			data->scaleX = 0;
		}

		offsetX = display->roundLabelX * (UNSCALED + 1 - data->scaleX);
		if (offsetX < 0) {
			offsetX += UNSCALED;
		}
		DisplayObjectEx(display->roundLabelObject, data->y, data->x + offsetX / (UNSCALED + 1), 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);

		offsetX = display->roundNumberX * (UNSCALED + 1 - data->scaleX);
		if (offsetX < 0) {
			offsetX += UNSCALED;
		}
		DisplayObjectEx(display->roundNumberObject, data->y, data->x + offsetX / (UNSCALED + 1), 0u, 110u, UNSCALED, (SpriteScale)data->scaleX, false);

		offsetY = 0x18 * (UNSCALED + 1 - data->scaleX);
		if (offsetY < 0) {
			offsetY += UNSCALED;
		}
		DisplayObjectEx(&OBJECTTABLE_VERSUSREADYGO[9], data->y + offsetY / (UNSCALED + 1) + 32, data->x, 0u, 110u, (SpriteScale)data->scaleX, UNSCALED, false);

		if (entity->frames == 0) {
			FreeEntity(entity);
			GameFlags &= ~GAME_VERSUSREADYGO;
		}
		break;

	default:
		break;
	}
}
