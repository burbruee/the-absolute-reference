#include "Game/Play/Item/UpdateItemExchgField.h"
#include "Game/Play/Item/ShowItemWarningExchgField.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Game/Graphics/ShowColumnMarker.h"
#include "Sound/Sound.h"

enum ExchgFieldState {
	STATE_START,
	STATE_DELAY,
	STATE_EXCHGFIELD,
	STATE_DEACTIVATE
};

typedef struct ExchgFieldData {
	int32_t exchgFrames;
} ExchgFieldData;

// STATE_*
#define frames values[0]

// STATE_EXCHGFIELD
#define exchgCol values[1]

void UpdateItemExchgField(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	ExchgFieldData* data = item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer) && !ItemBad(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_EXCHGFIELD;
				SetFieldBorderColor(activatingPlayer, ITEMTYPE_EXCHGFIELD);
				SetFieldBorderColor(itemPlayer, ITEMTYPE_EXCHGFIELD);
				ShowItemWarningExchgField(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				activatingPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				activatingPlayer->nowFlags |= NOW_NOGARBAGE;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				data->exchgFrames = 0;
				item->frames = 60;
				item->exchgCol = 1;
				item->states[0]++;
			}
			break;

		case STATE_DELAY:
			if (--item->frames == 0) {
				item->states[0]++;
				PlaySoundEffect(SOUNDEFFECT_CLEAR);
			}
			break;

		case STATE_EXCHGFIELD:
			if (data->exchgFrames++ & 1) {
				ShowColumnMarker(itemPlayer, item->exchgCol);
				for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
					if (!(MATRIX(itemPlayer, row, item->exchgCol).block & BLOCK_ITEM)) {
						MATRIX(itemPlayer, row, item->exchgCol) = MATRIX(activatingPlayer, row, item->exchgCol);
					}
					MATRIX(activatingPlayer, row, item->exchgCol).block &= ~BLOCK_ITEM;
				}
				item->exchgCol++;
			}

			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				MATRIX(itemPlayer, MATRIX_HEIGHT - 1, col).block = NULLBLOCK;
				MATRIX(itemPlayer, MATRIX_HEIGHT - 1, col).itemType = ITEMTYPE_NULL;
			}

			if (item->exchgCol == MATRIX_SINGLEWIDTH - 1) {
				item->frames = 40;
				item->states[0]++;
			}
			break;

		case STATE_DEACTIVATE:
			if (--item->frames == 0) {
				activatingPlayer->nowFlags &= ~NOW_NOGARBAGE;
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
				itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
				activatingPlayer->activeItemType = ITEMTYPE_NULL;
				SetFieldBorderColor(activatingPlayer, ITEMTYPE_NULL);
				SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
				DeactivateItem(item);
			}
			break;

		default:
			break;
		}
	}
}