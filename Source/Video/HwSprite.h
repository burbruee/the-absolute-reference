#pragma once

#include "Video/VideoDefs.h"
#include <stdint.h>

// TODO: Use these in the code.
typedef uint8_t SpriteScale;
#define UNSCALED 0x3F
// TODO: Investigate whether a percentage-input macro can be created and replace this with it.
#define SPRITESCALE(scale) ((SpriteScale)((scale) + UNSCALED))

// PS6406B sprite format:
//
// 0:??????YYYYYYYYYY 1:??????XXXXXXXXXX 2:-USSHHHHhhhhhhhh 3:|uBBWWWWwwwwwwww
// 4:PPPPPPPPbAAA?TTT 5:TTTTTTTTTTTTTTTT 6:???????????????? 7:????????????????
//
// Each character is a bit in a big endian uint16_t.
//
// ?: Unused or unknown.
// Y: Signed Y pixel position.
// X: Signed X pixel position.
// -: Flip sprite vertically.
// U: There are hints this could be something, but unknown for now.
// S: Sprite priority. TODO: Check how this works on a PS5V2. Unused by non-PS5V2 frontends for now.
// H: Sprite height minus one.
// h: Sprite height scale.
// |: Flip sprite horizontally.
// u: There are hints this could be something, but unknown for now.
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
// the first 6 uint16_t of SpriteData, rather than all 8.
//
// The tile number defined in the sprite follows this formula:
// realTileNumber / (bitsPerPixel + 1).
// So for a 4bpp sprite with starting tile number 2, 2 is used, while 1 would
// be used for an 8bpp sprite if the real tile number of the 8bpp sprite is 2.
//
// TODO: Investigate on a real PS5V2 whether the last two uint16_ts in a sprite
// are used by the PS6406B for anything (rotation? MAME documentation says
// backgrouds can rotate, so why not sprites?). There are some hints in the
// code they are used, but as of yet, no hints for what that is.
#define SPRITE_LENGTH 8
typedef uint16_t SpriteData[SPRITE_LENGTH];

// The MAME documentation says the PS6406B supports 1024 sprites, but the
// original game only supports 896. Though the first two sprites are taken up
// for what could be sprite/video settings (those sprites' tile numbers are 0,
// so they don't display in MAME, due to the tile data for that tile number
// being all zeros).
#define MAXSPRITES 0x380
#define SPRITE_FIRST 2

#define Sprites ((RAMDATA SpriteData*)&SPRITERAM[0x0000u])
#define SpriteNames ((RAMDATA uint16_t*)&SPRITERAM[0x1C00u])
#define SPRITENAME_TERMINATE 0x4000u

// Always use this when copying an entire sprite to the Sprites array.
#define SPRITE_COPY(dst, src) ( \
	(dst)[0] = (src)[0],  \
	(dst)[1] = (src)[1],  \
	(dst)[2] = (src)[2],  \
	(dst)[3] = (src)[3],  \
	(dst)[4] = (src)[4],  \
	(dst)[5] = (src)[5],  \
	(dst)[6] = (src)[6],  \
	(dst)[7] = (src)[7]   \
)
