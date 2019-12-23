#include "UpdateItemMirrorBlock.h"
#include "Player.h"
#include "ShowBlockField.h"
#include "ShowItemWarningMirrorBlock.h"
#include "ShowMirrorMarker.h"
#include "Sound.h"
#include <stdint.h>

enum MirrorBlockState {
	STATE_START,
	STATE_DELAY,
	STATE_INIT,
	STATE_MIRRORFIRST,
	STATE_END,
	STATE_STARTNEXT,
	STATE_INITNEXT,
	STATE_MIRRORNEXT,
	STATE_NEXT,
	STATE_DEACTIVATE
};

typedef struct ItemMirrorBlockData {
	int16_t topRow;
	int16_t unused;
	int16_t numBlocks;
	Square matrix[MATRIX_HEIGHT][MATRIX_SINGLEWIDTH];
} ItemMirrorBlockData;

#define mirrorFrames values[0]

#define mirrorColumn values[1]

#define firstBlockFrames values[1]

void UpdateItemMirrorBlock(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	ITEM_DATA_PTR(ItemMirrorBlockData, data, item);

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_MIRRORBLOCK;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_MIRRORBLOCK);
				ShowItemWarningMirrorBlock(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				data->numBlocks = 0;
				item->values[0] = 0; // NOTE: Unused.
				item->firstBlockFrames = 60;
				item->states[0]++;
			}
			break;

		case STATE_DELAY:
			if (--item->firstBlockFrames <= 0) {
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			for (int16_t row = 1; row < MATRIX_HEIGHT; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH; col++) {
					data->matrix[row][col] = MATRIX(itemPlayer, row, MATRIX_SINGLEWIDTH - 1 - col);
				}
			}

			data->topRow = 0;
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH; col++) {
					if (MATRIX(itemPlayer, row, col).block != NULLBLOCK) {
						data->topRow = row;
					}
				}
			}

			item->mirrorFrames = 46;
			item->mirrorColumn = 1;
			item->states[0]++;
			break;

		case STATE_MIRRORFIRST:
			if (!(itemPlayer->nowFlags & NOW_NOGARBAGE)) {
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
			}

			if (item->mirrorFrames > 0) {
				if (item->mirrorColumn < MATRIX_SINGLEWIDTH - 1) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						MATRIX(itemPlayer, row, item->mirrorColumn) = data->matrix[row][item->mirrorColumn];
					}
					ShowMirrorMarker(itemPlayer, item->mirrorColumn);
					item->mirrorColumn++;
				}
				item->mirrorFrames--;
			}
			else {
				item->values[0] = 0;
				item->states[0]++;
				data->numBlocks++;
			}
			break;

		case STATE_END:
			itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			item->states[0]++;
			break;

		case STATE_STARTNEXT:
			if (!(itemPlayer->nowFlags & NOW_NOGARBAGE)) {
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
			}

			if (itemPlayer->play.state == PLAYSTATE_GARBAGECHECK) {
				item->states[0]++;
			}
			break;

		case STATE_INITNEXT:
			for (int16_t row = 1; row < MATRIX_HEIGHT; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH; col++) {
					data->matrix[row][col] = MATRIX(itemPlayer, row, MATRIX_SINGLEWIDTH - 1 - col);
				}
			}

			data->topRow = 0;
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH; col++) {
					if (MATRIX(itemPlayer, row, col).block != NULLBLOCK) {
						data->topRow = row;
					}
				}
			}

			if (data->topRow > 0) {
				item->mirrorFrames = 15;
				item->mirrorColumn = 1;
				item->states[0]++;
			}
			else {
				item->states[0] = ITEMSTATE_DEFAULT;
			}
			break;

		case STATE_MIRRORNEXT:
			if (item->mirrorFrames > 0) {
				if (item->mirrorColumn < MATRIX_SINGLEWIDTH - 1) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						MATRIX(itemPlayer, row, item->mirrorColumn) = data->matrix[row][item->mirrorColumn];
					}
					ShowMirrorMarker(itemPlayer, item->mirrorColumn);
					item->mirrorColumn++;
				}
				item->mirrorFrames--;
			}
			else {
				item->values[0] = 0;
				item->states[0]++;
				data->numBlocks++;
			}
			break;

		case STATE_NEXT:
			if (data->numBlocks < 3) {
				item->states[0] = STATE_END;
			}
			else {
				item->states[0]++;
			}
			break;

		case STATE_DEACTIVATE:
		default:
			if (!NoDisableGarbage(item)) {
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			}
			itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			data->unused = 0;
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;
		}
	}
	// TODO
}