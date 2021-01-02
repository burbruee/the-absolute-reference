#include "Game/Play/Item/UpdateItemMirrorBlock.h"
#include "Game/Play/Item/ShowItemWarningMirrorBlock.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Game/Graphics/ShowColumnMarker.h"
#include "Sound/Sound.h"
#include <stdint.h>

enum MirrorBlockState {
	STATE_START,
	STATE_NEXT,
	STATE_INIT,
	STATE_MIRRORFIRST,
	STATE_END,
	STATE_STARTNEXT,
	STATE_INITNEXT,
	STATE_MIRRORNEXT,
	STATE_CHECKNEXT,
	STATE_DEACTIVATE
};

typedef struct MirrorBlockData {
	int16_t topRow;
	int16_t unused;
	int16_t numBlocks;
	MatrixBlock matrix[MATRIX_HEIGHT][MATRIX_SINGLEWIDTH];
} MirrorBlockData;

// STATE_*
#define frames values[0]

// STATE_MIRROR*
#define mirrorColumn values[1]

// STATE_NEXT
#define firstBlockFrames values[1]

void UpdateItemMirrorBlock(Item* item) {
	STATIC_ASSERT(sizeof(MirrorBlockData) <= sizeoffield(Item, data));

	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	MirrorBlockData* data = (MirrorBlockData*)item->data;

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

		case STATE_NEXT:
			if (--item->firstBlockFrames <= 0) {
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			for (size_t row = 1; row < MATRIX_HEIGHT; row++) {
				for (size_t col = 1; col < MATRIX_SINGLEWIDTH; col++) {
					data->matrix[row][col] = itemPlayer->matrix[row * activatingPlayer->matrixWidth + MATRIX_SINGLEWIDTH - 1 - col];
				}
			}

			data->topRow = 0;
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					if ((itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block & BLOCK_TYPE) != BLOCKTYPE_EMPTY) {
						data->topRow = row;
					}
				}
			}

			item->frames = 46;
			item->mirrorColumn = 1;
			item->states[0]++;
			break;

		case STATE_MIRRORFIRST:
			if (!(itemPlayer->nowFlags & NOW_NOGARBAGE)) {
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
			}

			if (item->frames > 0) {
				if (item->mirrorColumn < MATRIX_SINGLEWIDTH - 1) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						itemPlayer->matrix[row * activatingPlayer->matrixWidth + item->mirrorColumn] = data->matrix[row][item->mirrorColumn];
					}
					ShowColumnMarker(itemPlayer, item->mirrorColumn);
					item->mirrorColumn++;
				}
				item->frames--;
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
					data->matrix[row][col] = itemPlayer->matrix[row * activatingPlayer->matrixWidth + MATRIX_SINGLEWIDTH - 1 - col];
				}
			}

			data->topRow = 0;
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH; col++) {
					if (itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block != NULLBLOCK) {
						data->topRow = row;
					}
				}
			}

			if (data->topRow > 0) {
				item->frames = 15;
				item->mirrorColumn = 1;
				item->states[0]++;
			}
			else {
				item->states[0] = ITEMSTATE_DEFAULT;
			}
			break;

		case STATE_MIRRORNEXT:
			if (item->frames > 0) {
				if (item->mirrorColumn < MATRIX_SINGLEWIDTH - 1) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						itemPlayer->matrix[row * activatingPlayer->matrixWidth + item->mirrorColumn] = data->matrix[row][item->mirrorColumn];
					}
					ShowColumnMarker(itemPlayer, item->mirrorColumn);
					item->mirrorColumn++;
				}
				item->frames--;
			}
			else {
				item->values[0] = 0;
				item->states[0]++;
				data->numBlocks++;
			}
			break;

		case STATE_CHECKNEXT:
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
}
