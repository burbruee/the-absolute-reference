#pragma once

#include "Video/VideoDefs.h"
#include "Video/HwSprite.h"
#include "PlatformTypes.h"
#include <stdint.h>
#include <stdbool.h>

// The DisplayObject* functions accept ObjectData arrays, not SpriteData
// arrays, and each ObjectData contains only the following SpriteData fields,
// in the same layout as SpriteData:
//
// - Pixel Y
// - Pixel X
// - Vertical flip
// - Sprite priority
// - Height
// - Horizontal flip
// - Background priority
// - Width
// - Palette number*
// - Bits per pixel.
// - Tile number
//
// * The palette number field is only used by DisplayObjectEx. The value of the
// palette number field in the first element of the object data array is used
// for all sprites when passing zero as the palette number argument to
// DisplayObjectEx.
//
// When dynamically generating objects, only ever use the object tile numbers
// that are in Objects.data; the game follows this convention all throughout
// the code, and this allows renderers to optimize around that assumption.
//
// TODO: There appears to be more object data beyond what DisplayObject*
// functions support, and some of that data appears to be at indices 6 and 7 of
// SpriteData.
// NOTE: Bit 15 of word 0 appears to be used as a flag that, when true,
// indicates if an object should be skipped when displaying with functions
// UNK_60243E8 or UNK_602471C, at least.


typedef uint16_t ObjectData[6];

// A single object is composed of an array of one or more object datas. The
// number of sprites field in the first object data of an object is the only
// one used, though it appears the original object data table has the same
// number of sprites set in all the object datas of an object. Objects can be
// composed of up to 63 sprites.

enum ObjectBits {
	// object[0]
	OBJECT_Y = 0x03FFu,

	// object[1]
	OBJECT_NUMSPRITES = 0xFC00u,
	OBJECT_X = 0x03FFu,

	// object[2]
	OBJECT_FLIPY = 0x8000u,
	OBJECT_UNKNOWN0 = 0x4000u,
	OBJECT_SPRPRI = 0x3000u,
	OBJECT_H = 0x0F00u,
	OBJECT_SCALEY = 0x00FFu,

	// object[3]
	OBJECT_FLIPX = 0x8000u,
	OBJECT_UNKNOWN1 = 0x4000u,
	OBJECT_BGPRI = 0x3000u,
	OBJECT_W = 0x0F00u,
	OBJECT_SCALEX = 0x00FFu,

	// object[4]
	OBJECT_PALNUM = 0xFF00u,
	OBJECT_BPP = 0x0080u,
	OBJECT_ALPHA = 0x0070u,
	OBJECT_TILETOP = 0x0007u,

	// object[5]
	OBJECT_TILEBOTTOM = 0xFFFFu
};

#define OBJECT_GETY(object) ((int16_t)(((*(object))[0] & OBJECT_Y) << 6) >> 6)
#define OBJECT_GETNUMSPRITES(object) ((uint8_t)(((*(object))[1] & OBJECT_NUMSPRITES) >> 10))
#define OBJECT_GETX(object) ((int16_t)(((*(object))[1] & OBJECT_X) << 6) >> 6)
#define OBJECT_GETFLIPY(object) ((bool)(((*(object))[2] & OBJECT_FLIPY) >> 15))
#define OBJECT_GETUNKNOWN0(object) ((bool)(((*(object))[2] & OBJECT_UNKNOWN0) >> 14))
#define OBJECT_GETSPRPRI(object) ((uint8_t)(((*(object))[2] & OBJECT_SPRPRI) >> 12))
#define OBJECT_GETH(object) ((uint8_t)(((*(object))[2] & OBJECT_H) >> 8))
#define OBJECT_GETSCALEY(object) ((SpriteScale)((*(object))[2] & OBJECT_SCALEY))
#define OBJECT_GETFLIPX(object) ((bool)(((*(object))[3] & OBJECT_FLIPX) >> 15))
#define OBJECT_GETUNKNOWN1(object) ((bool)(((*(object))[2] & OBJECT_UNKNOWN1) >> 14))
#define OBJECT_GETBGPRI(object) ((uint8_t)(((*(object))[3] & OBJECT_BGPRI) >> 12))
#define OBJECT_GETW(object) ((uint8_t)(((*(object))[3] & OBJECT_W) >> 8))
#define OBJECT_GETSCALEX(object) ((SpriteScale)((*(object))[3] & 0xFF))
#define OBJECT_GETPALNUM(object) ((uint8_t)(((*(object))[4] & OBJECT_PALNUM) >> 8))
#define OBJECT_GETBPP(object) ((Bpp)(((*(object))[4] & OBJECT_BPP) >> 7))
#define OBJECT_GETALPHA(object) ((uint8_t)(((*(object))[4] & OBJECT_ALPHA) >> 4))
#define OBJECT_GETTILE(object) ((((uint32_t)(*(object))[4] & OBJECT_TILETOP) << 16) | ((uint32_t)(*(object))[5] & OBJECT_TILEBOTTOM))

#define OBJECT_TOY(y) ((uint16_t)((int16_t)(y) & OBJECT_Y))
#define OBJECT_TONUMSPRITES(numSprites) (((uint16_t)(numSprites) << 10) & OBJECT_NUMSPRITES)
#define OBJECT_TOX(x) ((uint16_t)((int16_t)(x) & OBJECT_X))
#define OBJECT_TOFLIPY(flipY) (((uint16_t)(flipY != 0u) << 15) & OBJECT_FLIPY)
#define OBJECT_TOUNKNOWN0(unknown0) (((uint16_t)(unknown0 != 0u) << 14) & OBJECT_UNKNOWN0)
#define OBJECT_TOSPRPRI(sprPri) (((uint16_t)(sprPri) << 12) & OBJECT_SPRPRI)
#define OBJECT_TOH(h) (((uint16_t)(h) << 8) & OBJECT_H)
#define OBJECT_TOSCALEY(scaleY) ((uint16_t)((scaleY) & OBJECT_SCALEY))
#define OBJECT_TOFLIPX(flipX) (((uint16_t)(flipX != 0u) << 15) & OBJECT_FLIPX)
#define OBJECT_TOUNKNOWN1(unknown1) (((uint16_t)(unknown1 != 0u) << 14) & OBJECT_UNKNOWN1)
#define OBJECT_TOBGPRI(bgPri) (((uint16_t)(bgPri) << 12) & OBJECT_BGPRI)
#define OBJECT_TOW(w) (((uint16_t)(w) << 8) & OBJECT_W)
#define OBJECT_TOSCALEX(scaleX) ((uint16_t)(scaleX) & OBJECT_SCALEX)
#define OBJECT_TOPALNUM(palNum) (((uint16_t)(palNum) << 8) & OBJECT_PALNUM)
#define OBJECT_TOBPP(bpp) (((uint16_t)(Bpp)(bpp) << 7u) & OBJECT_BPP)
#define OBJECT_TOALPHA(alpha) (((uint16_t)(alpha) << 4) & OBJECT_ALPHA)
#define OBJECT_TOTILETOP(tile) ((uint16_t)((uint32_t)(tile) >> 16u) & OBJECT_TILETOP)
#define OBJECT_TOTILEBOTTOM(tile) ((uint16_t)(tile) & OBJECT_TILEBOTTOM)

#define OBJECT_SETY(object, y)                   ((*(object))[0] = ((*(object))[0] & ~OBJECT_Y)          | OBJECT_TOY(y))
#define OBJECT_SETNUMSPRITES(object, numSprites) ((*(object))[1] = ((*(object))[1] & ~OBJECT_NUMSPRITES) | OBJECT_TONUMSPRITES(numSprites))
#define OBJECT_SETX(object, x)                   ((*(object))[1] = ((*(object))[1] & ~OBJECT_X)          | OBJECT_TOX(x))
#define OBJECT_SETFLIPY(object, flipY)           ((*(object))[2] = ((*(object))[2] & ~OBJECT_FLIPY)      | OBJECT_TOFLIPY(flipY))
#define OBJECT_SETUNKNOWN0(object, unknown0)     ((*(object))[2] = ((*(object))[2] & ~OBJECT_UNKNOWN0)   | OBJECT_TOUNKNOWN0(unknown0))
#define OBJECT_SETSPRPRI(object, sprPri)         ((*(object))[2] = ((*(object))[2] & ~OBJECT_SPRPRI)     | OBJECT_TOSPRPRI(sprPri))
#define OBJECT_SETH(object, h)                   ((*(object))[2] = ((*(object))[2] & ~OBJECT_H)          | OBJECT_TOH(h))
#define OBJECT_SETSCALEY(object, scaleY)         ((*(object))[2] = ((*(object))[2] & ~OBJECT_SCALEY)     | OBJECT_TOSCALEY(scaleY))
#define OBJECT_SETFLIPX(object, flipX)           ((*(object))[3] = ((*(object))[3] & ~OBJECT_FLIPX)      | OBJECT_TOFLIPX(flipX))
#define OBJECT_SETUNKNOWN1(object, unknown1)     ((*(object))[3] = ((*(object))[3] & ~OBJECT_UNKNOWN1)   | OBJECT_TOUNKNOWN1(unknown1))
#define OBJECT_SETBGPRI(object, bgPri)           ((*(object))[3] = ((*(object))[3] & ~OBJECT_BGPRI)      | OBJECT_TOBGPRI(bgPri))
#define OBJECT_SETW(object, w)                   ((*(object))[3] = ((*(object))[3] & ~OBJECT_W)          | OBJECT_TOW(w))
#define OBJECT_SETSCALEX(object, scaleX)         ((*(object))[3] = ((*(object))[3] & ~OBJECT_SCALEX)     | OBJECT_TOSCALEX(scaleX))
#define OBJECT_SETPALNUM(object, palNum)         ((*(object))[4] = ((*(object))[4] & ~OBJECT_PALNUM)     | OBJECT_TOPALNUM(palNum))
#define OBJECT_SETBPP(object, bpp)               ((*(object))[4] = ((*(object))[4] & ~OBJECT_BPP)        | OBJECT_TOBPP(bpp))
#define OBJECT_SETALPHA(object, alpha)           ((*(object))[4] = ((*(object))[4] & ~OBJECT_ALPHA)      | OBJECT_TOALPHA(alpha))
#define OBJECT_SETTILE(object, tile)            (((*(object))[4] = ((*(object))[4] & ~OBJECT_TILETOP)    | OBJECT_TOTILETOP(tile)), \
                                                 ((*(object))[5] = ((*(object))[5] & ~OBJECT_TILEBOTTOM) | OBJECT_TOTILEBOTTOM(tile)))

typedef struct ObjectDataTable {
	char header[8];
	ObjectData data[0x6DC];
} ObjectDataTable;

extern ROMDATA ObjectDataTable Objects;
#define OBJECTPTR(i) ((ROMDATA ObjectData*)&Objects.data[(i)])

// TODO: Define all objects here.

#define OBJECT_MASTERFIELDBORDER OBJECTPTR(0x4)
#define OBJECT_DOUBLESFIELDBORDER OBJECTPTR(0x5)
#define OBJECT_NORMALFIELDBORDER OBJECTPTR(0x6)

#define OBJECTTABLE_NORMALBLOCKS OBJECTPTR(0x7)
#define OBJECT_HARDBLOCK OBJECTPTR(0x17)
#define OBJECTTABLE_ITEMBLOCKS OBJECTPTR(0x18)
#define OBJECTTABLES_BLOCKEXPLOSIONS OBJECTPTR(0x4B)
#define NUMBLOCKEXPLOSIONS 32
#define NUMBLOCKEXPLOSIONTYPES 8

#define OBJECT_NEXTBLOCKLABEL OBJECTPTR(0x151)
#define OBJECT_SCORELABEL OBJECTPTR(0x152)
#define OBJECT_LEVELLABEL OBJECTPTR(0x153)

#define OBJECT_CREDIT OBJECTPTR(0x167)
#define OBJECT_FREE OBJECTPTR(0x168)
#define OBJECT_SERVICE OBJECTPTR(0x169)
#define OBJECT_COINCOLON OBJECTPTR(0x16A)

#define OBJECT_READY OBJECTPTR(0x16B)
#define OBJECT_GO OBJECTPTR(0x16C)

#define OBJECTTABLE_VERSUSREADYGO OBJECTPTR(0x171)

#define OBJECT_PUSH1PSTARTBUTTON OBJECTPTR(0x17B)
#define OBJECT_PUSH2PSTARTBUTTON OBJECTPTR(0x17C)
#define OBJECT_INSERTCOIN OBJECTPTR(0x17F)
#define OBJECT_A6F14 OBJECTPTR(0x181)
#define OBJECT_ADDCOIN OBJECTPTR(0x182)

#define OBJECT_SELECTMODE OBJECTPTR(0x198)

#define OBJECT_CHALLENGE OBJECTPTR(0x199)
#define OBJECTTABLE_YES OBJECTPTR(0x19E)
#define OBJECTTABLE_NO OBJECTPTR(0x1A0)

#define OBJECTTABLE_RANKINGDIGITS OBJECTPTR(0x1A4) 
#define OBJECT_RANKINGTIMECOLON OBJECTPTR(0x1AE)

#define OBJECTTABLE_GAMESTATUSTIMEDIGITS OBJECTPTR(0x1AF)
#define TIMEDIGIT_WIDTH 16
#define OBJECT_TIMECOLON OBJECTPTR(0x1B9)
#define TIMECOLON_WIDTH 12
#define TIMENUM_WIDTH 28

#define OBJECTTABLE_STATUSDIGITS OBJECTPTR(0x1BA)
#define STATUSDIGIT_WIDTH 8

#define OBJECTTABLE_GRADES OBJECTPTR(0x1C4)

#define OBJECT_SINGLEFIELDBG OBJECTPTR(0x1D8)
#define OBJECT_DOUBLESFIELDBG OBJECTPTR(0x1D9)

#define OBJECTTABLE_RANKINGCHARS OBJECTPTR(0x1DA)

#define OBJECTTABLE_MEDALS OBJECTPTR(0x23D)

#define OBJECT_NAMEENTRYLABEL OBJECTPTR(0x246)
#define OBJECTTABLE_RANKINGPLACES OBJECTPTR(0x247)
#define OBJECT_DOUBLESRANKINGLABEL OBJECTPTR(0x24C)
#define OBJECT_DOUBLESTODAYSBESTRANKINGLABEL OBJECTPTR(0x24D)
#define OBJECT_DOUBLESCLEARTIMELABEL OBJECTPTR(0x24E)
#define OBJECT_DOUBLESNAMELEVELRANKINGLABEL OBJECTPTR(0x24F)
#define OBJECT_DOUBLES1PRANKINGLABEL OBJECTPTR(0x250)
#define OBJECT_DOUBLES2PRANKINGLABEL OBJECTPTR(0x251)

#define OBJECT_ITEMBARBORDER OBJECTPTR(0x252)
#define OBJECT_ITEMBARBG OBJECTPTR(0x253)
#define OBJECTTABLE_ITEMBARCHUNKS OBJECTPTR(0x254)

#define OBJECT_VERSUSROUNDLIGHT OBJECTPTR(0x264)

#define OBJECTTABLE_CHARS OBJECTPTR(0x279)
#define OBJECTTABLE_HEXDIGITS OBJECTPTR(0x2A9)

#define OBJECTTABLE_LASER OBJECTPTR(0x2FF)
#define OBJECTTABLE_LASERRETICLE OBJECTPTR(0x30F)
#define OBJECTTABLE_ITEMWARNINGROLLROLL OBJECTPTR(0x31F)
#define OBJECTTABLE_ITEMWARNINGTRANSFORM OBJECTPTR(0x35F)
#define OBJECTTABLE_ROWDEL OBJECTPTR(0x39F)
#define OBJECTTABLE_ITEMWARNINGHARDBLOCK OBJECTPTR(0x3A7)
#define OBJECT_COLUMNMARKER OBJECTPTR(0x3E7)
#define OBJECTTABLE_ITEMWARNINGDEATHBLOCK OBJECTPTR(0x3E8)
#define OBJECTTABLE_ITEMBLOCKEXPLOSION OBJECTPTR(0x428)
#define OBJECTTABLE_ITEMWARNINGCOLORBLOCK OBJECTPTR(0x436)
#define OBJECTTABLE_ITEMWARNINGDARKBLOCK OBJECTPTR(0x476)
#define OBJECTTABLE_ITEMWARNINGEXCHGFIELD OBJECTPTR(0x4B6)
#define OBJECTTABLE_ITEMWARNINGMIRRORBLOCK OBJECTPTR(0x4F6)
#define OBJECTTABLE_ITEMWARNINGNEGAFIELD OBJECTPTR(0x536)
#define OBJECTTABLE_ITEMWARNINGSHOTGUN OBJECTPTR(0x5B6)
#define OBJECTTABLE_ITEMWARNINGXRAY OBJECTPTR(0x5F6)

#define OBJECT_COPYRIGHT OBJECTPTR(0x658)

#define OBJECTTABLE_ITEMDESCRIPTIONS OBJECTPTR(0x659)

#define OBJECT_PLEASEWAIT OBJECTPTR(0x66D)
#define OBJECTTABLE_NEWCHALLENGER OBJECTPTR(0x66E)

#define OBJECT_STAFF OBJECTPTR(0x670)

#define OBJECT_SINGLENEXTBLOCKBG OBJECTPTR(0x696)
#define OBJECT_DOUBLESNEXTBLOCKBG OBJECTPTR(0x697)

#define OBJECT_GRANDMASTERCONGRATULATIONS OBJECTPTR(0x698)

#define OBJECT_VERSUSWINDIGITS OBJECTPTR(0x699)
#define VERSUSWINDIGIT_WIDTH 8
#define OBJECT_VERSUSWINLABEL OBJECTPTR(0x6A3)

#define OBJECTTABLE_FIREWORKS OBJECTPTR(0x6A6)

// TODO: Define OBJECT_MODECODE20G etc. instead of using this. But use
// MODECODES for defining the specific mode code objects.
#define OBJECTTABLE_MODECODES OBJECTPTR(0x6C6)
#define MODECODE_HEIGHT 8

#define OBJECT_SECRETGRADE OBJECTPTR(0x6CC)

#define OBJECT_BRAVO OBJECTPTR(0x6CD)

#define OBJECT_AAEB0 OBJECTPTR(0x6CE)
#define OBJECT_AAEBC OBJECTPTR(0x6CF)
#define OBJECT_AAEC8 OBJECTPTR(0x6D0)

#define OBJECT_RETRYFORGRANDMASTER OBJECTPTR(0x6DB)
