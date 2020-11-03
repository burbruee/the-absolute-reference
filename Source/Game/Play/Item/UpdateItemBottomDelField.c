#include "Game/Play/Item/UpdateItemBottomDelField.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Game/Graphics/ShowRowDel.h"
#include "Sound/Sound.h"
#include <stdint.h>

enum BottomDelFieldState {
	STATE_START,
	STATE_CHECKDELAY,
	STATE_DELAY,
	STATE_INIT,
	STATE_BOTTOMDELFIELD,
	STATE_DELAYCOLLAPSE,
	STATE_COLLAPSE,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE,
	// NOTE: State 9 isn't in the original code.
	STATE_DELAYPLAYCLEARSOUND = 10,
	STATE_PLAYCLEARSOUND
};

typedef struct BottomDelFieldData {
	int16_t topRow;
	bool delayDel;
} BottomDelFieldData;

#define frames values[0]
#define delCol values[1]

void UpdateItemBottomDelField(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	BottomDelFieldData* data = (BottomDelFieldData*)item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			data->delayDel = false;
			item->states[0]++;
		case STATE_CHECKDELAY:
			RemoveItems(activatingPlayer);

			if (!(activatingPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL) {
				if (ItemConfusing(itemPlayer)) {
					data->delayDel = true;
				}

				if (!ItemBad(itemPlayer)) {
					activatingPlayer->activeItemType = ITEMTYPE_BOTTOMDELFIELD;
					SetFieldBorderColor(activatingPlayer, ITEMTYPE_BOTTOMDELFIELD);
					activatingPlayer->play.flags |= PLAYFLAG_FORCEENTRY;

					if (data->delayDel) {
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
			data->topRow = 0;
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					if (MATRIX(activatingPlayer, row, col).block & BLOCK_TYPE) {
						data->topRow = row;
					}
				}
			}
			data->topRow -= data->topRow / 2;

			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(activatingPlayer, MATRIX_HEIGHT - 1, col).block = NULLBLOCK;
			}

			if (data->topRow > 0) {
				item->frames = 0;
				item->delCol = 1;
				item->states[0]++;
			}
			else {
				item->frames = 25;
				item->states[0] = STATE_DELAYPLAYCLEARSOUND;
			}
			break;

		case STATE_BOTTOMDELFIELD:
			if (++item->frames >= 40) {
				item->states[0]++;
			}

			if (item->frames > 10) {
				int16_t col = item->delCol;
				for (int16_t row = 1; row <= data->topRow; row++) {
					if (col == 1) {
						ShowRowDel(activatingPlayer, row);
						PlaySoundEffect(SOUNDEFFECT_CLEAR);
					}

					if (col > 0 && col < MATRIX_SINGLEWIDTH - 1 && (MATRIX(activatingPlayer, row, col).block & BLOCK_TYPE)) {
						MATRIX(activatingPlayer, row, col).block |= BLOCK_ROLLROLL;
					}
					if (col - 3 > 0 && col - 3 < MATRIX_SINGLEWIDTH - 1 && (MATRIX(activatingPlayer, row, col - 3).block & BLOCK_TYPE)) {
						MATRIX(activatingPlayer, row, col - 3).block = NULLBLOCK;
						MATRIX(activatingPlayer, row, col - 3).itemType = ITEMTYPE_NULL;
					}
					if (row % 2) {
						col--;
					}
				}

				item->delCol++;
				if (col - 3 >= MATRIX_SINGLEWIDTH - 1) {
					item->states[0]++;
					item->frames = 25;
				}
			}
			break;

		case STATE_DELAYCOLLAPSE:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_COLLAPSE:
			for (int16_t row = data->topRow; row != 0; row--) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					MATRIX(activatingPlayer, row, col).block = NULLBLOCK;
				}
			}

			PlaySoundEffect(SOUNDEFFECT_COLLAPSE);

			for (int16_t row = 1; row < MATRIX_HEIGHT - 1 - data->topRow; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					MATRIX(activatingPlayer, row, col).block = MATRIX(activatingPlayer, data->topRow + row, col).block;
					MATRIX(activatingPlayer, row, col).brightness = MATRIX(activatingPlayer, data->topRow + row, col).brightness;
					MATRIX(activatingPlayer, row, col).visibleFrames = MATRIX(activatingPlayer, data->topRow + row, col).visibleFrames;
				}
			}

			for (int16_t row = MATRIX_HEIGHT - 1 - data->topRow; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					MATRIX(activatingPlayer, row, col).block = NULLBLOCK;
					MATRIX(activatingPlayer, row, col).itemType = ITEMTYPE_NULL;
				}
			}

			item->frames = 40;
			item->states[0]++;
			break;

		case STATE_DELAYDEACTIVATE:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_DEACTIVATE:
			activatingPlayer->nowFlags &= ~NOW_NOGARBAGE;
			activatingPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			data->delayDel = false;
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			activatingPlayer->clearItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(activatingPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;

		case STATE_DELAYPLAYCLEARSOUND:
			if (--item->frames == 0) {
				item->states[0]++;
				item->frames = 40;
			}
			break;

		case STATE_PLAYCLEARSOUND:
			PlaySoundEffect(SOUNDEFFECT_CLEAR);
			item->states[0] = STATE_DELAYDEACTIVATE;
			break;

		default:
			break;
		}
	}
}
