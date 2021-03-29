#include "Game/Graphics/ShowGameOverFade.h"
#include "Game/Graphics/Entity.h"
#include "Game/Play/Matrix.h"
#include "Input/Button.h"
#include "Main/Frame.h"
#include "Lib/Macros.h"

enum GameOverFadeState {
	STATE_FADE,
	STATE_CHECKENDFADE
};

typedef struct GameOverFadeData {
	struct FadeRow {
		int8_t brightness;
		uint16_t delay;
		uint16_t frames;
	} fadeRows[FIELD_HEIGHT + 1];
} GameOverFadeData;

#define fadesPerFrame values[0]
#define showGameOver values[2]

static void UpdateEntityGameOverFade(Entity* entity);

void ShowGameOverFade(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		player->activeItemType = ITEMTYPE_GAMEOVER;
		if (GameFlags & GAME_DOUBLES) {
			player->otherPlayer->activeItemType = ITEMTYPE_GAMEOVER;
		}

		entity->update = UpdateEntityGameOverFade;
		entity->fadesPerFrame = 1;

		uint32_t fadeDelay = (player->modeFlags & MODE_INVISIBLE) ? TIME(0, 4, 0) : 0u;
		entity->data.unionData.player = player;
		ENTITY_INST_DATA_PTR(GameOverFadeData, data, entity);
		for (int16_t row = 0; row < FIELD_HEIGHT + 1; row++) {
			data->fadeRows[row].delay = fadeDelay + row * 8;
			data->fadeRows[row].frames = 0u;
			data->fadeRows[row].brightness = -5;
		}

		for (int16_t row = 1; row < player->matrixHeight; row++) {
			for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
				MATRIX(player, row, col).brightness = 0;
			}
		}
	}
}

static void UpdateEntityGameOverFade(Entity* entity) {
	STATIC_ASSERT(sizeof(GameOverFadeData) <= sizeoffield(Entity, buffer));

	Player* player = entity->data.unionData.player;
	ENTITY_INST_DATA_PTR(GameOverFadeData, data, entity);
	if (CurrentPauseMode < PAUSEMODE_GAME) {
		if (entity->states[0] == STATE_FADE) {
			if (GameButtonsDown[player->num] & BUTTON_START) {
				entity->fadesPerFrame = 3;
			}

			for (int16_t fades = 0; fades < entity->fadesPerFrame; fades++) {
				for (int16_t row = 1; row < player->matrixHeight; row++) {
					if (data->fadeRows[row - 1].delay == 0) {
						if (data->fadeRows[row - 1].brightness < 5) {
							if (data->fadeRows[row - 1].frames++ % 2 == 0) {
								data->fadeRows[row - 1].brightness++;
							}

							for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
								if ((MATRIX(player, row, col).block & BLOCK_TYPE) >= BLOCKTYPE_I) {
									MATRIX(player, row, col).brightness = data->fadeRows[row - 1].brightness;
								}
							}
						}
						else if (data->fadeRows[row - 1].brightness == 5) {
							data->fadeRows[row - 1].brightness++;
							for (int16_t col = 1; col < player->matrixWidth - 1; col++) {
								Block* block = &MATRIX(player, row, col).block;
								if ((*block & BLOCK_TYPE) >= BLOCKTYPE_I) {
									*block = NULLBLOCK;
								}
							}
						}
					}
					else {
						data->fadeRows[row - 1].delay--;
					}
				}
			}

			if (data->fadeRows[FIELD_HEIGHT].brightness >= 6) {
				entity->states[0]++;
				player->showGameOver = true;
			}
		}
		else if (entity->states[0] == STATE_CHECKENDFADE && ((player->nowFlags & NOW_GAMEOVER) || (player->nowFlags & NOW_INIT))) {
			player->activeItemType = ITEMTYPE_NULL;
			ItemDescriptions[player->num] = ITEMTYPE_NULL;
			FreeEntity(entity);
		}
	}
}
