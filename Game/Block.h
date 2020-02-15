#pragma once

#include <stdint.h>
#include <stdbool.h>

// - My interpretation of Arika's meaning of "block" -
// Tetrominoes are blocks. But individual squares that compose a tetromino or a
// matrix/field are also blocks. So "block" is a bit ambiguous, and its exact
// meaning is context sensitive. But "matrix block" always means a block of a
// matrix or field.

#define NULLBLOCK 0x0000
typedef enum Block {
	BLOCK_TYPE = 0xF << 0,
	BLOCK_NUMFLASHFRAMES = 7 << 4,
	BLOCK_FLASH = 1 << 7,
	BLOCK_TRANSFORM = 1 << 8,
	BLOCK_BIG = 1 << 9,
	BLOCK_HARD = 1 << 10,
	BLOCK_ROLLROLL = 1 << 11,
	BLOCK_FADING = 1 << 12,
	BLOCK_ITEM = 1 << 13,
	BLOCK_INVISIBLE = 1 << 14,
	BLOCK_BLOCKING = 1 << 15
} Block;

// There can only be 16 block types.
typedef enum BlockType {
	BLOCKTYPE_EMPTY,
	BLOCKTYPE_WALL,
	BLOCKTYPE_I,
	BLOCKTYPE_Z,
	BLOCKTYPE_S,
	BLOCKTYPE_J,
	BLOCKTYPE_L,
	BLOCKTYPE_O,
	BLOCKTYPE_T,
	BLOCKTYPE_GARBAGE,
	NUMBLOCKTYPES
} BlockType;

#define TOBLOCKNUM(blockType) ((blockType) - 2u)
#define TOBLOCKTYPE(blockNum) ((blockNum) + 2u)
#define TOBLOCKNUMFLASHFRAMES(frames) (((frames) & 7) << 4)
