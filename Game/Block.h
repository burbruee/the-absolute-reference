#pragma once

#include <stdint.h>
#include <stdbool.h>

// - My interpretation of Arika's meaning of "block", along with some explanation of "square" -
// Tetrominoes are blocks.
// But the Block type is also used for matrix squares.
// So "block" doesn't necessarily indicate tetrominoes, but usually does when seen in identifiers.
// Though both tetrominoes and matrices are composed of squares.
// So matrices can be thought to be composed of blocks or squares.
// And "blocked" or "blocking" are used to describe blocks being obstructed, with "block(s)" never used for that meaning.

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
	BLOCKTYPE_GARBAGE
} BlockType;

#define TOBLOCKNUM(blockType) ((blockType) - 2u)
#define TOBLOCKTYPE(blockNum) ((blockNum) + 2u)
#define TOBLOCKNUMFLASHFRAMES(frames) (((frames) & 7) << 4)
