#include "Game/Play/Item/UpdateItemHardBlock.h"
#include "Game/Play/Item/ShowItemWarningHardBlock.h"
#include "Game/Play/Player.h"
#include "Game/Graphics/ShowBlockField.h"
#include "Sound/Sound.h"

enum HardBlockState {
	STATE_START,
	STATE_HARDBLOCK,
	STATE_DELAYDEACTIVATE,
	STATE_DEACTIVATE
};

// STATE_HARDBLOCK
#define frames values[0]

void UpdateItemHardBlock(Item* item) {
	Player* activatingPlayer = item->activatingPlayer;
	Player* itemPlayer = activatingPlayer->itemPlayer;

	if (!CheckDeactivateItem(item)) {
		switch (item->states[0]) {
		case STATE_START:
			RemoveItems(activatingPlayer);
			if (!(itemPlayer->nowFlags & NOW_LOCKING) && activatingPlayer->activeItemType == ITEMTYPE_NULL && !ItemGood(itemPlayer)) {
				activatingPlayer->activeItemType = ITEMTYPE_HARDBLOCK;
				SetFieldBorderColor(itemPlayer, ITEMTYPE_HARDBLOCK);
				ShowItemWarningHardBlock(itemPlayer);
				PlaySoundEffect(SOUNDEFFECT_BADITEM);
				itemPlayer->play.flags |= PLAYFLAG_FORCEENTRY;
				itemPlayer->nowFlags |= NOW_NOGARBAGE;
				item->frames = 99;
				itemPlayer->subStates[0]++;
			}
			break;

		case STATE_HARDBLOCK:
			if (item->frames == 30) {
				itemPlayer->nextBlock |= BLOCK_HARD;
			}

			if (--item->frames <= 0) {
				item->states[0]++;
			}
			break;

		case STATE_DELAYDEACTIVATE:
			item->states[0]++;
			break;

		case STATE_DEACTIVATE:
			itemPlayer->itemMiscFlags &= ~ITEMMISC_BIT10; // TODO: BIT10 is HARDBLOCK?
			itemPlayer->nowFlags &= ~NOW_NOGARBAGE;
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
			SetFieldBorderColor(itemPlayer, ITEMTYPE_NULL);
			DeactivateItem(item);
			break;

		default:
			break;
		}
	}
}