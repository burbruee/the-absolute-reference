#include "Game/Play/Item/UpdateItemLaser.h"
#include "Game/Play/Item/ShowLaser.h"
#include "Game/Play/Player.h"
#include "Game/Play/Matrix.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Game/Graphics/ShowEffect.h"
#include "Input/Button.h"
#include "Sound/Sound.h"

enum LaserState {
	STATE_START,
	STATE_INIT,
	STATE_DELAY,
	STATE_LASER,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE
};

typedef struct LaserData {
	int16_t laserCenterColumn;
	int16_t unused;
	int16_t laserFrames;
	int8_t numShifts[2];
	int8_t numRotations[2];
	int8_t laserWidth;
	int8_t laserColumns[FIELD_SINGLEWIDTH];
} LaserData;

typedef enum NumShifts {
	NUMSHIFTS_RIGHT,
	NUMSHIFTS_LEFT
} NumShifts;

typedef enum NumRotations {
	NUMROTATIONS_ITEMPLAYER,
	NUMROTATIONS_ACTIVATINGPLAYER
} NumRotations;

// STATE_*
#define frames values[0]

// STATE_DELAY
#define lockOnFrames values[1]

// STATE_LASER
#define delRow values[1]

static int16_t LaserCenterColumn(Player* player);

void UpdateItemLaser(Item* item) {
	STATIC_ASSERT(sizeof(LaserData) <= sizeoffield(Item, data));

	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	LaserData* data = (LaserData*)item->data;

	if (!CheckDeactivateItem(item)) {
		// BUG: Though laserCenterColumn isn't initialized on the first call of
		// UpdateLaserItem, and uses the uninitialized value here,
		// laserCenterColumn is initialized below, before its value is used to
		// control item behavior. After that init, this pair of if()s is
		// necessary for controlling the position of the laser.
		if (data->laserCenterColumn < 1) {
			data->laserCenterColumn = 1;
		}
		if (data->laserCenterColumn > FIELD_SINGLEWIDTH) {
			data->laserCenterColumn = FIELD_SINGLEWIDTH;
		}

		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_LASER;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_LASER);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				data->laserCenterColumn = LaserCenterColumn(itemPlayer);
				if (data->laserCenterColumn < 0 || data->laserCenterColumn > MATRIX_SINGLEWIDTH - 1) {
					data->laserCenterColumn = 5;
				}
				data->unused = (data->laserCenterColumn * 14) - 70;
				data->laserFrames = 80;
				data->numShifts[NUMSHIFTS_LEFT] = 0;
				data->numShifts[NUMSHIFTS_RIGHT] = 0;
				data->numRotations[NUMROTATIONS_ACTIVATINGPLAYER] = 0;
				data->numRotations[NUMROTATIONS_ITEMPLAYER] = 0;
				item->frames = 20;
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			if (--item->frames == 0) {
				item->frames = 30;
				item->lockOnFrames = 0;
				item->states[0]++;
			}
			break;

		case STATE_DELAY:
			if (data->laserFrames > 0) {
				if (GameButtonsNew[itemPlayer->num] & BUTTON_RIGHT) {
					data->numShifts[NUMSHIFTS_RIGHT]++;
					if (data->numShifts[NUMSHIFTS_RIGHT] % 3 == 0) {
						data->laserCenterColumn++;
					}
					if (data->laserCenterColumn < 1) {
						data->laserCenterColumn = 1;
					}
					if (data->laserCenterColumn > FIELD_SINGLEWIDTH) {
						data->laserCenterColumn = FIELD_SINGLEWIDTH;
					}
					data->unused = (data->laserCenterColumn * 14) - 70;
				}
				if (GameButtonsNew[itemPlayer->num] & BUTTON_LEFT) {
					data->numShifts[NUMSHIFTS_LEFT]++;
					if (data->numShifts[NUMSHIFTS_LEFT] % 3 == 0) {
						data->laserCenterColumn--;
					}
					if (data->laserCenterColumn < 1) {
						data->laserCenterColumn = 1;
					}
					if (data->laserCenterColumn > FIELD_SINGLEWIDTH) {
						data->laserCenterColumn = FIELD_SINGLEWIDTH;
					}
					data->unused = (data->laserCenterColumn * 14) + 70;
				}

				if (ROTATED_ANY(GameButtonsNew[itemPlayer->num])) {
					data->numRotations[NUMROTATIONS_ITEMPLAYER]++;
				}
				if (ROTATED_ANY(GameButtonsNew[activatingPlayer->num]) && !(activatingPlayer->modeFlags & MODE_ITEM)) {
					data->numRotations[NUMROTATIONS_ACTIVATINGPLAYER]++;
				}
			}

			if ((data->numRotations[NUMROTATIONS_ITEMPLAYER] / 10) - (data->numRotations[NUMROTATIONS_ACTIVATINGPLAYER] / 15) > 0) {
				data->laserWidth = (data->numRotations[NUMROTATIONS_ITEMPLAYER] / 10) - (data->numRotations[NUMROTATIONS_ACTIVATINGPLAYER] / 15) + 1;
			}
			else {
				data->laserWidth = 1;
			}

			const int16_t columnOffsets[FIELD_SINGLEWIDTH * 2] = {
				0,
				1, -1,
				2, -2,
				3, -3,
				4, -4,
				5, -5,
				6, -6,
				7, -7,
				8, -8,
				9, -9,
				-1
			};
			for (int16_t i = 0, col = 0; col < data->laserWidth; i++) {
				if (data->laserCenterColumn + columnOffsets[i] > 0 && data->laserCenterColumn + columnOffsets[i] < MATRIX_SINGLEWIDTH - 1) {
					ShowLaserReticle(itemPlayer, FIELD_HEIGHT, columnOffsets[i] + data->laserCenterColumn, data->laserFrames / 5);
					data->laserColumns[col] = data->laserCenterColumn + columnOffsets[i];
					col++;
				}
			}

			if (data->laserFrames > 0) {
				if (++item->lockOnFrames % 4 == 0) {
					data->laserFrames -= 5;
					if (data->laserFrames < 0) {
						data->laserFrames = 0;
					}
				}
				PlaySoundEffect(SOUNDEFFECT_LOCKON);
			}
			else {
				if (--item->frames == 0) {
					item->frames = 0;
					item->delRow = MATRIX_HEIGHT - 1;
					item->states[0]++;
				}
			}
			break;

		case STATE_LASER:
			for (int16_t i = 0; i < data->laserWidth; i++) {
				ShowLaserReticle(itemPlayer, FIELD_HEIGHT, data->laserColumns[i], 0);
				if (item->frames == 0) {
					ShowLaser(itemPlayer, FIELD_HEIGHT, data->laserColumns[i]);
				}
			}
			if (item->frames == 0) {
				PlaySoundEffect(SOUNDEFFECT_LASER);
			}

			if (++item->frames < FIELD_HEIGHT - 1) {
				const int16_t topRow = item->delRow;
				item->delRow -= 2;
				for (int16_t i = 0; i < data->laserWidth; i++) {
					for (int16_t numDelRows = 0, row = topRow; numDelRows < 2; row--, numDelRows++) {
						MatrixBlock* matrixBlock = &itemPlayer->matrix[row * activatingPlayer->matrixWidth + data->laserColumns[i]];
						if (matrixBlock->block != NULLBLOCK && row > 0 && row < MATRIX_SINGLEWIDTH - 1) {
							ShowFieldBlockExplosion(itemPlayer, row, data->laserColumns[i]);
							matrixBlock->block = NULLBLOCK;
							matrixBlock->itemType = ITEMTYPE_NULL;
							PlaySoundEffect(SOUNDEFFECT_19);
						}
					}
				}
			}
			else {
				item->frames = 40;
				item->states[0]++;
			}
			break;

		case STATE_DELAYDEACTIVATE:
			if (--item->frames <= 0) {
				item->states[0]++;
			}
			break;

		case STATE_DEACTIVATE:
		default:
			for (int16_t i = 0; i < lengthoffield(Player, screenOffset); i++) {
				itemPlayer->screenOffset[i] = 0;
			}
			itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;
		}
	}
}

static int16_t LaserCenterColumn(Player* player) {
	if ((player->activeBlock & BLOCK_TYPE) == BLOCKTYPE_I && (player->activeRotation == ROTATION_RIGHT || player->activeRotation == ROTATION_LEFT)) {
		return player->activePos[0].integer + 1;
	}
	else {
		return player->activePos[0].integer;
	}
}
