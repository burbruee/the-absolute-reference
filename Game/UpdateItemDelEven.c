#include "UpdateItemDelEven.h"
#include "Player.h"
#include "ShowBlockField.h"
#include "ShowRowDel.h"
#include "Sound.h"
#include <stdint.h>

enum DelEvenState {
	STATE_START,
	STATE_CHECKDELAY,
	STATE_DELAY,
	STATE_INIT,
	STATE_DELEVEN,
	STATE_DELAYCOLLAPSE,
	STATE_COLLAPSE,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE,
	// NOTE: State 9 isn't in the original code.
	STATE_DELAYPLAYCLEARSOUND = 10,
	STATE_PLAYCLEARSOUND
};

typedef struct DelEvenData {
	int16_t delRow;
	bool delayDel;
} DelEvenData;

// STATE_*
#define frames values[0]

void UpdateItemDelEven(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	DelEvenData* data = item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			data->delayDel = false;
			item->states[0]++;
		case STATE_CHECKDELAY:
			RemoveItems(activatingPlayer);
			if (!(activatingPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL) {
				if (!ItemConfusing(itemPlayer)) {
					data->delayDel = true;
				}

				if (!ItemBad(itemPlayer)) {
					activatingPlayer->activeItemType = ITEMTYPE_DELEVEN;
					activatingPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
					SetFieldBorderColor(activatingPlayer, ITEMTYPE_DELEVEN);
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
				activatingPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			data->delRow = MATRIX_HEIGHT - 1;
			for (bool check = true; check;) {
				data->delRow--;
				if (data->delRow == 1) {
					break;
				}
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					if (MATRIX(activatingPlayer, data->delRow, col).block != NULLBLOCK) {
						check = false;
					}
				}
			}

			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(activatingPlayer, MATRIX_HEIGHT - 1, col).block = NULLBLOCK;
				MATRIX(activatingPlayer, MATRIX_HEIGHT - 1, col).itemType = ITEMTYPE_NULL;
			}

			if (data->delRow > 1) {
				item->frames = 0;
				item->states[0]++;
			}
			else {
				item->frames = 25;
				item->states[0] = STATE_DELAYPLAYCLEARSOUND;
			}
			break;

		case STATE_DELEVEN:
			if (++item->frames % 8 == 0 && item->frames > 10) {
				PlaySoundEffect(SOUNDEFFECT_SELECT);
				ShowRowDel(activatingPlayer, data->delRow);

				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					if (MATRIX(activatingPlayer, data->delRow, col).block != NULLBLOCK) {
						MATRIX(activatingPlayer, data->delRow, col).block = NULLBLOCK;
					}
					MATRIX(activatingPlayer, data->delRow, col).itemType = ITEMTYPE_NULL;
				}

				if ((data->delRow -= 2) < 1) {
					item->states[0]++;
					item->frames = 25;
				}
			}
			break;

		case STATE_DELAYCOLLAPSE:
			if (--item->frames == 0) {
				item->states[0]++;
				PlaySoundEffect(SOUNDEFFECT_COLLAPSE);
			}
			break;

		case STATE_COLLAPSE:
			item->states[0]++;

			for (int16_t rowsAbove = 1, row = 1 + (data->delRow == 0); rowsAbove != MATRIX_HEIGHT / 2 - (data->delRow == 0); rowsAbove++, row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					MATRIX(activatingPlayer, row, col).block = MATRIX(activatingPlayer, row + rowsAbove, col).block;
					MATRIX(activatingPlayer, row, col).brightness = MATRIX(activatingPlayer, row + rowsAbove, col).brightness;
					MATRIX(activatingPlayer, row, col).visibleFrames = MATRIX(activatingPlayer, row + rowsAbove, col).visibleFrames;

					MATRIX(activatingPlayer, row + rowsAbove, col).block = NULLBLOCK;
					MATRIX(activatingPlayer, row + rowsAbove, col).itemType = ITEMTYPE_NULL;
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
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(activatingPlayer, ITEMTYPE_NULL);
			data->delayDel = false;
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
}