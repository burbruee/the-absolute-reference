#include "Game/Play/Item/UpdateItemFlipField.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Sound/Sound.h"

enum FlipFieldState {
	STATE_START,
	STATE_DELAY,
	STATE_INIT,
	STATE_LOWER,
	STATE_DELAYRISE,
	STATE_RISE,
	STATE_INITFLIPFIELD,
	STATE_FLIPFIELD,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE
};

typedef struct FlipFieldData {
	int16_t numFlipRows;
	int16_t topRow;
	int16_t topFlipRow;
	int16_t flipDelay;
	MatrixBlock matrix[NUMMATRIXBLOCKS_SINGLE];
} FlipFieldData;

// STATE_*
#define frames values[0]

// STATE_FLIPFIELD
#define flipFrames values[1]

void UpdateItemFlipField(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	FlipFieldData* data = (FlipFieldData*)item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);

			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				SetFieldBorderColor(itemPlayer, ITEMTYPE_FLIPFIELD);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				item->frames = 60;
				item->states[0]++;
			}
			break;

		case STATE_DELAY:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(itemPlayer, MATRIX_HEIGHT - 1, col).block = NULLBLOCK;
				MATRIX(itemPlayer, MATRIX_HEIGHT - 1, col).itemType = ITEMTYPE_NULL;
			}

			data->topRow = 0;
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					data->matrix[row * MATRIX_SINGLEWIDTH + col] = MATRIX(itemPlayer, row, col);
					if (MATRIX(itemPlayer, row, col).block & BLOCK_TYPE) {
						data->numFlipRows = row;
						data->topRow = row;
					}
				}
			}

			// NOTE: Appears this was mistakenly left in by Arika's programmer(s); the above loop does what this loop does.
			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					data->matrix[row * MATRIX_SINGLEWIDTH + col] = MATRIX(itemPlayer, row, col);
				}
			}

			if (data->topRow > 0) {
				item->states[0]++;
			}
			else {
				item->frames = 40;
				item->states[0] = STATE_DELAYDEACTIVATE;
			}
			break;

		case STATE_LOWER:
			itemPlayer->screenOffset[1] += 4;
			if (itemPlayer->screenOffset[1] > 48) {
				item->states[0]++;
				item->frames = 10;
			}
			break;

		case STATE_DELAYRISE:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_RISE:
			itemPlayer->screenOffset[1] -= 32;
			if (itemPlayer->screenOffset[1] <= 16) {
				item->states[0]++;
			}
			break;

		case STATE_INITFLIPFIELD:
			item->flipFrames = 1;
			data->topFlipRow = 1;
			data->flipDelay = data->topRow + data->numFlipRows / 2;
			item->states[0]++;
			break;

		case STATE_FLIPFIELD:
			if (data->numFlipRows == FIELD_HEIGHT) {
				itemPlayer->screenOffset[1] = 0;
			}

			if (data->numFlipRows < 2 * FIELD_HEIGHT - 1 && item->flipFrames >= data->flipDelay) {
				item->flipFrames = 0;
				data->flipDelay = ((data->numFlipRows >= FIELD_HEIGHT ? 2 * FIELD_HEIGHT - data->numFlipRows : data->numFlipRows) >> 1) / 7;

				for (int16_t copyRow = 1, flipRow = data->topFlipRow; flipRow < data->numFlipRows + 2; flipRow++) {
					int16_t row = flipRow >= FIELD_HEIGHT ? 2 * FIELD_HEIGHT - flipRow : flipRow;
					for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
						if (flipRow == data->topFlipRow) {
							MATRIX(itemPlayer, row, col).block = NULLBLOCK;
							MATRIX(itemPlayer, row, col).itemType = ITEMTYPE_NULL;
						}
						else {
							MATRIX(itemPlayer, row, col) = data->matrix[copyRow * MATRIX_SINGLEWIDTH + col];
						}
					}

					if (flipRow != data->topFlipRow) {
						copyRow++;
					}
				}

				data->topFlipRow++;
				data->numFlipRows++;
				if (data->numFlipRows >= 2 * FIELD_HEIGHT - 1) {
					PlaySoundEffect(SOUNDEFFECT_COLLAPSE);
					item->frames = 40;
					item->states[0]++;
				}
				else {
					item->flipFrames++;
				}
			}
			else {
				item->flipFrames++;
			}

			break;

		case STATE_DELAYDEACTIVATE:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_DEACTIVATE:
		default:
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			for (int16_t i = 0; i < lengthoffield(Player, screenOffset); i++) {
				itemPlayer->screenOffset[i] = 0;
			}
			itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			itemPlayer->nowFlags &= ~NOW_BIT15;
			DeactivateItem(item);
			break;
		}
	}
}
