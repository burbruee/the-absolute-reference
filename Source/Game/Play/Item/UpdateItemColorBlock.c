#include "Game/Play/Item/UpdateItemColorBlock.h"
#include "Game/Play/Item/ShowItemWarningColorBlock.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Sound/Sound.h"

enum ColorBlockState {
	STATE_START,
	STATE_NEXT,
	STATE_INIT,
	STATE_FIRSTBLOCK,
	STATE_COLORBLOCK,
	STATE_RESETBRIGHTNESS,
	STATE_DEACTIVATE
};

typedef struct ColorBlockData {
	int16_t matrixBrightnesses[MATRIX_HEIGHT][MATRIX_SINGLEWIDTH];
	int16_t numBlocks;
	int16_t numStartBlocks;
	bool stopAnim;
} ColorBlockData;

// STATE_*
#define frames values[0]

// STATE_COLORBLOCK
#define animIndex values[1]

void UpdateItemColorBlock(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	ColorBlockData* data = item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_COLORBLOCK;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_COLORBLOCK);
				ShowItemWarningColorBlock(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;

				for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
					for (int16_t col = 0; col < MATRIX_SINGLEWIDTH; col++) {
						MATRIX(itemPlayer, row, col).brightness = 5;
					}
				}

				item->frames = 98;
				item->states[0]++;
			}
			break;

		case STATE_NEXT:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			switch (activatingPlayer->numBlocks % 3) {
			case 0:
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					int16_t(*brightnessesRow)[MATRIX_SINGLEWIDTH] = &data->matrixBrightnesses[MATRIX_HEIGHT / 2];
					for (int16_t row = 1, topRow = FIELD_HEIGHT - 1, brightness = col; row < FIELD_HEIGHT / 2; row++, brightnessesRow++, brightness += col, topRow++) {
						data->matrixBrightnesses[row][col] = (row * col) % 10;
						data->matrixBrightnesses[row + 9][col] = (row * col) % 10;
						if (topRow <= FIELD_HEIGHT) {
							(*brightnessesRow)[col] = brightness % 10;
						}
					}
				}
				break;

			case 1:
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH / 2; col++) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						data->matrixBrightnesses[row][col] = (row - col >= 0 ? row - col : col - row) % 10;
						data->matrixBrightnesses[row][MATRIX_SINGLEWIDTH - 1 - col] = (row - col >= 0 ? row - col : col - row) % 10;
					}
				}
				break;

			case 2:
			default:
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH / 2; col++) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						data->matrixBrightnesses[row][col] = (row + col) % 10;
						data->matrixBrightnesses[row][MATRIX_SINGLEWIDTH - 1 - col] = (row + col) % 10;
					}
				}
				break;
			}

			item->frames = -10;
			item->animIndex = 0;
			data->numBlocks = 0;
			data->stopAnim = false;
			item->states[0]++;
			break;

		case STATE_FIRSTBLOCK:
			itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			data->numBlocks++;
			data->numStartBlocks = activatingPlayer->numBlocks;
			break;

		case STATE_COLORBLOCK:
			if (!data->stopAnim) {
				if (!(itemPlayer->nowFlags & NOW_NOGARBAGE)) {
					itemPlayer->nowFlags |= NOW_NOGARBAGE;
				}

				if (item->animIndex > 0) {
					for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
						for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
							MATRIX(itemPlayer, row, col).block &= ~BLOCK_INVISIBLE;
							int16_t newBrightness = (data->matrixBrightnesses[row][col] + item->animIndex) % 10;
							MATRIX(itemPlayer, row, col).brightness = newBrightness & 0xFF;
							if (newBrightness == 0) {
								MATRIX(itemPlayer, row, col).block |= BLOCK_INVISIBLE;
							}
						}
					}
				}
				if (item->frames % 3 == 0) {
					item->animIndex++;
				}
				item->frames++;
				if (itemPlayer->play.state != PLAYSTATE_GARBAGECHECK || data->numStartBlocks == itemPlayer->numBlocks) {
					itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
				}
				else if (item->frames > 600 || data->numBlocks >= 3) {
					data->stopAnim = true;
				}
				else {
					itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
					itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
					data->numBlocks++;
				}
			}
			else {
				item->states[0]++;
			}
			break;

		case STATE_RESETBRIGHTNESS:
			for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
				for (int16_t col = 0; col < MATRIX_SINGLEWIDTH; col++) {
					MATRIX(itemPlayer, row, col).brightness = 5;
					if (!(activatingPlayer->modeFlags & MODE_INVISIBLE)) {
						MATRIX(itemPlayer, row, col).block &= ~BLOCK_INVISIBLE;
					}
				}
			}
			item->states[0]++;
			break;

		case STATE_DEACTIVATE:
		default:
			itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			if (!NoDisableGarbage(item)) {
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			}
			DeactivateItem(item);
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			break;
		}
	}
}