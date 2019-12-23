#include "UpdateItemXRay.h"
#include "Player.h"
#include "ShowBlockField.h"
#include "ShowItemWarningXRay.h"
#include "Sound.h"

enum XRayState {
	STATE_START,
	STATE_DELAYINITXRAY,
	STATE_INITXRAY,
	STATE_XRAY,
	STATE_SETVISIBLE,
	STATE_DEACTIVATE
};

// STATE_*
#define frames values[0]

// STATE_XRAY
#define visibleColumn values[1]

void UpdateItemXRay(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_XRAY;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_XRAY);
				ShowItemWarningXRay(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				activatingPlayer->nowFlags |= NOW_NOGARBAGE;
				item->frames = 100;
				item->states[0]++;
			}
			break;

		case STATE_DELAYINITXRAY:
			if (--item->frames < 0) {
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
				item->states[0]++;
			}
			break;

		case STATE_INITXRAY:
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(itemPlayer, MATRIX_HEIGHT - 1, col).block = NULLBLOCK;
			}

			item->frames = 100;
			item->visibleColumn = 0;
			item->states[0]++;
			break;

		case STATE_XRAY:
			if (--item->frames > 0) {
				for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
					for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
						if (item->visibleColumn % 60 == col) {
							MATRIX(itemPlayer, row, col).block &= ~BLOCK_INVISIBLE;
						}
						else {
							MATRIX(itemPlayer, row, col).block |= BLOCK_INVISIBLE;
						}
					}
				}

				if (item->visibleColumn % 60 == 0) {
					PlaySoundEffect(SOUNDEFFECT_IRS);
				}

				item->visibleColumn++;
			}
			else {
				item->states[0]++;
			}
			break;

		case STATE_SETVISIBLE:
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					MATRIX(itemPlayer, row, col).block &= ~BLOCK_INVISIBLE;
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