#include "ShowBlockField.h"
#include "DisplayObject.h"
#include "Object.h"
#include "Video.h"
#include "Pal.h"
#include "Math.h"

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

void ShowBlock(Player* player, ShowBlockType showBlockType, bool show) {
	if (show) {
		Block block;
		Rotation rotation;
		ObjectData* blockObject;
		int16_t startY, startX;
		int16_t palNum;

		if (showBlockType == SHOWBLOCKTYPE_NEXT) {
			block = player->play.state == PLAYSTATE_BLOCKEDENTRY ? player->activeBlock : player->nextBlock;
			if ((GameFlags & GAME_VERSUS) && player->play.state == PLAYSTATE_START) {
				rotation = player->activeRotation;
			}
			else {
				rotation = ROTATION_ENTRY;
			}

			int16_t col;
			if (GameFlags & GAME_DOUBLES) {
				col = player->num == PLAYER1 ? ENTRY_DOUBLES1PCOL : ENTRY_DOUBLES2PCOL;
			}
			else {
				col = ENTRY_SINGLECOL;
			}

			startY = 0;
			if (block & BLOCK_BIG) {
				startY = 16;
				col--;
			}

			startX = player->screenPos[0] + player->screenOffset[0] + col * 8 - (player->matrixWidth / 2) * 8;
			startY = player->screenPos[1] + player->screenPos[1] - (player->matrixHeight + 3) * 8 - startY;
			palNum = 9;
		}
		else {
			block = player->activeBlock;
			rotation = player->activeRotation;
			int16_t col = F32I(player->activePos[0]), row = F32I(player->activePos[1]);
			if (block & BLOCK_BIG) {
				row++;
				col -= 2;
			}
			startX = player->screenPos[0] + player->screenOffset[0] + col * 8 - (player->matrixWidth / 2) * 8;
			startY = player->screenPos[1] + player->screenOffset[1] + (player->matrixHeight - row - 2) * 8 - (player->matrixHeight - 2) * 8 - 6;

			palNum = player->lockFrames / 6;
			if (player->lockFrames % 6 > 0) {
				palNum++;
			}
			if (palNum <= 0) {
				palNum = 0;
			}
			else if (palNum >= 4) {
				palNum = 4;
			}
			palNum += 4;
		}

		if (showBlockType == SHOWBLOCKTYPE_TLS) {
			block = player->activeBlock;
			rotation = player->activeRotation;
			int16_t col = F32I(player->activePos[0]), row = StepGravity(player, F32(20, 0x0000));
			if (block & BLOCK_BIG) {
				row++;
				col -= 2;
			}
			startX = player->screenPos[0] + player->screenOffset[0] + col * 8 - (player->matrixWidth / 2) * 8;
			startY = player->screenPos[1] + player->screenOffset[1] + (player->matrixHeight - row - 2) * 8 - (player->matrixHeight - 2) * 8 - 6;
			palNum = 4;
		}

		if (block & BLOCK_HARD) {
			blockObject = OBJECT_HARDBLOCK;
		}
		else if (block & BLOCK_ITEM) {
			uint8_t itemNum = showBlockType == SHOWBLOCKTYPE_NEXT ? TOITEMNUM(player->nextBlockItemType) : TOITEMNUM(player->activeBlockItemType);
			blockObject = &OBJECTTABLE_ITEMBLOCKS[itemNum];
		}
		else {
			blockObject = &OBJECTTABLE_NORMALBLOCKS[0];
		}

		int16_t palOffset;
		if (block & BLOCK_ITEM) {
			uint8_t itemNum = showBlockType == SHOWBLOCKTYPE_NEXT ? TOPALNUM(player->nextBlockItemType) : TOPALNUM(player->activeBlockItemType);
			palOffset = PalNumTableItemBlocks[itemNum];
		}
		else if (block & BLOCK_ROLLROLL) {
			palOffset = 48;
		}
		else if (block & BLOCK_HARD) {
			palOffset = 128;
		}
		else {
			size_t blockNum;
			if (block & BLOCK_TRANSFORM) {
				blockNum = Rand(7u);
			}
			else {
				blockNum = TOBLOCKNUM(block & BLOCK_TYPE);
			}
			palOffset = PalNumTableNormalBlocks[blockNum];
		}
		palNum += palOffset;

		int16_t blockSize;
		SpriteScale scale;
		if (block & BLOCK_BIG) {
			blockSize = 2;
			scale = SPRITESCALE(0x40);
		}
		else {
			blockSize = 1;
			scale = UNSCALED;
		}

		BlockDefSquare* blockDef = BLOCKDEF(block & BLOCK_TYPE);
		int16_t displaySize = blockSize * 8;
		for (int16_t row = 0, y = startY; row < 4; row++, y += displaySize) {
			BlockDefSquare* blockDefRow = BLOCKDEFROW(blockDef, rotation, row);
			for (int16_t col = 0, x = startX; col < 4; col++, x += displaySize) {
				if (blockDefRow[col] != BLOCKDEFSQUARE_EMPTY) {
					DisplayObjectEx(blockObject, y, x, palNum, 100u, scale, scale, false);
				}
			}
		}
	}
}

void ShowField(Player* player);

void ShowFieldPlus(Player *player);
