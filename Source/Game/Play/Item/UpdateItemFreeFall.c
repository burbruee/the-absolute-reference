#include "Game/Play/Item/UpdateItemFreeFall.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Game/Graphics/ShowEffect.h"
#include "Sound/Sound.h"
#include "Lib/Math.h"

enum FreeFallState {
	STATE_START,
	STATE_CHECKDELAY,
	STATE_DELAY,
	STATE_INITSLAM,
	STATE_DELAYSLAM,
	STATE_SLAM,
	STATE_RISE,
	STATE_NEXT,
	STATE_FREEFALL,
	STATE_DELAYGARBAGE,
	STATE_GARBAGE,
	STATE_DELAYCOLLAPSE,
	STATE_COLLAPSE,
	STATE_DEACTIVATE
};

typedef struct FreeFallData {
	bool waitForEntry;
	int16_t numSlams;
	int16_t numLines;
} FreeFallData;

#define slamFrames values[0]
#define frames values[1]

void UpdateItemFreeFall(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	int16_t numLines;
	FreeFallData* data = (FreeFallData*)item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			data->waitForEntry = false;
			data->numSlams = 0;
			data->numLines = 0;
		case STATE_CHECKDELAY:
			RemoveItems(activatingPlayer);
			if (!(activatingPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL) {
				if (ItemConfusing(itemPlayer)) {
					data->waitForEntry = true;
				}

				if (!ItemBad(itemPlayer)) {
					activatingPlayer->activeItemType = ITEMTYPE_FREEFALL;
					SetFieldBorderColor(activatingPlayer, ITEMTYPE_FREEFALL);
					if (GameFlags & GAME_VERSUS) {
						SetFieldBorderColor(itemPlayer, ITEMTYPE_FREEFALL);
					}
					activatingPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
					if (data->waitForEntry) {
						item->states[0]++;
					}
					else {
						activatingPlayer->nowFlags |= NOW_NOGARBAGE;
						item->states[0] += 2u;
					}
				}
			}
			break;

		case STATE_DELAY:
			if (activatingPlayer->play.state == PLAYSTATE_GARBAGECHECK) {
				activatingPlayer->nowFlags |= NOW_NOGARBAGE;
				item->states[0]++;
			}
			break;

		case STATE_INITSLAM:
			item->slamFrames = 30;
			item->states[0]++;
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(activatingPlayer, MATRIX_HEIGHT - 1, col).block = NULLBLOCK;
				MATRIX(activatingPlayer, MATRIX_HEIGHT - 1, col).itemType = ITEMTYPE_NULL;
			}
			break;

		case STATE_DELAYSLAM:
			if (--item->slamFrames == 0) {
				item->frames = 4;
				item->states[0]++;
			}
			break;

		case STATE_SLAM:
			activatingPlayer->screenOffset[1] -= 8;
			if (--item->frames == 0) {
				item->frames = 2;
				item->states[0]++;
			}
			break;

		case STATE_RISE:
			activatingPlayer->screenOffset[1] += 16;
			if (--item->frames == 0) {
				PlaySoundEffect(SOUNDEFFECT_COLLAPSE);
				item->states[0]++;
			}
			break;

		case STATE_NEXT:
			if (++data->numSlams >= 2) {
				activatingPlayer->screenOffset[1] = 0;
				item->frames = 0;
				item->states[0]++;
			}
			else {
				item->slamFrames = 10;
				item->states[0] = STATE_DELAYSLAM;
			}
			break;

		case STATE_FREEFALL:
			if (--item->frames == 0) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
						if (MATRIX(activatingPlayer, row, col).block == NULLBLOCK) {
							bool noDrop = false;
							int16_t dropRow = row;
							do {
								if (++dropRow > FIELD_HEIGHT) {
									noDrop = true;
								}
							} while (MATRIX(activatingPlayer, dropRow, col).block == NULLBLOCK);
							if (noDrop) {
								continue;
							}

							MATRIX(activatingPlayer, row, col) = MATRIX(activatingPlayer, dropRow, col);
							MATRIX(activatingPlayer, dropRow, col).block = NULLBLOCK;
							MATRIX(activatingPlayer, dropRow, col).itemType = ITEMTYPE_NULL;
						}
					}
				}

				item->states[0]++;
				item->frames = 25;
			}
			break;

		case STATE_DELAYGARBAGE:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_GARBAGE:
			FreeFall(activatingPlayer);
			for (numLines = 1; numLines < MATRIX_HEIGHT - 1; numLines++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					if ((MATRIX(activatingPlayer, numLines, col).block & BLOCK_TYPE) == BLOCKTYPE_EMPTY) {
						break;
					}
				}
			}
			numLines--;

			if ((GameFlags & GAME_VERSUS) || (activatingPlayer->modeFlags & MODE_ITEM)) {
				int16_t sentGarbageRows = 0;
				int16_t garbageHoleCol = (Rand(1192u) + 1) % 10;
				if (numLines != 0) {
					for (int16_t row = numLines; row != 0; row--) {
						ShowLineClear(activatingPlayer, row);
						for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
							if (itemPlayer->numGarbageRows + sentGarbageRows < GARBAGEHEIGHT) {
								if (col == garbageHoleCol) {
									itemPlayer->garbage[(itemPlayer->numGarbageRows + sentGarbageRows) * MATRIX_SINGLEWIDTH + col - 1] = NULLBLOCK;
								}
								else {
									itemPlayer->garbage[(itemPlayer->numGarbageRows + sentGarbageRows) * MATRIX_SINGLEWIDTH + col - 1] = MATRIX(activatingPlayer, row, col).block;
								}
							}

							MATRIX(activatingPlayer, row, col).block = NULLBLOCK;
							MATRIX(activatingPlayer, row, col).itemType = ITEMTYPE_NULL;
						}
					}
				}

				if (itemPlayer->numGarbageRows + sentGarbageRows > GARBAGEHEIGHT) {
					sentGarbageRows = 8 - itemPlayer->numGarbageRows;
				}
				if (sentGarbageRows > 1) {
					itemPlayer->numGarbageRows += sentGarbageRows;
					PlaySoundEffect(SOUNDEFFECT_CLEAR);
				}
			}
			else {
				if (numLines != 0) {
					for (int16_t row = numLines; row != 0; row--) {
						ShowLineClear(activatingPlayer, row);
						for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
							MATRIX(activatingPlayer, row, col).block = NULLBLOCK;
							MATRIX(activatingPlayer, row, col).itemType = ITEMTYPE_NULL;
						}
					}
				}
				if (numLines > 1) {
					PlaySoundEffect(SOUNDEFFECT_CLEAR);
				}
			}
			data->numLines = numLines;

			if (data->numLines != 0) {
				item->frames = 25;
				item->states[0]++;
			}
			else {
				item->frames = 25;
				item->states[0] = STATE_DEACTIVATE;
			}
			break;

		case STATE_DELAYCOLLAPSE:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_COLLAPSE:
			if (data->numLines != 0) {
				for (int16_t row = 1; row < MATRIX_HEIGHT - 1 - data->numLines; row++) {
					for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
						MATRIX(activatingPlayer, row, col) = MATRIX(activatingPlayer, row + data->numLines, col);
					}
				}
			}

			for (int16_t row = MATRIX_HEIGHT - 1 - data->numLines; row < MATRIX_HEIGHT - 1; row++) {
				for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
					MATRIX(activatingPlayer, row, col).block = NULLBLOCK;
					MATRIX(activatingPlayer, row, col).itemType = ITEMTYPE_NULL;
				}
			}

			item->frames = 40;
			item->states[0]++;
			break;

		case STATE_DEACTIVATE:
		default:
			if (--item->frames == 0) {
				data->waitForEntry = false;
				activatingPlayer->activeItemType = ITEMTYPE_NULL;
				SetFieldBorderColor(activatingPlayer, ITEMTYPE_NULL);
				if (GameFlags & GAME_VERSUS) {
					SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
				}
				activatingPlayer->screenOffset[1] = 0;
				activatingPlayer->nowFlags &= ~NOW_NOGARBAGE;
				activatingPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
				DeactivateItem(item);
			}
			break;
		}
	}
}
