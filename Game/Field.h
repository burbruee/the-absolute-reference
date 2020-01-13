#pragma once

#include "Block.h"
#include "Item.h"
#include "Fixed.h"
#include "HwInput.h"
#include <stdint.h>

// The field includes the squares available for clears.
#define FIELD_SINGLEWIDTH 10
#define FIELD_DOUBLESWIDTH 14
#define FIELD_HEIGHT 20

// The matrix includes the squares of the field and walls.
#define MATRIX_SINGLEWIDTH (1 + FIELD_SINGLEWIDTH + 1)
#define MATRIX_DOUBLESWIDTH (1 + FIELD_DOUBLESWIDTH + 1)
#define MATRIX_HEIGHT (1 + FIELD_HEIGHT + 1)

// Some code considers the middle region of the matrix without walls the field.
// Though row 21, the vanish row, is available for clears.
// So row 21 is both part of the field and part of the matrix's top wall.

// The number of squares in matrices.
#define NUMSQUARES_SINGLE (MATRIX_SINGLEWIDTH * MATRIX_HEIGHT)
#define NUMSQUARES_DOUBLES (MATRIX_DOUBLESWIDTH * MATRIX_HEIGHT)
#define NUMSQUARES_SHARED (NUMSQUARES_SINGLE * NUMPLAYERS)

// The matrix positions of newly entered blocks.
#define ENTRY_SINGLECOL 4
#define ENTRY_1PDOUBLESCOL 3
#define ENTRY_2PDOUBLESCOL 10
#define ENTRY_ROW (MATRIX_HEIGHT - 1)
// Top-most position within the entry row; using this causes entered blocks to
// initially float in sub-1G gravity.
#define ENTRYPOS_Y (F16(ENTRY_ROW, 0xFFFF))

// A matrix is made of this type of square.
// This type of square is not for use with blocks.
typedef struct Square {
	Block block;
	int8_t brightness;
	ItemType itemType;
	int16_t visibleFrames;
} Square;
