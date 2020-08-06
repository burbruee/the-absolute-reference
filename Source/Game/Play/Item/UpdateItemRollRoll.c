#include "Game/Play/Item/UpdateItemRollRoll.h"
#include "Game/Play/Item/ShowItemWarningRollRoll.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Sound/Sound.h"

enum RollRollState {
	STATE_START,
	STATE_NEXT,
	STATE_INIT,
	STATE_ROLLROLL,
	STATE_DEACTIVATE
};

typedef struct RollRollData {
	int16_t rollRollDelay;
	int16_t numStartBlocks;
} RollRollData;

// STATE_DELAY
#define frames values[2]

// STATE_ROLLROLL
#define rollRollFrames values[0]
#define numBlocks values[1]

void UpdateItemRollRoll(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	RollRollData* data = item->data;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_ROLLROLL;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_ROLLROLL);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				ShowItemWarningRollRoll(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				item->frames = 100;
				item->numBlocks = 0;
				item->states[0]++;
			}
			break;

		case STATE_NEXT:
			if (--item->frames == 0) {
				item->states[0]++;
			}
			break;

		case STATE_INIT:
			if (item->numBlocks < 3 && (itemPlayer->nextBlock & (BLOCK_TRANSFORM | BLOCK_BIG | BLOCK_HARD | BLOCK_ROLLROLL)) != BLOCK_ROLLROLL) {
				itemPlayer->nextBlock |= BLOCK_ROLLROLL;
			}

			itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			item->numBlocks++;
			data->numStartBlocks = itemPlayer->numBlocks;
			item->rollRollFrames = 0;
			data->rollRollDelay = 30;
			item->states[0]++;
			break;

		case STATE_ROLLROLL:
			if (!(itemPlayer->nowFlags & NOW_NOGARBAGE)) {
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
			}

			if (item->numBlocks <= 2 && (itemPlayer->nextBlock & (BLOCK_TRANSFORM | BLOCK_BIG | BLOCK_HARD | BLOCK_ROLLROLL)) != BLOCK_ROLLROLL) {
				itemPlayer->nextBlock |= BLOCK_ROLLROLL;
			}

			if (item->rollRollFrames % data->rollRollDelay == 0) {
				itemPlayer->itemMiscFlags |= ITEMMISC_ROTATE;
				item->rollRollFrames = 0;
			}
			item->rollRollFrames++;

			if (itemPlayer->play.state == PLAYSTATE_GARBAGECHECK && itemPlayer->numBlocks != data->numStartBlocks && itemPlayer->numGarbageRows == 0) {
				if (item->numBlocks < 3) {
					item->states[0] = STATE_INIT;
				}
				else {
					item->states[0]++;
				}
			}
			else {
				itemPlayer->nowFlags &= NOW_NOGARBAGE;
			}
			break;

		case STATE_DEACTIVATE:
			itemPlayer->itemMiscFlags &= ~ITEMMISC_ROTATE;
			itemPlayer->play.flags &= PLAYFLAG_FORCEENTRY;
			if (!NoDisableGarbage(itemPlayer)) {
				itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			}
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;

		default:
			break;
		}
	}
}