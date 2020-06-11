#pragma once

#include "PlatformTypes.h"
#include "VideoDefs.h"
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
// _0x60243E8 or _0x602471C, at least.


typedef uint16_t ObjectData[6];

// A single object is composed of an array of one or more object datas. The
// number of sprites field in the first object data of an object is the only
// one used, though it appears the original object data table has the same
// number of sprites set in all the object datas of an object. Objects can be
// composed of up to 63 sprites.

#define OBJECT_GETY(sprite) ((int16_t)(((*(sprite))[0] & 0x3FF) << 6) >> 6)
#define OBJECT_GETNUMSPRITES(object) ((uint8_t)(((*(object))[1] >> 10) & 0x3F))
#define OBJECT_GETX(sprite) ((int16_t)(((*(sprite))[1] & 0x3FF) << 6) >> 6)
#define OBJECT_GETFLIPY(sprite) ((bool)(((*(sprite))[2] >> 15) & 1))
#define OBJECT_GETSPRPRI(sprite) ((uint8_t)(((*(sprite))[2] >> 12) & 3))
#define OBJECT_GETH(sprite) ((uint8_t)(((*(sprite))[2] >> 8) & 0xF))
#define OBJECT_GETSCALEY(sprite) ((uint8_t)((*(sprite))[2] & 0xFF))
#define OBJECT_GETFLIPX(sprite) ((bool)(((*(sprite))[3] >> 15) & 1))
#define OBJECT_GETBGPRI(sprite) ((uint8_t)(((*(sprite))[3] >> 12) & 3))
#define OBJECT_GETW(sprite) ((uint8_t)(((*(sprite))[3] >> 8) & 0xF))
#define OBJECT_GETSCALEX(sprite) ((uint8_t)((*(sprite))[3] & 0xFF))
#define OBJECT_GETPALNUM(sprite) ((uint8_t)(((*(sprite))[4] >> 8) & 0xFF))
#define OBJECT_GETBPP(sprite) ((Bpp)(((*(sprite))[4] >> 7) & 1))
#define OBJECT_GETALPHA(sprite) ((uint8_t)(((*(sprite))[4] >> 4) & 7))
#define OBJECT_GETTILE(sprite) (((((uint32_t)(*(sprite))[4]) << 16) | (uint32_t)(*(sprite))[5]) & 0x7FFFF)

#define OBJECT_SETY(sprite, y) ((*(sprite))[0] = ((*(sprite))[0] & ~0x03FF) | ((uint16_t)(y) & 0x3FF))
#define OBJECT_SETNUMSPRITES(object, numSprites) ((*(object))[1] = ((*(object))[1] & 0x03FFu) | (((uint16_t)(numSprites) & 0x3Fu) << 10u))
#define OBJECT_SETX(sprite, x) ((*(sprite))[1] = ((*(sprite))[1] & ~0x03FF) | ((uint16_t)(x) & 0x3FF))
#define OBJECT_SETFLIPY(sprite, flipY) ((*(sprite))[2] = ((*(sprite))[2] & ~0x8000) | (((uint16_t)(flipY) & 1) << 15)
#define OBJECT_SETSPRITEPRI(sprite, spritePri) ((*(sprite))[2] = ((*(sprite))[2] & ~0x7000) | (((uint16_t)(spritePri) & 3) << 12))
#define OBJECT_SETH(sprite, h) ((*(sprite))[2] = ((*(sprite))[2] & ~0x0F00) | (((uint16_t)(h) & 0xF) << 8))
#define OBJECT_SETSCALEY(sprite, scaleX) ((*(sprite))[2] = ((*(sprite))[2] & ~0x00FF) | ((uint16_t)(scaleX) & 0xFF))
#define OBJECT_SETFLIPX(sprite, flipX) ((*(sprite))[3] = ((*(sprite))[3] & ~0x8000) | (((uint16_t)(flipX) & 1) << 15))
#define OBJECT_SETBGPRI(sprite, bgPri) ((*(sprite))[3] = ((*(sprite))[3] & ~0xF000) | (((uint16_t)(bgPri) & 3) << 12))
#define OBJECT_SETW(sprite, w) ((*(sprite))[3] = ((*(sprite))[3] & ~0x0F00) | (((uint16_t)(w) & 0xF) << 8))
#define OBJECT_SETSCALEX(sprite, scaleY) ((*(sprite))[3] = ((*(sprite))[3] & ~0x00FF) | ((uint16_t)(scaleY) & 0xFF))
#define OBJECT_SETPALNUM(sprite, palNum) ((*(sprite))[4] = ((*(sprite))[4] & ~0xFF00) | (((uint16_t)(palNum) & 0xFF) << 8))
#define OBJECT_SETBPP(sprite, bpp) ((*(sprite))[4] = ((*(sprite))[4] & ~0x0080) | (((uint16_t)(bpp) & 1) << 7))
#define OBJECT_SETALPHA(sprite, alpha) ((*(sprite))[4] = ((*(sprite))[4] & ~0x0070) | (((uint16_t)(alpha) & 7) << 4))
#define OBJECT_SETTILE(sprite, tile) (((*(sprite))[4] = ((*(sprite))[4] & ~0x0007) | ((uint16_t)(((uint32_t)(tile)) >> 16) & 7)), ((*(sprite))[5] = ((uint16_t)(tile) & 0xFFFF)))


typedef struct ObjectDataTable {
	char header[8];
	ObjectData data[0x6DC];
} ObjectDataTable;

// TODO: Define in PlatformSupport/DataTable.c
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

#define OBJECT_PUSH1PSTARTBUTTON OBJECTPTR(0x17B)
#define OBJECT_PUSH2PSTARTBUTTON OBJECTPTR(0x17C)
#define OBJECT_INSERTCOIN OBJECTPTR(0x17F)
#define _0xA6F14 OBJECTPTR(0x181)
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

#define OBJECTTABLE_LASER OBJECTPTR(0x2FF)
#define OBJECTTABLE_LASERRETICLE OBJECTPTR(0x30F)
#define OBJECTTABLE_ITEMWARNINGROLLROLL OBJECTPTR(0x31F)
#define OBJECTTABLE_ITEMWARNINGTRANSFORM OBJECTPTR(0x35F)
#define OBJECTTABLE_ROWDEL OBJECTPTR(0x39F)
#define OBJECTTABLE_ITEMWARNINGHARDBLOCK OBJECTPTR(0x3A7)
#define OBJECT_COLUMNMARKER OBJECTPTR(0x3E7)
#define OBJECTTABLE_ITEMWARNINGDEATHBLOCK OBJECTPTR(0x3E8)
#define OBJECTTABLE_ITEMWARNINGCOLORBLOCK OBJECTPTR(0x436)
#define OBJECTTABLE_ITEMWARNINGDARKBLOCK OBJECTPTR(0x476)
#define OBJECTTABLE_ITEMWARNINGEXCHGFIELD OBJECTPTR(0x4B6)
#define OBJECTTABLE_ITEMWARNINGMIRRORBLOCK OBJECTPTR(0x4F6)
#define OBJECTTABLE_ITEMWARNINGNEGAFIELD OBJECTPTR(0x536)
#define OBJECTTABLE_ITEMWARNINGSHOTGUN OBJECTPTR(0x5B6)
#define OBJECTTABLE_ITEMWARNINGXRAY OBJECTPTR(0x5F6)
#define OBJECTTABLE_ITEMDESCRIPTIONS OBJECTPTR(0x659)

#define OBJECT_PLEASEWAIT OBJECTPTR(0x66D)
#define OBJECTTABLE_NEWCHALLENGER OBJECTPTR(0x66E)

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

#define _0xAAEB0 OBJECTPTR(0x6CE)
#define _0xAAEBC OBJECTPTR(0x6CF)
#define _0xAAEC8 OBJECTPTR(0x6D0)

#define OBJECT_RETRYFORGRANDMASTER OBJECTPTR(0x6DB)