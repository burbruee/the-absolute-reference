#include "Game/Play/Item/UpdateItemShotgun.h"
#include "Game/Play/Player.h"
#include "Game/Play/Block.h"
#include "Game/Play/Matrix.h"
#include "Game/Play/Item/ShowItemWarningShotgun.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Game/Graphics/ShowEffect.h"
#include "Sound/Sound.h"
#include "Lib/Math.h"

enum ShotgunState {
	STATE_START,
	STATE_DELAY,
	STATE_INIT,
	STATE_INDICATESHOTS,
	STATE_SHOTGUN,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE
};
typedef struct ShotgunData {
	Block matrix[MATRIX_HEIGHT][MATRIX_SINGLEWIDTH]; // 0x0
	int16_t shotColumns[MATRIX_HEIGHT]; // 0x210
	int16_t numShots; // 0x23C
} ShotgunData;

// STATE_*
#define frames values[0]

// STATE_DELAY
#define delayShotgunFrames values[1]

// STATE_SHOTGUN
#define delayDelSoundFrames values[1]

void UpdateItemShotgun(Item* item) {
	STATIC_ASSERT(sizeof(ShotgunData) <= sizeoffield(Item, data));

	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	ShotgunData* data = (ShotgunData*)item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_SHOTGUN;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_SHOTGUN);
				ShowItemWarningShotgun(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				item->frames = 0;
				item->delayShotgunFrames = 60;
				item->states[0]++;
			}
			break;

		case STATE_DELAY:
			if (--item->delayShotgunFrames == 0) {
				data->numShots = 0;
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
				for (int16_t col = 0; col < MATRIX_SINGLEWIDTH; col++) {
					// BUG: This uses the activating player's matrix width, when it should probably use item player's.
					data->matrix[row][col] = itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block;
				}
			}

			for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
				data->shotColumns[row] = 0;
			}

			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				int16_t col;
				do {
					col = (Rand(64u) & 0xF) + 1;
				} while (col > FIELD_SINGLEWIDTH);

				if (MATRIX(itemPlayer, row, col).block != NULLBLOCK) {
					data->shotColumns[row] = col;
					data->numShots++;
				}
			}

			item->states[0]++;
			break;

		case STATE_INDICATESHOTS:
			for (int16_t row = 0; row < MATRIX_HEIGHT - 1; row++) {
				if (data->shotColumns[row] != 0) {
					Block indicatorBlock = NULLBLOCK;
					if ((item->frames % 2) == 0) {
						indicatorBlock |= BLOCKTYPE_O;
					}
					else {
						indicatorBlock |= BLOCKTYPE_Z;
					}
					// BUG: This uses the activating player's matrix width, when it should probably use item player's.
					itemPlayer->matrix[row * activatingPlayer->matrixWidth + data->shotColumns[row]].block = indicatorBlock;
				}
			}

			if (++item->frames >= 30) {
				for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
					for (int16_t col = 0; col < MATRIX_SINGLEWIDTH; col++) {
						// BUG: This uses the activating player's matrix width, when it should probably use item player's.
						itemPlayer->matrix[row * activatingPlayer->matrixWidth + col].block = data->matrix[row][col];
					}
				}

				item->frames = 0;
				PlaySoundEffect(SOUNDEFFECT_SHOTGUN);
				item->states[0]++;
			}
			break;

		// NOTE: This state depends on item->values[1] initially being 0;
		// STATE_DELAY leaves it 0 when finished, without explicitly
		// initializing it.
		case STATE_SHOTGUN:
			if (++item->frames >= MATRIX_HEIGHT - 1) {
				item->frames = 40;
				item->states[0]++;
				itemPlayer->screenOffset[1] = 0;
			}
			else {
				itemPlayer->screenOffset[1] = (item->frames & 2) == 0 ? 3 : -3;
				if (data->shotColumns[item->frames] != 0) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						if (data->shotColumns[row] != 0) {
							ShowFieldBlockExplosion(itemPlayer, row, data->shotColumns[row]);
						}
					}
					// BUG: This uses the activating player's matrix width, when it should probably use item player's.
					itemPlayer->matrix[item->frames * activatingPlayer->matrixWidth + data->shotColumns[item->frames]].block = NULLBLOCK;
					itemPlayer->matrix[item->frames * activatingPlayer->matrixWidth + data->shotColumns[item->frames]].itemType = ITEMTYPE_NULL;
					if (item->delayDelSoundFrames == 0) {
						PlaySoundEffect(SOUNDEFFECT_19);
						item->delayDelSoundFrames++;
					}
				}

				if (item->delayDelSoundFrames != 0 && ++item->delayDelSoundFrames > 5) {
					item->delayDelSoundFrames = 0;
				}
			}
			break;

		case STATE_DELAYDEACTIVATE:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_DEACTIVATE:
			itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;

		default:
			break;
		}
	}
}
