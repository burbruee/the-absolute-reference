#include "UpdateItemLeftMovField.h"
#include "Player.h"
#include "ShowBlockField.h"
#include "Sound.h"
#include <stdint.h>

enum LeftMovFieldState {
	STATE_START,
	STATE_CHECKDELAY,
	STATE_DELAY,
	STATE_INIT,
	STATE_SHIFT,
	STATE_DELAYSLAM,
	STATE_SLAM,
	STATE_LEFTMOVFIELD,
	STATE_CENTER,
	STATE_DELAYPOSTCENTER,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE
};

typedef struct LeftMovFieldData {
	bool delayLeftMov;
} LeftMovFieldData;

// STATE_*
#define frames values[0]

void UpdateItemLeftMovField(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	LeftMovFieldData* data = item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			data->delayLeftMov = false;
			item->states[0]++;
		case STATE_CHECKDELAY:
			RemoveItems(activatingPlayer);
			if (!(activatingPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL) {
				if (ItemConfusing(itemPlayer)) {
					data->delayLeftMov = true;
				}

				if (!ItemBad(itemPlayer)) {
					activatingPlayer->activeItemType = ITEMTYPE_LEFTMOVFIELD;
					SetFieldBorderColor(activatingPlayer, ITEMTYPE_LEFTMOVFIELD);
					activatingPlayer->play.flags |= PLAYFLAG_FORCEENTRY;

					if (data->delayLeftMov) {
						item->states[0]++;
					}
					else {
						activatingPlayer->nowFlags |= NOW_NOGARBAGE;
						item->states[0] += 2;
					}
				}
			}
			break;

		case STATE_DELAY:
			if (activatingPlayer->play.state == PLAYSTATE_GARBAGECHECK) {
				activatingPlayer->nowFlags |= NOW_NOGARBAGE;
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			item->states[0]++;
			item->frames = 0;
			break;

		case STATE_SHIFT:
			activatingPlayer->screenOffset[0] += 4;
			if (activatingPlayer->screenOffset[0] > 48) {
				item->states[0]++;
				item->frames = 10;
			}
			break;

		case STATE_DELAYSLAM:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_SLAM:
			activatingPlayer->screenOffset[0] -= 32;
			if (activatingPlayer->screenOffset <= -16) {
				item->states[0]++;
			}
			break;

		case STATE_LEFTMOVFIELD:
			for (int16_t row = 1; row < MATRIX_HEIGHT; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					if (MATRIX(activatingPlayer, row, col).block == NULLBLOCK) {
						int16_t movCol = col;
						bool movLeft = true;
						do {
							if (++movCol >= MATRIX_SINGLEWIDTH - 1) {
								col = MATRIX_SINGLEWIDTH;
								movLeft = false;
								break;
							}
						} while (MATRIX(activatingPlayer, row, movCol).block == NULLBLOCK);

						if (movLeft) {
							MATRIX(activatingPlayer, row, col).block = MATRIX(activatingPlayer, row, movCol).block;
							MATRIX(activatingPlayer, row, col).brightness = MATRIX(activatingPlayer, row, movCol).brightness;
							MATRIX(activatingPlayer, row, col).visibleFrames = MATRIX(activatingPlayer, row, movCol).visibleFrames;

							MATRIX(activatingPlayer, row, movCol).block = NULLBLOCK;
							MATRIX(activatingPlayer, row, movCol).itemType = ITEMTYPE_NULL;
						}
					}
				}
			}
			PlaySoundEffect(SOUNDEFFECT_COLLAPSE);
			item->states[0]++;
			break;

		case STATE_CENTER:
			activatingPlayer->screenOffset[0] += 2;
			if (activatingPlayer->screenOffset[0] >= 0) {
				activatingPlayer->screenOffset[0] = 0;
				item->frames = 0;
				item->states[0]++;
			}
			break;

		case STATE_DELAYPOSTCENTER:
			if (--item->frames <= 0) {
				item->states[0]++;
				item->frames = 40;
			}
			break;

		case STATE_DELAYDEACTIVATE:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_DEACTIVATE:
			activatingPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(activatingPlayer, ITEMTYPE_NULL);
			data->delayLeftMov = false;
			activatingPlayer->screenOffset[0] = 0;
			for (int16_t i = 0; i < lengthoffield(Item, values); i++) {
				item->values[i] = 0;
			}
			activatingPlayer->nowFlags &= ~NOW_NOGARBAGE;
			DeactivateItem(item);
			break;

		default:
			break;
		}
	}
}