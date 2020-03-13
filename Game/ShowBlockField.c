#include "ShowBlockField.h"
#include "Temp.h"
#include "DisplayObject.h"
#include "Object.h"
#include "Video.h"
#include "Layer.h"
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

typedef enum BlockBorder {
	BLOCKBORDER_NONE = 0,
	BLOCKBORDER_TOP = 1,
	BLOCKBORDER_BOTTOM = 2,
	BLOCKBORDER_LEFT = 4,
	BLOCKBORDER_RIGHT = 8
} BlockBorder;

void ShowField(Player* player) {
	const int16_t x = player->screenPos[0] + player->screenOffset[0];
	const int16_t y = player->screenPos[1] + player->screenOffset[1];

	const ObjectData* fieldBorderObject;
	uint8_t palNum;
	if (GameFlags & GAME_DOUBLES) {
		fieldBorderObject = OBJECT_DOUBLESFIELDBORDER;
		palNum = 16u;
	}
	else {
		if (player->modeFlags & (MODE_MASTER|MODE_VERSUS)) {
			fieldBorderObject = OBJECT_MASTERFIELDBORDER;
		}
		else {
			fieldBorderObject = OBJECT_NORMALFIELDBORDER;
		}

		if (player->num == PLAYER1) {
			palNum = 16u;
		}
		else {
			palNum = 32u;
		}
	}
	DisplayObject(fieldBorderObject, y, x, palNum, LAYER_MATRIX);

	for (int16_t row = 0, displayY = y - ((player->matrixHeight - 1) * 8 + 6); row < player->matrixHeight; row++, displayY += 8) {
		for (int16_t col = 0, displayX = x - (player->matrixWidth / 2) * 8; col < player->matrixWidth; col++, displayX += 8) {
			Block block = MATRIX(player, player->matrixHeight - row - 1, col).block;
			if ((block & BLOCK_TYPE) && (block & BLOCK_TYPE) != BLOCKTYPE_WALL) {
				BlockBorder blockBorders = BLOCKBORDER_NONE;
				if (row != 0 && MATRIX(player, player->matrixHeight - row - 2, col).block == NULLBLOCK) {
					blockBorders = BLOCKBORDER_BOTTOM;
				}
				// NOTE: The check against 0 here was not in TAP, and had to be
				// added, to avoid out-of-bounds access of player 2's matrix.
				// The normal behavior for player 1 is for the top row to not
				// have any borders on the top, and the change enforces that
				// behavior for both players.
				if (row != 0 && row != player->matrixHeight - 1 && MATRIX(player, player->matrixHeight - row, col).block == NULLBLOCK) {
					blockBorders |= BLOCKBORDER_TOP;
				}
				if (col != 1 && MATRIX(player, player->matrixHeight - row - 1, col - 1).block == NULLBLOCK) {
					blockBorders |= BLOCKBORDER_LEFT;
				}
				if (col != player->matrixWidth - 2 && MATRIX(player, player->matrixHeight - row - 1, col + 1).block == NULLBLOCK) {
					blockBorders |= BLOCKBORDER_RIGHT;
				}
				const ObjectData* blockObject = &OBJECTTABLE_NORMALBLOCKS[0];
				if (player->itemPlayer->activeItemType != ITEMTYPE_COLORBLOCK && player->activeItemType != ITEMTYPE_GAMEOVER) {
					switch(blockBorders) {
					default: blockObject = &OBJECTTABLE_NORMALBLOCKS[0]; break;
					case BLOCKBORDER_LEFT: blockObject = &OBJECTTABLE_NORMALBLOCKS[1]; break;
					case BLOCKBORDER_TOP: blockObject = &OBJECTTABLE_NORMALBLOCKS[2]; break;
					case BLOCKBORDER_RIGHT: blockObject = &OBJECTTABLE_NORMALBLOCKS[3]; break;
					case BLOCKBORDER_BOTTOM: blockObject = &OBJECTTABLE_NORMALBLOCKS[4]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_LEFT: blockObject = &OBJECTTABLE_NORMALBLOCKS[5]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_RIGHT: blockObject = &OBJECTTABLE_NORMALBLOCKS[6]; break;
					case BLOCKBORDER_BOTTOM | BLOCKBORDER_RIGHT: blockObject = &OBJECTTABLE_NORMALBLOCKS[7]; break;
					case BLOCKBORDER_BOTTOM | BLOCKBORDER_LEFT: blockObject = &OBJECTTABLE_NORMALBLOCKS[8]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_LEFT | BLOCKBORDER_RIGHT: blockObject = &OBJECTTABLE_NORMALBLOCKS[9]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_BOTTOM | BLOCKBORDER_RIGHT: blockObject = &OBJECTTABLE_NORMALBLOCKS[10]; break;
					case BLOCKBORDER_BOTTOM | BLOCKBORDER_LEFT | BLOCKBORDER_RIGHT: blockObject = &OBJECTTABLE_NORMALBLOCKS[11]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_BOTTOM | BLOCKBORDER_LEFT: blockObject = &OBJECTTABLE_NORMALBLOCKS[12]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_BOTTOM: blockObject = &OBJECTTABLE_NORMALBLOCKS[13]; break;
					case BLOCKBORDER_LEFT | BLOCKBORDER_RIGHT: blockObject = &OBJECTTABLE_NORMALBLOCKS[14]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_BOTTOM | BLOCKBORDER_LEFT | BLOCKBORDER_RIGHT: blockObject = &OBJECTTABLE_NORMALBLOCKS[15]; break;
					}
				}

				if (block & BLOCK_ITEM) {
					blockObject = &OBJECTTABLE_ITEMBLOCKS[TOITEMNUM(MATRIX(player, player->matrixHeight - row - 1, col).itemType)];
				}

				if (!(block & BLOCK_FLASH)) {
					uint8_t blockPalNum = PalNumTableNormalBlocks[TOBLOCKNUM(block & BLOCK_TYPE)] + 5u;
					if (player->activeItemType == ITEMTYPE_GAMEOVER) {
						int8_t brightness = MATRIX(player, player->matrixHeight - row - 1, col).brightness;
						if (brightness > 5) {
							brightness = 5;
						}
						palNum = blockPalNum - brightness;
					}
					else {
						if (block & BLOCK_HARD) {
							blockObject = OBJECT_HARDBLOCK;
							blockPalNum = 137u;
						}
						if (block & BLOCK_ITEM) {
							blockPalNum = PalNumTableItemBlocks[TOITEMNUM(MATRIX(player, player->matrixHeight - row - 1, col).itemType)] + 5u;
						}
						palNum = blockPalNum;
						if (player->itemPlayer->activeItemType == ITEMTYPE_COLORBLOCK) {
							int8_t brightness = MATRIX(player, row, col).brightness;
							palNum = 137u;
							if (brightness > -1 && brightness < 9) {
								palNum = blockPalNum + MATRIX(player, row, col).brightness - 1;
							}
						}
					}
				}
				else {
					const uint8_t flashFrames = GETBLOCKFLASHFRAMES(block) - 1;
					if (flashFrames == 0) {
						block &= ~BLOCK_FLASH;
					}
					else {
						block = (block & ~BLOCK_FLASHFRAMES) | TOBLOCKFLASHFRAMES(flashFrames);
					}
					MATRIX(player, player->matrixHeight - row - 1, col).block = block;
					palNum = 137u;
				}
				if (!(block & BLOCK_INVISIBLE)) {
					DisplayObject(blockObject, displayY, displayX, palNum, LAYER_MATRIX);
				}
			}
		}
	}

	const ObjectData* fieldBgObject;
	if (GameFlags & GAME_DOUBLES) {
		fieldBgObject = OBJECT_DOUBLESFIELDBG;
	}
	else {
		fieldBgObject = OBJECT_SINGLEFIELDBG;
	}
	DisplayObject(fieldBgObject, y, x, 148u, LAYER_FIELDBG);
}

void ShowFieldPlus(Player* player) {
	const int16_t x = player->screenPos[0] + player->screenOffset[0];
	const int16_t y = player->screenPos[1] + player->screenOffset[1];

	const ObjectData* fieldBorderObject;
	uint8_t palNum;
	if (GameFlags & GAME_DOUBLES) {
		fieldBorderObject = OBJECT_DOUBLESFIELDBORDER;
		palNum = 16u;
	}
	else {
		fieldBorderObject = OBJECT_NORMALFIELDBORDER;
		if (!(player->modeFlags & MODE_TADEATH) && (player->modeFlags & (MODE_MASTER | MODE_VERSUS | MODE_TGMPLUS))) {
			fieldBorderObject = OBJECT_MASTERFIELDBORDER;
		}
		if ((player->nowFlags & NOW_SELECTING) && (player->modeFlags & MODE_DOUBLES)) {
			fieldBorderObject = OBJECT_MASTERFIELDBORDER;
		}
		if (player->num == PLAYER1) {
			palNum = 16u;
		}
		else {
			palNum = 32u;
		}
	}
	DisplayObject(fieldBorderObject, y, x, palNum, LAYER_MATRIX);

	ObjectData* blockObject = Temp;
	size_t numBlockObjects = 0u;
	for (int16_t row = 0, displayY = y - ((player->matrixHeight - 1) * 8 - 6); row < player->matrixHeight; row++, displayY += 8) {
		for (int16_t col = 0, displayX = x - (player->matrixWidth / 2) * 8; col < player->matrixWidth; col++, displayX += 8) {
			Block block = MATRIX(player, player->matrixHeight - row - 1, col).block;
			if ((block & BLOCK_TYPE) && (block & BLOCK_TYPE) != BLOCKTYPE_WALL) {
				BlockBorder blockBorders = BLOCKBORDER_NONE;
				if (row != 0 && MATRIX(player, player->matrixHeight - row - 2, col).block == NULLBLOCK) {
					blockBorders |= BLOCKBORDER_BOTTOM;
				}
				// NOTE: The check against 0 here was not in TAP, and had to be
				// added, to avoid out-of-bounds access of player 2's matrix.
				// The normal behavior for player 1 is for the top row to not
				// have any borders on the top, and the change enforces that
				// behavior for both players.
				if (row != 0 && row != player->matrixHeight - 1 && MATRIX(player, player->matrixHeight - row, col).block == NULLBLOCK) {
					blockBorders |= BLOCKBORDER_TOP;
				}
				if (col != 1 && MATRIX(player, player->matrixHeight - row - 1, col - 1).block == NULLBLOCK) {
					blockBorders |= BLOCKBORDER_LEFT;
				}
				if (col != player->matrixWidth - 2 && MATRIX(player, player->matrixHeight - row - 1, col + 1).block == NULLBLOCK) {
					blockBorders |= BLOCKBORDER_RIGHT;
				}
				const ObjectData* srcBlockObject = OBJECTTABLE_NORMALBLOCKS;
				if (player->itemPlayer->activeItemType != ITEMTYPE_COLORBLOCK && player->activeItemType != ITEMTYPE_GAMEOVER && !(block & BLOCK_FADING) && (player->nowFlags & NOW_PLAYING)) {
					switch(blockBorders) {
					default: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[0]; break;
					case BLOCKBORDER_LEFT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[1]; break;
					case BLOCKBORDER_TOP: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[2]; break;
					case BLOCKBORDER_RIGHT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[3]; break;
					case BLOCKBORDER_BOTTOM: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[4]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_LEFT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[5]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_RIGHT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[6]; break;
					case BLOCKBORDER_BOTTOM | BLOCKBORDER_RIGHT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[7]; break;
					case BLOCKBORDER_BOTTOM | BLOCKBORDER_LEFT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[8]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_LEFT | BLOCKBORDER_RIGHT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[9]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_BOTTOM | BLOCKBORDER_RIGHT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[10]; break;
					case BLOCKBORDER_BOTTOM | BLOCKBORDER_LEFT | BLOCKBORDER_RIGHT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[11]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_BOTTOM | BLOCKBORDER_LEFT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[12]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_BOTTOM: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[13]; break;
					case BLOCKBORDER_LEFT | BLOCKBORDER_RIGHT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[14]; break;
					case BLOCKBORDER_TOP | BLOCKBORDER_BOTTOM | BLOCKBORDER_LEFT | BLOCKBORDER_RIGHT: srcBlockObject = &OBJECTTABLE_NORMALBLOCKS[15]; break;
					}
				}
				if (block & BLOCK_ITEM) {
					srcBlockObject = &OBJECTTABLE_ITEMBLOCKS[TOITEMNUM(MATRIX(player, player->matrixHeight - row - 1, col).itemType)];
				}
				if (block & BLOCK_FLASH) {
					int16_t flashFrames = GETBLOCKFLASHFRAMES(block);
					if (flashFrames - 1 == 0) {
						block &= ~BLOCK_FLASH;
					}
					else {
						block = (block & BLOCK_FLASHFRAMES) | TOBLOCKFLASHFRAMES(flashFrames - 1);
					}
					MATRIX(player, player->matrixHeight - row - 1, col).block = block;
					palNum = 137u;
				}
				else {
					const uint8_t blockNum = TOBLOCKNUM(block);
					palNum = PalNumTableNormalBlocks[blockNum] + 5u;
					if (player->activeItemType == ITEMTYPE_GAMEOVER) {
						if (block & BLOCK_ITEM) {
							palNum = PalNumTableItemBlocks[TOITEMNUM(MATRIX(player, player->matrixHeight - row - 1, col).itemType)] + 5u;
						}
						int8_t brightness = MATRIX(player, player->matrixHeight - row - 1, col).brightness;
						if (brightness > 5) {
							brightness = 5;
						}
						palNum -= brightness;
					}
					else {
						if (block & BLOCK_HARD) {
							srcBlockObject = OBJECT_HARDBLOCK;
							palNum = 137u;
						}
						if (block & BLOCK_ITEM) {
							palNum = PalNumTableItemBlocks[TOITEMNUM(MATRIX(player, player->matrixHeight - row - 1, col).itemType) + 5u];
						}
						const ItemType itemType = player->itemPlayer->activeItemType;
						if (itemType == ITEMTYPE_COLORBLOCK || itemType == NUMITEMTYPES) {
							int8_t brightness = MATRIX(player, row, col).brightness;
							palNum = 137u;
							if (brightness > -1 && brightness < 9) {
								if (block & BLOCK_HARD) {
									palNum = 128u;
								}
								else if (block & BLOCK_ITEM) {
									palNum = PalNumTableItemBlocks[TOITEMNUM(MATRIX(player, player->matrixHeight - row - 1, col).itemType)];
								}
								else {
									palNum = PalNumTableNormalBlocks[blockNum];
								}
								palNum += MATRIX(player, row, col).brightness;
							}
						}
						else if (block & BLOCK_FADING) {
							if (MATRIX(player, player->matrixHeight - row - 1, col).visibleFrames == 0) {
								MATRIX(player, player->matrixHeight - row - 1, col).block = MATRIX(player, player->matrixHeight - row - 1, col).block | BLOCK_INVISIBLE;
								palNum = PalNumTableNormalBlocks[blockNum];
							}
							else if (--MATRIX(player, player->matrixHeight - row - 1, col).visibleFrames < 10) {
								palNum -= 5u - MATRIX(player, player->matrixHeight - row - 1, col).visibleFrames / 2;
							}
						}
					}
				}
				if ((!(block & BLOCK_FADING) || MATRIX(player, player->matrixHeight - row - 1, col).visibleFrames != 0) && !(block & BLOCK_INVISIBLE)) {
					for (size_t i = 0; i < sizeof(ObjectData) / sizeof(uint16_t); i++) {
						(*blockObject)[i] = (*srcBlockObject)[i];
					}
					// NOTE: This sets the object's X position to the block's X
					// position and the number of sprites to 1 for the object;
					// the setting of the number of sprites is unnecessary, as
					// the code modifies the sprite number field below to
					// display in 32-sprite chunks, but the setting is kept
					// here for documentation.
					(*blockObject)[1] = (displayX & 0x3FF) | 0x400;
					SPRITE_SETY(blockObject, displayY);
					SPRITE_SETPAL(blockObject, palNum);
					blockObject++;
					numBlockObjects++;
				}
			}
		}
	}

	ObjectData* blocksObject = Temp;
	while (numBlockObjects != 0u) {
		size_t stride = numBlockObjects < 32u ? numBlockObjects : 31u;
		(*blocksObject)[1] &= 0x3FF;
		OBJECT_SETNUMSPRITES(blocksObject, stride);
		DisplayObject(blocksObject, 0, 0, 0u, LAYER_MATRIX);
		blocksObject += stride;
		numBlockObjects -= stride;
	}

	const ObjectData* fieldBgObject;
	if (GameFlags & GAME_DOUBLES) {
		fieldBgObject = OBJECT_DOUBLESFIELDBG;
	}
	else {
		fieldBgObject = OBJECT_SINGLEFIELDBG;
	}
	DisplayObject(fieldBgObject, y, x, 148u, LAYER_FIELDBG);
}
