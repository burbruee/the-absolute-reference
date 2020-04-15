#pragma once

#include "VideoDefs.h"
#include <stdint.h>

// TODO: Use these in the code.
typedef uint8_t SpriteScale;
#define UNSCALED 0x3Fu
// TODO: Investigate whether a percentage-input macro can be created and replace this with it.
#define SPRITESCALE(scale) ((SpriteScale)((scale) + UNSCALED))

// PS6406B sprite format:
//
// 0:??????YYYYYYYYYY 1:??????XXXXXXXXXX 2:-?SSHHHHhhhhhhhh 3:|?BBWWWWwwwwwwww
// 4:PPPPPPPPbAAA?TTT 5:TTTTTTTTTTTTTTTT 6:???????????????? 7:????????????????
//
// Each character is a bit in a big endian uint16_t.
//
// ?: Unused or unknown.
// Y: Y pixel position.
// X: X pixel position.
// -: Flip sprite vertically.
// S: Sprite priority. TODO: Check how this works on a PS5V2. Unused by non-PS5V2 frontends for now.
// H: Sprite height minus one.
// h: Sprite height scale.
// |: Flip sprite horizontally.
// B: Background priority.
// W: Sprite width.
// w: Sprite width scale.
// P: Palette.
// b: Bits per pixel.
// A: Alpha.
// T: Tile number.
//
// Unscaled sprite pixel dimensions are "16 * (W + 1)" by "16 * (H + 1)".
//
// Some of the unused space is used in objects, though objects are an array of
// 6 uint16_t, rather than 8.
//
// The tile number defined in the sprite follows this formula:
// realTileNumber / (bitsPerPixel + 1).
// So for a 4bpp sprite with starting tile number 2, 2 is used, while 1 would
// be used for an 8bpp sprite if the real tile number of the 8bpp sprite is 2.
//
// TODO: Investigate on a real PS5V2 whether the last two uint16_ts in a sprite
// are used by the PS6406B for anything (rotation? MAME documentation says
// backgrouds can rotate, so why not sprites?).
typedef uint16_t SpriteData[8];

// The MAME documentation says the PS6406B supports 1024 sprites, but the
// original game only supports 896. Though the first two sprites are taken up
// for what could be sprite/video settings (those sprites' tile numbers are 0,
// so they don't display in MAME, due to the tile data for that tile number
// being all zeros).
#define MAXSPRITES 0x380
#define SPRITE_FIRST 2

// TODO: Change these to OBJECT_ and move them into DisplayObject.h.
#define SPRITE_GETY(sprite) ((int16_t)(((*(sprite))[0] & 0x3FF) << 6) >> 6)
#define SPRITE_GETX(sprite) ((int16_t)(((*(sprite))[1] & 0x3FF) << 6) >> 6)
#define SPRITE_GETFLIPY(sprite) ((bool)(((*(sprite))[2] >> 15) & 1))
#define SPRITE_GETSPRPRI(sprite) ((uint8_t)(((*(sprite))[2] >> 12) & 3))
#define SPRITE_GETH(sprite) ((uint8_t)(((*(sprite))[2] >> 8) & 0xF))
#define SPRITE_GETSCALEY(sprite) ((uint8_t)((*(sprite))[2] & 0xFF))
#define SPRITE_GETFLIPX(sprite) ((bool)(((*(sprite))[3] >> 15) & 1))
#define SPRITE_GETBGPRI(sprite) ((uint8_t)(((*(sprite))[3] >> 12) & 3))
#define SPRITE_GETW(sprite) ((uint8_t)(((*(sprite))[3] >> 8) & 0xF))
#define SPRITE_GETSCALEX(sprite) ((uint8_t)((*(sprite))[3] & 0xFF))
#define SPRITE_GETPALNUM(sprite) ((uint8_t)(((*(sprite))[4] >> 8) & 0xFF))
#define SPRITE_GETBPP(sprite) ((Bpp)(((*(sprite))[4] >> 7) & 1))
#define SPRITE_GETALPHA(sprite) ((uint8_t)(((*(sprite))[4] >> 4) & 7))
#define SPRITE_GETTILE(sprite) (((((uint32_t)(*(sprite))[4]) << 16) | (uint32_t)(*(sprite))[5]) & 0x7FFFF)

#define SPRITE_SETY(sprite, y) ((*(sprite))[0] = ((*(sprite))[0] & ~0x03FF) | ((uint16_t)(y) & 0x3FF))
#define SPRITE_SETX(sprite, x) ((*(sprite))[1] = ((*(sprite))[1] & ~0x03FF) | ((uint16_t)(x) & 0x3FF))
#define SPRITE_SETFLIPY(sprite, flipY) ((*(sprite))[2] = ((*(sprite))[2] & ~0x8000) | (((uint16_t)(flipY) & 1) << 15)
#define SPRITE_SETSPRITEPRI(sprite, spritePri) ((*(sprite))[2] = ((*(sprite))[2] & ~0x7000) | (((uint16_t)(spritePri) & 3) << 12))
#define SPRITE_SETH(sprite, h) ((*(sprite))[2] = ((*(sprite))[2] & ~0x0F00) | (((uint16_t)(h) & 0xF) << 8))
#define SPRITE_SETSCALEY(sprite, scaleX) ((*(sprite))[2] = ((*(sprite))[2] & ~0x00FF) | ((uint16_t)(scaleX) & 0xFF))
#define SPRITE_SETFLIPX(sprite, flipX) ((*(sprite))[3] = ((*(sprite))[3] & ~0x8000) | (((uint16_t)(flipX) & 1) << 15))
#define SPRITE_SETBGPRI(sprite, bgPri) ((*(sprite))[3] = ((*(sprite))[3] & ~0xF000) | (((uint16_t)(bgPri) & 3) << 12))
#define SPRITE_SETW(sprite, w) ((*(sprite))[3] = ((*(sprite))[3] & ~0x0F00) | (((uint16_t)(w) & 0xF) << 8))
#define SPRITE_SETSCALEX(sprite, scaleY) ((*(sprite))[3] = ((*(sprite))[3] & ~0x00FF) | ((uint16_t)(scaleY) & 0xFF))
#define SPRITE_SETPALNUM(sprite, palNum) ((*(sprite))[4] = ((*(sprite))[4] & ~0xFF00) | (((uint16_t)(palNum) & 0xFF) << 8))
#define SPRITE_SETBPP(sprite, bpp) ((*(sprite))[4] = ((*(sprite))[4] & ~0x0080) | (((uint16_t)(bpp) & 1) << 7))
#define SPRITE_SETALPHA(sprite, alpha) ((*(sprite))[4] = ((*(sprite))[4] & ~0x0070) | (((uint16_t)(alpha) & 7) << 4))
#define SPRITE_SETTILE(sprite, tile) (((*(sprite))[4] = ((*(sprite))[4] & ~0x0007) | ((uint16_t)(((uint32_t)(tile)) >> 16) & 7)), ((*(sprite))[5] = ((uint16_t)(tile) & 0xFFFF)))

#define Sprites ((RAMDATA SpriteData*)&SPRITERAM[0x0000u])
#define SpriteNames ((RAMDATA uint16_t*)&SPRITERAM[0x1C00u])
