#include "Game/Play/Item/UpdateItemDarkBlock.h"
#include "Game/Play/Item/ShowItemWarningDarkBlock.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Video/Object.h"
#include "Sound/Sound.h"

enum DarkBlockState {
	STATE_START,
	STATE_DELAY,
	STATE_INIT,
	STATE_DARKBLOCK,
	STATE_FINISH,
	STATE_DEACTIVATE
};

typedef struct DarkBlockData {
	int16_t darkSwitchDelay;
} DarkBlockData;

#define darkFrames values[0]
#define frames values[1]
#define darkSwitchTime values[2]

void UpdateItemDarkBlock(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	DarkBlockData* data = item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_DARKBLOCK;
				for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
					for (int16_t col = 0; col < MATRIX_SINGLEWIDTH; col++) {
						MATRIX(activatingPlayer, row, col).brightness = 5;
					}
				}
				SetFieldBorderColor(itemPlayer, ITEMTYPE_DARKBLOCK);
				ShowItemWarningDarkBlock(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				item->frames = 0;
				item->states[0]++;
			}
			break;

		case STATE_DELAY:
			if (--item->frames < 0) {
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(itemPlayer, MATRIX_HEIGHT - 1, col).block = NULLBLOCK;
			}

			item->darkFrames = 490;
			item->frames = 0;
			item->darkSwitchTime = 0;
			data->darkSwitchDelay = 12;
			break;

		case STATE_DARKBLOCK:
			if (--item->darkFrames > 0) {
				if (item->darkFrames > TIME(0, 7, 0) && item->frames >= data->darkSwitchDelay) {
					item->frames = 0;
					if (++item->darkSwitchTime > 1 && data->darkSwitchDelay > 3) {
						data->darkSwitchDelay -= 3;
					}

					for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
						for (int16_t col = 0; col < MATRIX_SINGLEWIDTH; col++) {
							if (MATRIX(itemPlayer, row, col).block & BLOCK_TYPE) {
								if (MATRIX(itemPlayer, row, col).block & BLOCK_INVISIBLE) {
									MATRIX(itemPlayer, row, col).block &= ~BLOCK_INVISIBLE;
								}
								else {
									MATRIX(itemPlayer, row, col).block |= BLOCK_INVISIBLE;
								}
							}
						}
					}
				}

				if (item->darkFrames <= TIME(0, 7, 0)) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
							if (MATRIX(itemPlayer, row, col).block & BLOCK_TYPE) {
								MATRIX(itemPlayer, row, col).block |= BLOCK_INVISIBLE;
							}
						}
					}
				}
				item->frames++;
			}
			else {
				item->states[0]++;
			}
			break;

		case STATE_FINISH:
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					if (!(activatingPlayer->modeFlags & MODE_INVISIBLE)) {
						MATRIX(itemPlayer, row, col).block &= ~BLOCK_INVISIBLE;
					}
				}
			}

			item->states[0]++;
			break;

		case STATE_DEACTIVATE:
		default:
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;
		}
	}
}