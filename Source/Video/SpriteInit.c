#include "Video/SpriteInit.h"
#include "Video/Video.h"
#include "Lib/LibC.h"

// Cases 0 and 1 appear to initialize the first two sprites with some settings.
// Case 2 adds an unscaled sprite (similar to DisplayObject).
// Case 3 does nothing.
// Case 4 appears to be like DisplayObjectEx, as it supports scaling inputs.
// Case 5 appears similar to case 4, with some differences in its inputs.
void AddSprite(AddSpriteType type, AddSpriteData* data) {
	const ObjectData* object = data->data;
	switch (type) {
	case ADDSPRITE_0:
		NumSprites = SPRITE_FIRST;
		UNK_6061932.tempSprite[0] = 0xFFF0u;
		UNK_6061932.tempSprite[1] = 0xFFF0u;
		UNK_6061932.tempSprite[2] = 0x003Fu;
		UNK_6061932.tempSprite[3] = 0x003Fu;
		UNK_6061932.tempSprite[4] = 0x0000u;
		UNK_6061932.tempSprite[5] = 0x0000u;
		Sprites[0][0] = 0xFFF0u;
		Sprites[0][1] = 0xFFF0u;
		Sprites[0][2] = 0x003Fu;
		Sprites[0][3] = 0x003Fu;
		Sprites[0][4] = 0x0000u;
		Sprites[0][5] = 0x0000u;
		Sprites[0][6] = UNK_6061932.tempSprite[6];
		Sprites[0][7] = UNK_6061932.tempSprite[7];
		Sprites[1][0] = 0xFFF0u;
		Sprites[1][1] = 0xFFF0u;
		Sprites[1][2] = 0x003Fu;
		Sprites[1][3] = 0x003Fu;
		Sprites[1][4] = 0x0000u;
		Sprites[1][5] = 0x0000u;
		Sprites[1][6] = UNK_6061932.tempSprite[6];
		Sprites[1][7] = UNK_6061932.tempSprite[7];
		return;

	case ADDSPRITE_1:
		NumSprites = SPRITE_FIRST;
		UNK_6061932.tempSprite[0] = 0x0000u;
		UNK_6061932.tempSprite[1] = 0x0200u;
		UNK_6061932.tempSprite[2] = 0x0D3Fu;
		UNK_6061932.tempSprite[3] = 0x313Fu;
		UNK_6061932.tempSprite[4] = 0x0000u;
		UNK_6061932.tempSprite[5] = 0x0003u;
		Sprites[0][0] = 0x01C0u;
		Sprites[0][1] = 0x0200u;
		Sprites[0][2] = 0x003Fu;
		Sprites[0][3] = 0x003Fu;
		Sprites[0][4] = 0x0000u;
		Sprites[0][5] = 0x0003u;
		Sprites[0][6] = UNK_6061932.tempSprite[6];
		Sprites[0][7] = UNK_6061932.tempSprite[7];
		Sprites[1][0] = 0x0000u;
		Sprites[1][1] = 0x0200u;
		Sprites[1][2] = 0x0D3Fu;
		Sprites[1][3] = 0x313Fu;
		Sprites[1][4] = 0x0000u;
		Sprites[1][5] = 0x0003u;
		Sprites[1][6] = UNK_6061932.tempSprite[6];
		Sprites[1][7] = UNK_6061932.tempSprite[7];
		return;
	case ADDSPRITE_2:
		UNK_6061932.tempSprite[0] = data->y;
		UNK_6061932.tempSprite[1] = data->x;
		// Set sprite's vertical flip, sprite priority, height, and height scale.
		UNK_6061932.tempSprite[2] = ((*object)[2] & ~OBJECT_SCALEY) | UNSCALED;
		// Set sprite's horizontal flip, background priority, width, and width scale.
		UNK_6061932.tempSprite[3] = (data->flipXBgPri << 12) | ((*object)[3] & ~OBJECT_SCALEX) | UNSCALED;
		// Set sprite's palette, BPP, alpha, and upper 3 tile number bits.
		UNK_6061932.tempSprite[4] = OBJECT_TOPALNUM(data->palNum) | ((*object)[4] & ~OBJECT_PALNUM);
		// Set sprite's lower 16 tile number bits.
		UNK_6061932.tempSprite[5] = (*object)[5];
		// Add sprite.
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;

	case ADDSPRITE_3:
		return;

	case ADDSPRITE_4:
		UNK_6061932.tempSprite[0] = data->y1;
		UNK_6061932.tempSprite[1] = data->x1;
		// Set sprite's vertical flip, unknown word 2 bit, sprite priority, height, and height scale.
		UNK_6061932.tempSprite[2] =
			((*object)[2] & (OBJECT_UNKNOWN0 | OBJECT_H)) |
			(data->flipYSprPriH << 8) |
			data->scaleY;
		// Set sprite's horizontal flip, unknown word 3 bit, background
		// priority, width, and width scale.
		// Also or's in a bitmask for some reason, that can set bits [15, 8] of
		// the word. It appears the or'd in mask is intended to set flipX, as
		// everything else in word 3 is set here.
		UNK_6061932.tempSprite[3] =
			(data->flipXBgPriW << 9) |
			((*object)[3] & (OBJECT_UNKNOWN1 | OBJECT_W)) |
			OBJECT_TOBGPRI(data->flipXBgPri) |
			OBJECT_TOSCALEX(data->scaleX);
		uint16_t palNum;
		if (data->palNum == 0u) {
			palNum = OBJECT_GETPALNUM(object);
		}
		else {
			palNum = data->palNum;
		}
		// Set sprite's palette, BPP, alpha, and upper 3 tile number bits.
		UNK_6061932.tempSprite[4] = OBJECT_TOPALNUM(palNum) | ((*object)[4] & ~OBJECT_PALNUM);
		// Set sprite's lower 16 tile number bits.
		UNK_6061932.tempSprite[5] = (*object)[5];
		// Add sprite.
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;

	case ADDSPRITE_5:
		// Set X and Y position of the sprite, but clear all other bits in words 0 and 1.
		UNK_6061932.tempSprite[0] = OBJECT_TOY(data->y1);
		UNK_6061932.tempSprite[1] = OBJECT_TOX(data->x1);
		// Set sprite's vertical flip, unknown word 2 bit, sprite priority, height, and height scale.
        UNK_6061932.tempSprite[2] = (data->flipYSprPriH << 8) | data->scaleY;
		// Set sprite's horizontal flip, unknown word 3 bit, background priority, width, and width scale.
        UNK_6061932.tempSprite[3] = (data->flipXBgPriW << 8) | data->scaleX;
		// Set sprite's palette, BPP, alpha, and upper 3 tile number bits.
		UNK_6061932.tempSprite[4] = OBJECT_TOPALNUM(data->palNum) | data->bppAlphaTileTop;
		// Set sprite's lower 16 tile number bits.
        UNK_6061932.tempSprite[5] = (*object)[5];
		// Add sprite.
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;

	case ADDSPRITE_ANIM: {
		ObjectData* animFrameObject = &object[data->animFrame];
		uint8_t flipXBgPriW;
		if (data->UNK_20 < 4) {
			UNK_6061932.tempSprite[0] = OBJECT_TOY(data->y + (*animFrameObject)[0]);
			UNK_6061932.tempSprite[1] = OBJECT_TOX(data->x + (*animFrameObject)[1]);
			// Disable vertical flip, clear unknown word 2 bit, set sprite
			// prioity to zero, and set sprite height to one.
			UNK_6061932.tempSprite[2] &= OBJECT_SCALEY;
			// Set background priority to one.
			flipXBgPriW = 0x10u;
		}
		else {
			if (data->verticalHorizontal & 0x8000u) {
				UNK_6061932.tempSprite[0] = OBJECT_TOY(data->y - (int8_t)(*animFrameObject)[0] - 16);
			}
			else {
				UNK_6061932.tempSprite[0] = OBJECT_TOY(data->y + (*animFrameObject)[0]);
			}

			if (data->verticalHorizontal & 0x0080u) {
				UNK_6061932.tempSprite[1] = OBJECT_TOX(data->x - (int8_t)(*animFrameObject)[1] - 16);
			}
			else {
				UNK_6061932.tempSprite[1] = OBJECT_TOX(data->x + (*animFrameObject)[1]);
			}
			UNK_6061932.tempSprite[2] |= data->verticalHorizontal & ~OBJECT_SCALEY;
			flipXBgPriW = (uint8_t)data->verticalHorizontal | 0x10u;
		}

		UNK_6061932.tempSprite[3] |= (uint16_t)flipXBgPriW << 8;
		UNK_6061932.tempSprite[4] |= (*animFrameObject)[4] & OBJECT_PALNUM;
		UNK_6061932.tempSprite[5] = (*animFrameObject)[5];
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;
	}

	case ADDSPRITE_7:
		UNK_6061932.tempSprite[0] = OBJECT_TOY(data->y + (*object)[0]);
		UNK_6061932.tempSprite[1] = OBJECT_TOX(data->x + (*object)[1]);
		UNK_6061932.tempSprite[2] |= (*object)[2] & ~OBJECT_SCALEY;
		UNK_6061932.tempSprite[3] |= ((*object)[3] & ~OBJECT_SCALEX) | OBJECT_TOBGPRI(1);
		UNK_6061932.tempSprite[4] = (*object)[4];
		UNK_6061932.tempSprite[5] = object[data->animFrame][5];
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;

	case ADDSPRITE_8:
		UNK_6061932.tempSprite[0] = OBJECT_TOY(data->y + (*object)[0]);
		UNK_6061932.tempSprite[1] = OBJECT_TOX(data->x + (*object)[1]);
		UNK_6061932.tempSprite[2] = ((*object)[2] & ~OBJECT_SCALEY) | UNSCALED;
		UNK_6061932.tempSprite[3] = ((*object)[3] & ~OBJECT_SCALEX) | UNSCALED | OBJECT_TOBGPRI(2);
		UNK_6061932.tempSprite[4] = (*object)[4];
        UNK_6061932.tempSprite[5] = (*object)[5];
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;

	case ADDSPRITE_9:
		UNK_6061932.tempSprite[0] = OBJECT_TOY(200);
		UNK_6061932.tempSprite[1] = OBJECT_TOX(10);
		UNK_6061932.tempSprite[2] = UNSCALED;
		UNK_6061932.tempSprite[3] = OBJECT_TOBGPRI(3u) | UNSCALED;
		UNK_6061932.tempSprite[4] = OBJECT_TOTILETOP(0x00);
		UNK_6061932.tempSprite[5] = OBJECT_TOTILEBOTTOM(0x1Fu);
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;

	case ADDSPRITE_10:
		UNK_6061932.tempSprite[0] = data->y1;
		UNK_6061932.tempSprite[1] = data->x1;
		UNK_6061932.tempSprite[2] = (data->flipYSprPriH << 8) | UNSCALED;
		UNK_6061932.tempSprite[3] = (data->flipXBgPriW << 8) | UNSCALED;
		UNK_6061932.tempSprite[4] = OBJECT_TOPALNUM(data->palNum);
		UNK_6061932.tempSprite[5] = OBJECT_TOTILEBOTTOM(0xFu);
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;

	case ADDSPRITE_11:
		UNK_6061932.tempSprite[0] = data->y1;
		UNK_6061932.tempSprite[1] = data->x1;
		UNK_6061932.tempSprite[2] = (data->flipYSprPriH << 8) | OBJECT_TOSCALEY(data->scaleY);
		UNK_6061932.tempSprite[3] = (data->flipXBgPriW << 8) | OBJECT_TOSCALEX(data->scaleX);
		UNK_6061932.tempSprite[4] = OBJECT_TOPALNUM(data->palNum) | data->bppAlphaTileTop;
		UNK_6061932.tempSprite[5] = data->tileBottom;
		SPRITE_COPY(Sprites[NumSprites], UNK_6061932.tempSprite);
		NumSprites++;
		return;

	default:
		// BUG: Original didn't have a default case, and would have undefined
		// behavior. Fixed that here.
		return;
	}
}
