#pragma once

#include "GameBg.h"
#include "VideoDefs.h"
#include "HwData.h"
#include "HwSprite.h"
#include "DisplayObject.h"
#include "Fixed.h"
#include "PlatformTypes.h"
#include <stdint.h>
#include <stdbool.h>

extern const int16_t _0x60356C8[4];

// TODO: Could be data for setting sprites with AddSprite and other functions
// (so name it SetSpriteData). Might only be for setting one sprite at a time,
// unlike the object functions that write multiple sprites for a single object.
// For AddSprite, data either points to one ObjectData for a static
// sprite or an ObjectData array with one ObjectData per animation frame.
// SH-2: sizeof(AddSpriteData) == 0x40
typedef struct AddSpriteData {
	const void* data;
	int16_t y;
	int16_t x;
	uint16_t y1;
	uint16_t x1;
	uint16_t flipYSprPriH;
	uint16_t flipXBgPriW;
	uint8_t scaleY;
	uint8_t scaleX;
	int16_t animFrame;
	uint16_t flipXBgPri;
	uint16_t palNum;
	uint8_t bppAlphaTileTop;
	uint16_t tileBottom;

	// In AddSprite setting type 6, this can affect whether the flip settings
	// are used; when less than 4, no flip settings are used, otherwise flip
	// settings are used along with some other adjustments to the set data.
	int16_t _0x20;

	uint16_t verticalHorizontal;
	int16_t _0x28;
	uint8_t layer;
	uint8_t _0x2B;
	uint16_t _0x2C;
	uint8_t _0x2E;
	uint8_t _0x2F;
	int16_t _0x30;
	int16_t _0x32;
	int16_t _0x34;
	uint16_t _0x36;
	int16_t _0x38;
} AddSpriteData;

// The number of sprites written to Sprites.
extern int16_t NumSprites;

typedef struct struct_0x607D218_0x300 {
	void (*_0x0[4])();
} struct_0x607D218_0x300;

typedef struct struct_0x607D218 struct_0x607D218;
struct struct_0x607D218 {
	struct_0x607D218* _0x0;
	struct_0x607D218* _0x4;
	struct_0x607D218* _0x8;
	struct_0x607D218* _0xC;
	struct_0x607D218* _0x10;
	struct_0x607D218* _0x14;
	uint16_t _0x18;
	uint16_t _0x1A;
	uint8_t _0x1C[4];
	Color backdropColor;
	int16_t _0x24;
	Fixed32 _0x28;
	Fixed32 _0x2C;
	uint16_t _0x30; // TODO: Might be bool.
	struct_0x607D218_0x300 _0x300[16];
};

#define STRUCT_607D218_300_END(s) (&(s)->_0x300[15])

extern struct_0x607D218* _0x606005C;

extern AddSpriteData SpriteAdders[64];

extern int16_t SpriteAdderNameTable[64];

extern uint32_t _0x60618F0[16];

typedef struct struct_0x6061932 {
	SpriteData tempSprite;
	uint16_t _0x10;
	uint16_t _0x12;
	uint16_t _0x14;
	uint16_t _0x16;
	uint16_t _0x18;
} struct_0x6061932;

extern struct_0x6061932 _0x6061932;

#define MAXVIDEOSETTERS 32
uint16_t NumVideoSetters;
typedef struct VideoSetter {
	// NOTE: Video set functions must have three arguments, each of which is either
	// a pointer or intptr_t/uintptr_t.

	void (* set)(void*, void*, void*);
	void* args[3];
} VideoSetter;

VideoSetter VideoSetters[MAXVIDEOSETTERS];

extern struct_0x607D218* _0x6064750;

// SH-2: sizeof(struct_0x60AD228) == 0x68
typedef struct struct_0x60AD228 {
	uint16_t _0x0;
	uint32_t* _0x4[2];
	uint16_t _0xC[2];
	GameBg* _0x10;
	int32_t _0x14;
	uint32_t _0x18[2];
	uint32_t _0x20[2];
	int32_t _0x28[2];
	int16_t _0x30[2];
	int16_t _0x34[2];
	int16_t _0x38[2];
	int16_t _0x3C[2];
	int16_t _0x40[2];
	int16_t _0x46[2];
	int32_t _0x4C;
	int32_t _0x50;
	int16_t _0x54;
	int16_t _0x56;
	int16_t _0x58;
	GameBg* _0x60;
	int32_t _0x64;
} struct_0x60AD228;

extern struct_0x60AD228 _0x60AD228[11];

typedef struct Bg {
	int16_t _0x0;
	int16_t _0x2;
	int16_t _0x4;
	int16_t _0x6;
	int16_t _0x8;
	int16_t _0xA;
	int16_t _0xC;
	int16_t darkness;
	int16_t _0x10;
	int8_t _0x12[2];
	int32_t _0x14;
	int16_t _0x18[11];
	int8_t _0x2E[2];
	int32_t _0x30;
	int32_t _0x34;
	int16_t _0x38[4];
	int32_t _0x40;
	int32_t _0x44;
	int8_t _0x48[4];
	int32_t _0x4C;

	// TODO: Maybe these should be merged into uint32_t _0x50[2][448].
	// These might be used for temporarily storing the background map tile
	// data, before being written to background RAM.
	uint32_t _0x50[448];
	uint32_t _0x750[448];
} Bg;

extern Bg Bgs[4];

void _0x6023788();

void _0x602378C();

void _0x6023790();

void _0x60237DE();

void _0x602392E(int32_t, void (*)());

void _0x60239B8(int32_t, int32_t);

void _0x6023A0E(struct_0x607D218* arg0, void (*arg1)(), uint32_t arg2, uint32_t arg3, uint32_t arg4);

void* _0x6023A98(struct_0x607D218*, void*, void*, void*);

void _0x6023B76(struct_0x607D218*);

void _0x6023BC4(struct_0x607D218*, void (*)());

void _0x6023C3E(struct_0x607D218*, void*);

void* _0x6023C8A(struct_0x607D218*, void*);

int32_t _0x6023CBC(void (*)(), uint32_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);

struct_0x607D218* _0x6023DAE(struct_0x607D218*);

void _0x6023E5A(struct_0x607D218*);

struct_0x607D218* _0x6023EFE(void (**)(), struct_0x607D218*);

void* _0x6023FA4(void (**)(), struct_0x607D218*);

void _0x6024030(struct_0x607D218*);

void _0x602406E();

void _0x60240A8(void*);

// All lesser pause modes are active for a given pause mode. So PAUSEMODE_GAME
// enables pausing of the background, too.
typedef enum PauseMode {
	PAUSEMODE_NOPAUSE = 0,
	PAUSEMODE_BG = 30,
	PAUSEMODE_GAME = 40
} PauseMode;

void SetNextPauseMode(PauseMode nextPauseMode);
void DisablePause();
PauseMode GetNextPauseMode();

void AllocSpriteLayerNames(int16_t layer, int16_t num);
void FreeSpriteLayer(uint16_t layer);
void InitSpriteAlpha();
void _0x602419C();
void _0x6024244();
void _0x60243E8(AddSpriteData* arg0);
void _0x602471C(AddSpriteData* arg0);
void WriteSpriteLayers();
void InitSpriteLayers();
int16_t AllocSpriteAdder();
void _0x6024B78(int16_t);
void _0x6024C00(int16_t);
void _0x6024C3C(int16_t i, int16_t y, int16_t x, const ObjectData* objectTable);
void _0x6024C82(int16_t, void*);
void _0x6024CBC(int16_t, int16_t, int16_t, void*, int16_t);
void _0x6024CF4(int16_t, int16_t, int16_t, void*, int16_t, int16_t, uint16_t);
void _0x6024D74(int16_t, int16_t, int16_t, void*, int16_t, int16_t, uint16_t);
void _0x6024DF0(int16_t);
void _0x6024E5C(int16_t);
void _0x6024ED8();
void _0x6025078();
void _0x60251B8(int16_t, uint32_t*, uint32_t*);
void _0x602523C();
void _0x602526A(void*, void*, void*);
int32_t _0x60257EE();
void _0x602585E(int16_t);
int32_t _0x6025918();
void _0x60259D6(int16_t);
int16_t _0x60259EA(int16_t, int32_t*);
void _0x6025AAC();
int32_t _0x6025AE4(int16_t, GameBg*);
void _0x6025B9A(int16_t, GameBg*, int32_t, int32_t);
void _0x6025BDC(int16_t);
void _0x6025C1E(int16_t, int16_t, int16_t);
void _0x6025DFC();
void _0x6026530(int16_t, void*, int16_t, int16_t, int16_t);
void _0x6026698(int16_t bgNum, void**, void*, int16_t, int16_t, int16_t, int16_t, int16_t);
void _0x60267E4(int16_t);
void _0x602682A(int16_t, int16_t, int16_t, bool);
void _0x6026870(int16_t, int16_t, int16_t);
GameBg* _0x6026AAC(int16_t, int16_t, int16_t*, int16_t*);
// TODO
void _0x6026FCA(int16_t bgIndex, int16_t arg1);
void _0x6026FDC(int16_t, int16_t);
// TODO
void SetBgDarkness(int16_t bgIndex, int16_t darkness);
// TODO
void SetScanlinesBank(uint8_t bankNum); // NOTE: I assume the argument was the bank number, but VideoSetScanlinesBank doesn't used it. -Brandon McGriff
void VideoSetScanlinesBank(void*, void*, void*);
void _0x6029498(int16_t);
// TODO
void _0x60294C0(uint8_t arg0);
void SetBackdropColor(Color color);
void VideoSetBackdropColor(uintptr_t color, void*, void*);
void _0x6029546(int16_t arg0, int16_t arg1, int16_t arg2, int16_t arg3);
void _0x602970C();
void _0x602975E();
// Runs all the video setters.
void SetVideo();
// Disables all video setters.
void InitVideoSetters();
void _0x6029814(uint32_t arg0, uint32_t arg1, uint8_t bgStart, uint8_t bgEnd);
// The current palette is stepped every frame between the pal* arrays: N -> M
// (starting with palN, stepping towards palM). Palette cycles that stop free
// themselves when finished. Up to 32 palette cycles can be active.
typedef enum PalCycleType {
	PALCYCLETYPE_FREE,
	PALCYCLETYPE_UPSTOP = 64, // 0 -> 1, stop.
	PALCYCLETYPE_UPRESTART,   // 0 -> 1, restart, 0 -> 1, and so on.
	PALCYCLETYPE_BOUNCE,      // 0 -> 1, 1 -> 0, and so on.
	PALCYCLETYPE_DOWNRESTART, // 1 -> 0, restart, 1 -> 0, and so on.
	PALCYCLETYPE_DOWNSTOP     // 1 -> 0, stop.
} PalCycleType;
#define MAXPALCYCLES 32
void InitPalCycles();
void UpdatePalCycles();
void NewPalCycle(uint8_t palNum, Color *pal0, Color *pal1, int16_t perPalDelay, PalCycleType type, int8_t stride, uint8_t endStep);
// Pass this to FreePalCycles to free all cycles.
#define FREEALLPALCYCLES 500u
void FreePalCycles(uint16_t palNum);
void DisablePalCycles();
void EnablePalCycles();
// TODO
void SetPal(uint8_t palNum, uint8_t numPals, ROMDATA Color* pal);
// TODO
void _0x602AA16();
void _0x602AA4C();
void _0x602AA64();
void _0x602AB9E();
// TODO
void _0x602AC68(int16_t*);
// NOTE: This should probably only contain functions from AllocSpriteLayerNames to _0x602AC68.
