#include "SpriteInit.h"
#include "Video.h"
#include "LibC.h"

// Cases 0 and 1 appear to initialize the first two sprites with some settings.
// Case 2 adds an unscaled sprite (similar to DisplayObject).
// Case 3 does nothing.
// TODO: Case 4 appears to be like DisplayObjectEx, as it supports scaling inputs.
// TODO: Looks like a good name could be "SetSprite(SpriteSetType type, ???)".
// This appears to be a Psikyo-written function for writing sprites.
void _0x602DA0C(uint8_t arg0, struct_0x606006C* arg1) {
	switch (arg0) {
	case 0u:
		NumSprites = SPRITE_FIRST;
		_0x6061932.tempSprite[0] = 0xFFF0u;
		_0x6061932.tempSprite[1] = 0xFFF0u;
		_0x6061932.tempSprite[2] = 0x003Fu;
		_0x6061932.tempSprite[3] = 0x003Fu;
		_0x6061932.tempSprite[4] = 0x0000u;
		_0x6061932.tempSprite[5] = 0x0000u;
		Sprites[0][0] = 0xFFF0u;
		Sprites[0][1] = 0xFFF0u;
		Sprites[0][2] = 0x003Fu;
		Sprites[0][3] = 0x003Fu;
		Sprites[0][4] = 0x0000u;
		Sprites[0][5] = 0x0000u;
		Sprites[0][6] = _0x6061932.tempSprite[6];
		Sprites[0][7] = _0x6061932.tempSprite[7];
		Sprites[1][0] = 0xFFF0u;
		Sprites[1][1] = 0xFFF0u;
		Sprites[1][2] = 0x003Fu;
		Sprites[1][3] = 0x003Fu;
		Sprites[1][4] = 0x0000u;
		Sprites[1][5] = 0x0000u;
		Sprites[1][6] = _0x6061932.tempSprite[6];
		Sprites[1][7] = _0x6061932.tempSprite[7];
		return;

	case 1u:
		NumSprites = SPRITE_FIRST;
		_0x6061932.tempSprite[0] = 0x0000u;
		_0x6061932.tempSprite[1] = 0x0200u;
		_0x6061932.tempSprite[2] = 0x0D3Fu;
		_0x6061932.tempSprite[3] = 0x313Fu;
		_0x6061932.tempSprite[4] = 0x0000u;
		_0x6061932.tempSprite[5] = 0x0003u;
		Sprites[0][0] = 0x01C0u;
		Sprites[0][1] = 0x0200u;
		Sprites[0][2] = 0x003Fu;
		Sprites[0][3] = 0x003Fu;
		Sprites[0][4] = 0x0000u;
		Sprites[0][5] = 0x0003u;
		Sprites[0][6] = _0x6061932.tempSprite[6];
		Sprites[0][7] = _0x6061932.tempSprite[7];
		Sprites[1][0] = 0x0000u;
		Sprites[1][1] = 0x0200u;
		Sprites[1][2] = 0x0D3Fu;
		Sprites[1][3] = 0x313Fu;
		Sprites[1][4] = 0x0000u;
		Sprites[1][5] = 0x0003u;
		Sprites[1][6] = _0x6061932.tempSprite[6];
		Sprites[1][7] = _0x6061932.tempSprite[7];
		return;
	case 2u:
		_0x6061932.tempSprite[0] = arg1->y;
		_0x6061932.tempSprite[1] = arg1->x;
		// Set sprite's vertical flip, sprite priority, height, and height scale.
		_0x6061932.tempSprite[2] = (arg1->objectTable[0][2] & 0x3F00u) | UNSCALED;
		// Set sprite's horizontal flip, background priority, width, and width scale.
		_0x6061932.tempSprite[3] = (arg1->flipXBgPri << 12) | (arg1->objectTable[0][3] & 0xFF00u) | UNSCALED;
		// Set sprite's palette, BPP, alpha, and upper 3 tile number bits.
		_0x6061932.tempSprite[4] = (arg1->palNum << 8) | (arg1->objectTable[0][4] & 0xFFu);
		// Set sprite's lower 16 tile number bits.
		_0x6061932.tempSprite[5] = arg1->objectTable[0][5];
		// Add sprite.
		for (size_t i = 0u; i < SPRITE_LENGTH; i++) {
			Sprites[NumSprites][i] = _0x6061932.tempSprite[i];
		}
		NumSprites++;
		return;

	case 3u:
		return;

	case 4u:
		_0x6061932.tempSprite[0] = arg1->y1;
		_0x6061932.tempSprite[1] = arg1->x1;
		// Set sprite's vertical flip, unknown word 2 bit, sprite priority, height, and height scale.
		_0x6061932.tempSprite[2] = (arg1->objectTable[0][2] & 0x4F00u) | (arg1->flipYSprPri << 8) | arg1->scaleY;
		// TODO
		return;

		// TODO: Cases up to 11u.
	default:
		// BUG: Original didn't have a default case, and would have undefined
		// behavior. Fixed that here.
		return;
	}
}
