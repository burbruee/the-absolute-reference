#include "Item.h"
#include "Player.h"
#include "Field.h"
#include "ShowBlockField.h"
#include "Video.h"
#include "Pal.h"
#include "Frame.h"

void RemoveItems(Player* player) {
	for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
		for (int16_t col = 1; col < MATRIX_SINGLEWIDTH; col++) {
			MATRIX(player, row, col).block &= ~BLOCK_ITEM;
		}
	}

	if (player->nextBlock & BLOCK_ITEM) {
		player->nextBlock &= BLOCK_ITEM;
		player->numItemBarBlocks = 0u;
	}
	player->nextBlockItemType = ITEMTYPE_NULL;
	ItemDescriptions[player->num] = ITEMTYPE_NULL;
}

bool CheckDeactivateItem(Item* item) {
	if (!(GameFlags & GAME_ITEMSNOUSE)) {
		Player* activatingPlayer = item->activatingPlayer;
		for (int16_t i = 0; i < lengthoffield(Player, fieldShift); i++) {
			activatingPlayer->fieldShift[i] = 0;
		}

		Player* itemPlayer = item->activatingPlayer->itemPlayer;
		for (int16_t row = 1; row < MATRIX_HEIGHT - 1; row++) {
			for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
				if (item->activatingPlayer->modeFlags & MODE_INVISIBLE) {
					MATRIX(itemPlayer, row, col).block &= BLOCK_INVISIBLE;
				}
				MATRIX(itemPlayer, row, col).brightness = 1;
			}
		}

		if (activatingPlayer->activeItemType != ITEMTYPE_GAMEOVER) {
			activatingPlayer->activeItemType = ITEMTYPE_NULL;
		}

		SetFieldBorderColor(activatingPlayer, ITEMTYPE_NULL);
		itemPlayer->nextBlock &= ~BLOCK_ROLLROLL;
		itemPlayer->nextBlock &= ~BLOCK_TRANSFORM;
		DeactivateItem(item);
		return true;
	}
	else {
		return false;
	}
}


bool FreeFall(Player* player) {
	bool fell = false;
	for (int16_t row = 1; row < MATRIX_HEIGHT; row++) {
		for (int16_t col = 1; col < MATRIX_SINGLEWIDTH - 1; col++) {
			if (MATRIX(player, row, col).block == NULLBLOCK) {
				for (int16_t fallRow = row + 1; fallRow < MATRIX_HEIGHT; fallRow++) {
					if (MATRIX(player, fallRow, col).block != NULLBLOCK) {
						MATRIX(player, row, col).block = MATRIX(player, fallRow, col).block;
						MATRIX(player, row, col).itemType = MATRIX(player, fallRow, col).itemType;
						MATRIX(player, fallRow, col).block = NULLBLOCK;
						MATRIX(player, fallRow, col).itemType = ITEMTYPE_NULL;
						fell = true;
						break;
					}
				}
			}
		}
	}
	return fell;
}

// Could be a leftover used for some unfinished item.
bool _0x6018584(Player* player) {
	int16_t row;
	for (row = 0; row < MATRIX_HEIGHT; row++) {
		if (MATRIX(player, row, 0).block != BLOCKTYPE_WALL && MATRIX(player, row, MATRIX_SINGLEWIDTH - 1).block != BLOCKTYPE_WALL) {
			return true;
		}
	}

	do {
		if (MATRIX(player, row + 0, 0).block != BLOCKTYPE_WALL) {
			return true;
		}
	} while (++row < MATRIX_HEIGHT);

	return false;
}

// When one item is good and the other is bad, or "item" is neutral, this
// returns true, indicating item->activatingPlayer->itemPlayer's garbage
// now-flag should remain unchanged. Otherwise, the garbage now-flag should be
// disabled on item->activatingPlayer->itemPlayer.
//
// It seems the intent is for garbage to be enabled when one player is
// disadvantaged/affected by multiple active items. The case of garbage being
// enabled when the item is neutral could be a bug, and the intent may have
// been for garbage to be disabled then.
//
// There's some logic in place for three or more active items, but that logic
// never applies in TAP.
ItemCategory NoDisableGarbage(Item* item) {
	int8_t numCategorizedItems[NUMPLAYERS]; // Number of items categorized, that is, either good or bad.
	int8_t* numCategorizedItems1p = &numCategorizedItems[PLAYER1];
	int8_t* numCategorizedItems2p = &numCategorizedItems[PLAYER2];
	ItemCategory itemCategories[NUMPLAYERS];
	ItemCategory* itemCategory1p = &itemCategories[PLAYER1];
	ItemCategory* itemCategory2p = &itemCategories[PLAYER2];
	ItemCategory itemCategory;

	numCategorizedItems[PLAYER2] = 0;
	numCategorizedItems[PLAYER1] = 0;
	*itemCategory2p = ITEMCAT_NEUTRAL;
	*itemCategory1p = ITEMCAT_NEUTRAL;

	itemCategory = GetItemCategory(item->type);
	if (itemCategory == ITEMCAT_NEUTRAL) {
		// BUG: This should return bool, but the developer forgot to provide a
		// return value when itemCategory is ITEMCAT_NEUTRAL, so it ends up
		// returning itemCategory in that case.
		return itemCategory;
	}

	Player* activatingPlayer = item->activatingPlayer;
	numCategorizedItems[activatingPlayer->num]++;
	itemCategories[activatingPlayer->num] = itemCategory;

	Item* currentItem = item;
	Item* nextItem = item;
	nextItem = nextItem->next;

	for (int16_t i = 0u; i < NUMITEMTYPES - 1; i++) {
		if (nextItem == NULL) {
			return false;
		}
		currentItem = nextItem;
		nextItem = currentItem->next;
		itemCategory = GetItemCategory(currentItem->type);
		if (itemCategory != ITEMCAT_NEUTRAL) {
			activatingPlayer = currentItem->activatingPlayer;
			numCategorizedItems[activatingPlayer->num]++;
			itemCategories[activatingPlayer->num] = itemCategory;
			if (
				(*numCategorizedItems1p != 1 || *numCategorizedItems2p != 1 || *itemCategory1p != *itemCategory2p) &&
				(numCategorizedItems[activatingPlayer->num] != 1 || activatingPlayer->activeItemType != currentItem->type) &&
				currentItem->type != ITEMTYPE_NULL) {
				return true;
			}
		}
	}
	return false;
}

static const ItemType GoodItemTypes[] = {
	ITEMTYPE_TOPDELFIELD,
	ITEMTYPE_BOTTOMDELFIELD,
	ITEMTYPE_RIGHTMOVFIELD,
	ITEMTYPE_LEFTMOVFIELD,
	ITEMTYPE_DELEVEN,
	ITEMTYPE_FREEFALL
};

static const ItemType BadItemTypes[] = {
	ITEMTYPE_DEATHBLOCK,
	ITEMTYPE_NEGAFIELD,
	ITEMTYPE_COLORBLOCK,
	ITEMTYPE_180DEGREEFIELD,
	ITEMTYPE_SHOTGUN,
	ITEMTYPE_HARDBLOCK,
	ITEMTYPE_LASER,
	ITEMTYPE_ROLLROLL,
	ITEMTYPE_TRANSFORM,
	ITEMTYPE_FREEFALL,
	ITEMTYPE_XRAY,
	ITEMTYPE_EXCHGFIELD,
	ITEMTYPE_MIRRORBLOCK,
	ITEMTYPE_DARKBLOCK
};

static const ItemType ConfusingItemTypes[] = {
	ITEMTYPE_COLORBLOCK,
	ITEMTYPE_XRAY,
	ITEMTYPE_MIRRORBLOCK
};

bool ItemGood(Player* player) {
	for (int16_t i = 0; i < lengthof(GoodItemTypes); i++) {
		if (player->activeItemType == GoodItemTypes[i]) {
			return true;
		}
	}
	return false;
}

bool ItemBad(Player* player) {
	for (int16_t i = 0; i < lengthof(BadItemTypes); i++) {
		if (player->activeItemType == BadItemTypes[i]) {
			return true;
		}
	}
	return false;
}

ItemCategory GetItemCategory(ItemType itemType) {
	int16_t good = 0;
	for (int16_t i = 0; i < lengthof(GoodItemTypes); i++) {
		if (itemType == GoodItemTypes[i]) {
			good = 1;
		}
	}

	int16_t bad = 0;
	for (int16_t i = 0; i < lengthof(BadItemTypes); i++) {
		if (itemType == BadItemTypes[i]) {
			bad = 1;
		}
	}

	return good + bad;
}

bool ItemConfusing(Player* player) {
	for (int16_t i = 0; i < lengthof(ConfusingItemTypes); i++) {
		if (player->activeItemType == ConfusingItemTypes[i]) {
			return true;
		}
	}
	return false;
}

void _0x601886C() {
	SetPal(180u, 16u, PALPTR(0x1E3));
	SetPal(196u, 1u, PALPTR(0x1F3));
}

#include "UpdateItemDeathBlock.h"
#include "UpdateItemNegaField.h"
#include "UpdateItemColorBlock.h"
#include "UpdateItemTopDelField.h"
#include "UpdateItemBottomDelField.h"
#include "UpdateItemRightMovField.h"
#include "UpdateItemLeftMovField.h"
#include "UpdateItemDelEven.h"
#include "UpdateItemFlipField.h"
#include "UpdateItemShotgun.h"
#include "UpdateItemHardBlock.h"
#include "UpdateItemLaser.h"
#include "UpdateItemRollRoll.h"
#include "UpdateItemTransform.h"
#include "UpdateItemFreeFall.h"
#include "UpdateItemXRay.h"
#include "UpdateItemExchgField.h"
#include "UpdateItemMirrorBlock.h"
#include "UpdateItemDarkBlock.h"
static void (*const ItemUpdaters[NUMITEMTYPES])(Item*) = {
	UpdateItemDeathBlock,
	UpdateItemNegaField,
	UpdateItemColorBlock,
	UpdateItemTopDelField,
	UpdateItemBottomDelField,
	UpdateItemRightMovField,
	UpdateItemLeftMovField,
	UpdateItemDelEven,
	UpdateItemFlipField,
	UpdateItemShotgun,
	UpdateItemHardBlock,
	UpdateItemLaser,
	UpdateItemRollRoll,
	UpdateItemTransform,
	UpdateItemFreeFall,
	UpdateItemXRay,
	UpdateItemExchgField,
	UpdateItemMirrorBlock,
	UpdateItemDarkBlock
};

#if 0
void UpdateVersusItems(Player* player) {
	if (player->modeFlags & MODE_VERSUS) {
		for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++) {
			if (VersusPlayers[playerNum].versusItemType >= ITEMTYPE_DEATHBLOCK && player->versusItemType <= ITEMTYPE_DARKBLOCK) {
				// NOTE: This line expects that item updaters take a player pointer, not an item pointer.
				ItemUpdaters[TOITEMNUM(player->versusItemType)](&VersusPlayers[playerNum]);
			}
		}
	}
}
#endif

static Item ItemHeap[6];
static uint16_t NumFreeItems;
static Item* ItemHeapHead;
static Item* ItemHeapTail;

void InitItems() {
	NumFreeItems = lengthof(ItemHeap);
	ItemHeapHead = NULL;
	ItemHeapTail = NULL;
	for (int16_t i = 0; i < lengthof(ItemHeap); i++) {
		ItemHeap[i].status = ITEMSTATUS_FREE;
		ItemHeap[i].type = ITEMTYPE_NULL;

		for (int16_t j = 0; j < lengthoffield(Item, values); j++) {
			ItemHeap[i].values[j] = 0;
		}

		for (int16_t j = 0; j < lengthoffield(Item, states); j++) {
			ItemHeap[i].states[j] = 0u;
		}

		uint8_t* itemData = (uint8_t*)ItemHeap[i].data;
		for (size_t j = 0; j < sizeoffield(Item, data); j++) {
			itemData[j] = 0u;
		}
	}
}

Item* AllocItem() {
	if (NumFreeItems != 0u) {
		Item* item = ItemHeap;
		for (int16_t i = 0; i < lengthof(ItemHeap); i++, item++) {
			if (item->status == ITEMSTATUS_FREE) {
				NumFreeItems--;
				if (ItemHeapHead == NULL) {
					ItemHeapHead = item;
					ItemHeapTail = item;
					item->previous = NULL;
					item->next = NULL;
					return item;
				}
				else {
					item->next = ItemHeapTail->next;
					ItemHeapTail->next = item;
					item->previous = ItemHeapTail;
					ItemHeapTail = item;
					return item;
				}
			}
		}
	}
	return NULL;
}

void DeactivateItem(Item* item) {
	item->status = ITEMSTATUS_INACTIVE;
}

void UpdateItems() {
	if (ItemHeapHead != NULL && CurrentPauseMode < PAUSEMODE_GAME) {
		for (Item* item = ItemHeapHead; item != NULL; item = item->next) {
			switch (item->status) {
			case ITEMSTATUS_ACTIVE:
				if (item->type >= ITEMTYPE_DEATHBLOCK && item->type <= ITEMTYPE_DARKBLOCK) {
					ItemUpdaters[TOITEMNUM(item->type)](item);
				}
				else {
					item->status = ITEMSTATUS_INACTIVE;
				}
				break;

			case ITEMSTATUS_INACTIVE:
			default:
				if (item->previous == NULL) {
					ItemHeapHead = item->next;
				}
				else {
					item->previous->next = item->next;
				}

				if (item->next == NULL) {
					ItemHeapTail = item->previous;
				}
				else {
					item->next->previous = item->previous;
				}

				item->status = ITEMSTATUS_FREE;
				item->type = ITEMTYPE_NULL;
				for (int16_t i = 0; i < lengthoffield(Item, values); i++) {
					item->values[i] = 0;
				}
				for (int16_t i = 0; i < lengthoffield(Item, states); i++) {
					item->states[i] = 0u;
				}
				NumFreeItems++;
				break;
			}
		}
	}
}
