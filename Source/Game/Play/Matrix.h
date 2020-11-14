#pragma once

#include "Game/Play/Block.h"
#include "Game/Play/Item/Item.h"
#include "Lib/Fixed.h"
#include <stdint.h>

// The field includes the blocks available for clears.
#define FIELD_SINGLEWIDTH 10
#define FIELD_DOUBLESWIDTH 14
#define FIELD_HEIGHT 20

// The matrix includes the blocks of the field and walls.
#define MATRIX_SINGLEWIDTH (1 + FIELD_SINGLEWIDTH + 1)
#define MATRIX_DOUBLESWIDTH (1 + FIELD_DOUBLESWIDTH + 1)
#define MATRIX_HEIGHT (1 + FIELD_HEIGHT + 1)

// Some code considers the middle region of the matrix without walls the field.
// Though row 21, the vanish row, is available for clears.
// So row 21 is both part of the field and part of the matrix's top wall.

// The number of blocks in matrices.
#define NUMMATRIXBLOCKS_SINGLE (MATRIX_SINGLEWIDTH * MATRIX_HEIGHT)
#define NUMMATRIXBLOCKS_DOUBLES (MATRIX_DOUBLESWIDTH * MATRIX_HEIGHT)
#define NUMMATRIXBLOCKS_SHARED (NUMMATRIXBLOCKS_SINGLE * NUMPLAYERS)

// The matrix positions of newly entered tetrominoes.
#define ENTRY_SINGLECOL 4
#define ENTRY_DOUBLES1PCOL 3
#define ENTRY_DOUBLES2PCOL 10
#define ENTRY_ROW (MATRIX_HEIGHT - 1)
// Top-most position within the entry row; using this causes entered
// tetrominoes to initially float in sub-1G gravity.
#define ENTRYPOS_Y (F32(ENTRY_ROW, 0xFFFF))

// A matrix is made of blocks of this type.
// This type of block is not for use with tetrominoes.
typedef struct MatrixBlock {
	Block block;
	int8_t brightness;
	ItemType itemType;
	int16_t visibleFrames;
} MatrixBlock;
