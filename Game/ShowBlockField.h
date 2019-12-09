#pragma once

#include "Player.h"
#include "Item.h"
#include <stdbool.h>

extern const uint16_t PalNumTableNormalBlocks[11];

void SetFieldBorderColor(Player* player, ItemType itemType);

typedef enum ShowBlockType {
	SHOWBLOCKTYPE_ACTIVE,
	SHOWBLOCKTYPE_NEXT,
	SHOWBLOCKTYPE_TLS
} ShowBlockType;
void ShowBlock(Player* player, ShowBlockType showBlockType, bool show);

void ShowField(Player* player);

void ShowFieldPlus(Player *player);
