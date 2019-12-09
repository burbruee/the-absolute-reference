#include "UpdateItemDeathBlock.h"
#include "Player.h"
#include "ShowBlockField.h"
#include "ShowItemWarningDeathBlock.h"
#include "Sound.h"

enum DeathBlockState {
	STATE_START,
	STATE_SEND,
	STATE_NEXT,
	STATE_DEACTIVATE
};

#define sendFrames values[0]

void UpdateItemDeathBlock(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;
	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			if (
				!(itemPlayer->nowFlags & NOW_LOCKING) &&
				!(activatingPlayer->activeItemType == ITEMTYPE_NULL) &&
				!ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_DEATHBLOCK;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_DEATHBLOCK);
				ShowItemWarningDeathBlock(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);

				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				itemPlayer->sendFrames = 98u;
			}

		case STATE_NEXT:
			item->states[0]++;
			break;

		case STATE_SEND:
			if (item->sendFrames == 30u) {
				itemPlayer->nextBlock |= BLOCK_BIG;
			}
			if (--item->sendFrames == 0u) {
				item->states[0]++;
			}
			break;

		case STATE_DEACTIVATE:
			itemPlayer->itemMiscFlags &= ~ITEMMISC_DEATHBLOCK;
			itemPlayer->play.flags &= ~PLAYFLAG_FORCEENTRY;
			itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			itemPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;

		default:
			break;
		}
	}
}
