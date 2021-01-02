#include "Game/Play/Item/UpdateItemXRay.h"
#include "Game/Play/Item/ShowItemWarningXRay.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Sound/Sound.h"

enum XRayState {
	STATE_START,
	STATE_DELAY,
	STATE_INIT,
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

		case STATE_DELAY:
			if (--item->frames < 0) {
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				itemPlayer->matrix[(MATRIX_HEIGHT - 1) * activatingPlayer->matrixWidth + col].block = NULLBLOCK;
			}

			item->frames = 300;
			item->visibleColumn = 0;
			item->states[0]++;
			break;

		case STATE_XRAY:
			if (--item->frames > 0) {
				for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
					for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
						if ((itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block & BLOCK_TYPE) != BLOCKTYPE_EMPTY) {
							if (item->visibleColumn % 60 == col) {
								itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block &= ~BLOCK_INVISIBLE;
							}
							else {
								itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block |= BLOCK_INVISIBLE;
							}
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
					if ((itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block & BLOCK_TYPE) != BLOCKTYPE_EMPTY && !(activatingPlayer->modeFlags & MODE_INVISIBLE)) {
						itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block &= ~BLOCK_INVISIBLE;
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