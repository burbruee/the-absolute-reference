#include "UpdateItemShotgun.h"
#include "Player.h"
#include "Block.h"
#include "Field.h"
#include "ShowBlockField.h"
#include "ShowEffect.h"
#include "Sound.h"
#include "Math.h"

enum ShotgunState {
	STATE_START,
	STATE_DELAYSHOTGUN,
	STATE_INITSHOTGUN,
	STATE_INDICATESHOTS,
	STATE_SHOTGUN,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE
};
typedef struct ShotgunData {
	Block matrix[MATRIX_HEIGHT][MATRIX_SINGLEWIDTH];
	int16_t shotColumns[MATRIX_HEIGHT];
	int16_t numShots;
} ShotgunData;

// STATE_*
#define frames values[0]

// STATE_DELAYSHOTGUN
#define delayShotgunFrames values[1]

// STATE_SHOTGUN
#define delayDeleteSoundFrames values[1]

void UpdateItemShotgun(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	ITEM_DATA_PTR(ShotgunData, data, item);
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

		case STATE_DELAYSHOTGUN:
			if (--item->delayShotgunFrames == 0) {
				data->numShots = 0;
				item->states[0]++;
			}
			break;

		case STATE_INITSHOTGUN:
			for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
				for (int16_t col = 0; col < MATRIX_SINGLEWIDTH; col++) {
					data->matrix[row][col] = MATRIX(itemPlayer, row, col).block;
				}
			}

			for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
				data->shotColumns[row] = 0;
			}

			for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
				int16_t col = (Rand(64u) & 0xF) + 1;
				if (col >= MATRIX_SINGLEWIDTH - 1) {
					continue;
				}

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
					if ((item->frames & 1) == 0) {
						indicatorBlock |= BLOCKTYPE_O;
					}
					else {
						indicatorBlock |= BLOCKTYPE_Z;
					}
					MATRIX(itemPlayer, row, data->shotColumns[row]).block = indicatorBlock;
				}
			}

			if (++item->frames >= 30) {
				for (int16_t row = 0; row < MATRIX_HEIGHT; row++) {
					for (int16_t col = 0; col < MATRIX_SINGLEWIDTH; col++) {
						MATRIX(itemPlayer, row, col).block = data->matrix[row][col];
					}
				}

				item->frames = 0;
				PlaySoundEffect(SOUNDEFFECT_SHOTGUN);
				item->states[0]++;
			}
			break;

		// NOTE: This state depends on item->values[1] initially being 0;
		// STATE_DELAYSHOTGUN leaves it 0 when finished, without explicitly
		// initializing it.
		case STATE_SHOTGUN:
			if (++item->frames >= MATRIX_HEIGHT - 1) {
				item->frames = 40;
				item->states[0]++;
				itemPlayer->fieldShift[1] = 0;
			}
			else {
				itemPlayer->fieldShift[1] = (item->frames & 2) == 0 ? 3 : -3;
				if (data->shotColumns[item->frames] != 0) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						ShowSquareExplosion(itemPlayer, row, data->shotColumns[row]);
					}
					MATRIX(itemPlayer, item->frames, data->shotColumns[item->frames]).block = NULLBLOCK;
					MATRIX(itemPlayer, item->frames, data->shotColumns[item->frames]).itemType = ITEMTYPE_NULL;
					if (item->delayDeleteSoundFrames == 0) {
						PlaySoundEffect(SOUNDEFFECT_19);
						item->delayDeleteSoundFrames++;
					}
				}

				if (item->delayDeleteSoundFrames != 0 && ++item->delayDeleteSoundFrames > 5) {
					item->delayDeleteSoundFrames = 0;
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
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;

		default:
			break;
		}
	}
}