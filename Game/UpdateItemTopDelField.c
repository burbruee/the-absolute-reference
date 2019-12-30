#include "UpdateItemTopDelField.h"
#include "Player.h"
#include "ShowBlockField.h"
#include "ShowRowDel.h"
#include "Sound.h"

enum TopDelFieldState {
	STATE_START,
	STATE_CHECKDELAY,
	STATE_DELAY,
	STATE_INIT,
	STATE_TOPDELFIELD,
	STATE_FINISH,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE,
	// NOTE: States 8 and 9 aren't in TAP's code.
	STATE_DELAYPLAYCLEARSOUND = 10,
	STATE_PLAYCLEARSOUND
};

typedef struct TopDelFieldData {
	int16_t numDelRows;
	int16_t bottomDelRow;
	bool delayDel;
} TopDelFieldData;

// STATE_*
#define frames values[0]

// STATE_TOPDELFIELD
#define startDelCol values[1]

void UpdateItemTopDelField(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	ITEM_DATA_PTR(TopDelFieldData, data, item);

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

			if (!ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_TOPDELFIELD;
				SetFieldBorderColor(activatingPlayer, ITEMTYPE_TOPDELFIELD);

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
		data->numDelRows = 0;
		for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
			int16_t topRow = 0;
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				if (MATRIX(activatingPlayer, row, col).block & BLOCK_TYPE) {
					topRow = row;
				}
			}
			if (data->numDelRows < topRow) {
				data->numDelRows = topRow;
			}
		}

		data->bottomDelRow = data->numDelRows / 2;
		data->numDelRows -= data->bottomDelRow;
		data->bottomDelRow++;
		for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
			MATRIX(activatingPlayer, MATRIX_HEIGHT - 1, col).block = NULLBLOCK;
			MATRIX(activatingPlayer, MATRIX_HEIGHT - 1, col).itemType = ITEMTYPE_NULL;
		}

		if (data->numDelRows > 0) {
			item->frames = 0;
			item->startDelCol = 1;
			item->states[0]++;
		}
		else {
			item->frames = 25;
			item->states[0] = STATE_DELAYPLAYCLEARSOUND;
		}
		break;

	case STATE_TOPDELFIELD:
		if (++item->frames >= 40) {
			item->states[0]++;
		}

		if (item->frames > 10) {
			int16_t col = item->startDelCol;
			for (int16_t row = data->bottomDelRow + data->numDelRows - 1; row >= data->bottomDelRow; row--) {
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

				if (row & 1) {
					col--;
				}
			}

			item->startDelCol++;

			if (col - 3 >= MATRIX_SINGLEWIDTH - 1) {
				item->states[0]++;
			}
		}
		break;

	case STATE_FINISH:
		item->states[0]++;

		for (int16_t numDelRows = data->numDelRows; numDelRows != 0; numDelRows--) {
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				int16_t row = data->bottomDelRow + numDelRows - 1;
				if (row > 0 && row < MATRIX_HEIGHT / 2) {
					MATRIX(activatingPlayer, row, col).block = NULLBLOCK;
				}
				MATRIX(activatingPlayer, row, col).itemType = ITEMTYPE_NULL;
			}
		}

		item->frames = 40;
		break;

	case STATE_DELAYDEACTIVATE:
		if (--item->frames == 0) {
			item->states[0]++;
		}
		break;

	case STATE_DEACTIVATE:
	default:
		activatingPlayer->nowFlags &= ~NOW_NOGARBAGE;
		activatingPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
		item->startDelCol = 0;
		activatingPlayer->activeItemType = ITEMTYPE_NULL;
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
	}
}