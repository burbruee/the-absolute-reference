#include "Game/Play/Item/UpdateItemTransform.h"
#include "Game/Play/Item/ShowItemWarningTransform.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Sound/Sound.h"

enum TransformState {
	STATE_START,
	STATE_NEXT,
	STATE_INIT,
	STATE_TRANSFORM,
	STATE_DEACTIVATE
};

typedef struct TransformData {
	int16_t numStartBlocks;
} TransformData;

#define numTransformBlocks values[0]

// STATE_NEXT
#define frames values[2]

void UpdateItemTransform(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	TransformData* data = (TransformData*)item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_TRANSFORM;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_TRANSFORM);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				ShowItemWarningTransform(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				item->frames = 100;
				item->numTransformBlocks = 0;
				item->states[0]++;
			}
			break;

		case STATE_NEXT:
			if (--item->frames == 0) {
				itemPlayer->nextBlock |= BLOCK_TRANSFORM;
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			itemPlayer->play.flags &= PLAYFLAG_FORCEENTRY;
			itemPlayer->nowFlags &= NOW_NOGARBAGE;
			item->numTransformBlocks++;
			data->numStartBlocks = itemPlayer->numBlocks;
			item->states[0]++;
			break;

		case STATE_TRANSFORM:
			if (!(itemPlayer->nowFlags & NOW_NOGARBAGE)) {
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
			}

			if (item->numTransformBlocks <= 2 && (itemPlayer->nextBlock & (BLOCK_TRANSFORM | BLOCK_BIG | BLOCK_HARD | BLOCK_ROLLROLL)) != BLOCK_TRANSFORM) {
				itemPlayer->nextBlock |= BLOCK_TRANSFORM;
			}

			if (itemPlayer->play.state == PLAYSTATE_GARBAGECHECK && itemPlayer->numBlocks != data->numStartBlocks) {
				if (itemPlayer->numGarbageRows != 0) {
					itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
				}
				else if (item->numTransformBlocks < 3) {
					item->states[0] = STATE_INIT;
				}
				else {
					item->states[0]++;
				}
			}
			else {
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			}
			break;

		case STATE_DEACTIVATE:
		default:
			itemPlayer->play.flags &= PLAYFLAG_FORCEENTRY;
			if (!NoDisableGarbage(item)) {
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			}
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;
		}
	}
}
