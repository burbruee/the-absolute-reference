#include "ShowBlockField.h"
#include "Video.h"
#include "Pal.h"

const uint16_t PalNumTableNormalBlocks[NUMBLOCKTYPES] = { 58u, 68u, 78u, 88u, 98u, 108u, 118u, 48u, 128u, 138u };
const uint16_t PalNumTableItemBlocks[NUMITEMTYPES] = { 226u, 226u, 226u, 236u, 236u, 236u, 236u, 236u, 226u, 226u, 226u, 226u, 226u, 226u, 246u, 226u, 246u, 226u, 226u };
const Color* PalTableItemFieldBorder[NUMITEMTYPES] = {
	PAL_REDFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_BLUEFIELDBORDER,
	PAL_BLUEFIELDBORDER,
	PAL_BLUEFIELDBORDER,
	PAL_BLUEFIELDBORDER,
	PAL_BLUEFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_GREENFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_GREENFIELDBORDER,
	PAL_REDFIELDBORDER,
	PAL_REDFIELDBORDER,
};

void SetFieldBorderColor(Player* player, ItemType itemType) {
	uint8_t palNum = player->num == PLAYER1 ? 16u : 32u;

	if (itemType != ITEMTYPE_NULL) {
		SetPal(palNum, 16u, PalTableItemFieldBorder[itemType]);
	}
	else if (GameFlags & GAME_DOUBLES) {
		SetPal(palNum, 16u, PAL_DOUBLESFIELDBORDER);
	}
	else if (player->modeFlags & MODE_TGMPLUS) {
		SetPal(palNum, 16u, PAL_BLUEFIELDBORDER);
	}
	else if (player->modeFlags & MODE_TADEATH) {
		SetPal(palNum, 16u, PAL_REDFIELDBORDER);
	}
	else if (player->modeFlags & (MODE_MASTER | MODE_VERSUS)) {
		SetPal(palNum, 16u, PAL_WHITEFIELDBORDER);
	}
	else if (player->modeFlags & MODE_NORMAL) {
		SetPal(palNum, 16u, PAL_NORMALFIELDBORDER);
	}
}

void ShowBlock(Player* player, ShowBlockType showBlockType, bool show);

void ShowField(Player* player);

void ShowFieldPlus(Player *player);
