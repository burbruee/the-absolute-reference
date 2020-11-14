#include "Game/Graphics/ShowNewChallenger.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/Layer.h"
#include "Game/Play/Player.h"
#include "Video/Video.h"
#include "Video/Object.h"
#include "Game/PalNum.h"
#include "Video/Pal.h"
#include "Sound/Sound.h"
#include "Main/Frame.h"
#include "Lib/Fixed.h"
#include <stdint.h>

typedef struct NewChallengerData {
	Fixed32 scaleY;
	Fixed32 scaleYV;
	Fixed32 scaleYA;
	int16_t x;
} NewChallengerData;

enum NewChallengerState {
	// TODO: Rename these, probably. Do some runtime analysis to decide the
	// names.
	STATE_SCROLLRIGHTSTART,
	STATE_SCROLLRIGHT, // TODO: This uses values[1], which needs a name based on this state's final name.
	STATE_SCALESLOWSTART,
	STATE_SCALESLOW,
	STATE_SCALEFAST
};

#define displayFrames values[0]
static void UpdateEntityNewChallenger(Entity* entity);

void ShowNewChallenger(Player* player) {
	Entity* entity;
	if ((entity = AllocEntity()) != NULL) {
		entity->update = UpdateEntityNewChallenger;
		entity->states[0] = STATE_SCALESLOWSTART;
		entity->displayFrames = 180;
		PlaySoundEffect(SOUNDEFFECT_NEWCHALLENGER);
		SetPal(PALNUM_TEXTOVERLAYBG, 1u, PAL_TEXTOVERLAYBG);
	}
}


static void UpdateEntityNewChallenger(Entity* entity) {
	ENTITY_BUFFER_PTR(NewChallengerData, data, entity);
	if (CurrentPauseMode < PAUSEMODE_GAME) {
		switch (entity->states[0]) {
		case STATE_SCROLLRIGHTSTART:
			entity->states[0]++;
			entity->values[1] = 8;
			data->scaleY = F32(0x04, 0x0000);
			data->x = -320;

		case STATE_SCROLLRIGHT:
			data->x += 40;
			if (data->x > 0) {
				data->x = 0;
			}
			if (--entity->values[1] == 0) {
				entity->states[0]++;
			}
			break;

		case STATE_SCALESLOWSTART:
			entity->states[0]++;
			data->x = 0;
			data->scaleY = F32(0x04, 0x0000);
			data->scaleYV = F32(0x18, 0x0000);
			data->scaleYA = F32(0x01, 0x8000);

		case STATE_SCALESLOW:
			data->scaleY += data->scaleYV;
			data->scaleYV -= data->scaleYA;
			if (data->scaleY <= F32(UNSCALED, 0x0000)) {
				entity->states[0]++;
				data->scaleYV = -F32(0x02, 0x0000);
				data->scaleYA = -F32(0x00, 0x2000);
			}
			break;

		case STATE_SCALEFAST:
			data->scaleY += data->scaleYV;
			data->scaleYV -= data->scaleYA;
			if (data->scaleY >= F32(UNSCALED, 0x0000)) {
				entity->states[0]++;
				data->scaleY = F32(UNSCALED, 0x0000);
			}
			break;

		default:
			break;
		}

		if (--entity->displayFrames != 0) {
			SpriteScale scaleY = (SpriteScale)F32I(data->scaleY);
			int16_t y = (0x40 - F32I(data->scaleY)) * 20;
			if (y < 0) {
				y += 127;
			}
			// TODO: See what index 1 of the object table is.
			DisplayObjectEx(&OBJECTTABLE_NEWCHALLENGER[0], y / 128 + 120, data->x + 170, PALNUM_TEXTOVERLAYBG, 125u, (SpriteScale)F32I(data->scaleY), UNSCALED, false);
		}
		else {
			GameFlags &= ~GAME_CHALLENGEDELAY;
			FreeEntity(entity);
		}
	}
}
