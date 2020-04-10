#pragma once

#include "VideoDefs.h"
#include "Unknown.h"
#include "HwData.h"
#include "HwSprite.h"
#include "PlatformTypes.h"
#include <stdint.h>
#include <stdbool.h>

// TODO: Background data, and a lot more.

// The number of sprites written to the sprite table.
extern int16_t NumSprites;

extern uint32_t _0x60618F0[16];

extern SpriteData TempSprite;

extern unknown_type* _0x6064750;

void AllocSpriteLayerNames(int16_t layer, uint16_t num);
void FreeSpriteLayer(uint16_t layer);
void InitSpriteAlpha();
struct struct_0x606006C;
typedef struct struct_0x606006C struct_0x606006C;
void _0x602419C();
void _0x6024244();
void _0x60243E8(struct_0x606006C* arg0);
void _0x602471C(struct_0x606006C* arg0);
void WriteSpriteLayers();
void InitSpriteLayers();
int16_t _0x6024B0C();
void _0x6024B78(int16_t);
void _0x6024C00(int16_t);
void _0x6024C3C(int16_t, int16_t, int16_t, void*);
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
void _0x602526A();
int32_t _0x60257EE();
void _0x602585E(int16_t);
int32_t _0x6025918();
void _0x60259D6(int16_t);
int16_t _0x60259EA(int16_t, int32_t*);
void _0x6025AAC();
int32_t _0x6025AE4(int16_t, int32_t*);
void _0x6025B9A(int16_t, int32_t*, int32_t, int32_t);
void _0x6025BDC(int16_t);
void _0x6025C1E(int16_t, int16_t, int16_t);
void _0x6025DFC();
// TODO
void _0x6026FCA(uint16_t, uint16_t);
// TODO
void _0x60294C0(uint8_t arg0);
// TODO
void VideoSetBackdropColor(uintptr_t color, void* unused1, void* unused2);
// TODO
// Runs all the video setters.
void SetVideo();
// Disables all video setters.
void ResetVideoSetters();
void _0x6029814(uint32_t arg0, uint32_t arg1, uint8_t arg2, uint8_t arg3);
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
void NewPalCycle(uint8_t palNum, Color *pal0, Color *pal1, int16_t perPalDelay, PalCycleType type, uint8_t stride, uint8_t endStep);
// Pass this to FreePalCycles to free all cycles.
#define FREEALLPALCYCLES NUMPALS
void FreePalCycles(uint16_t palNum);
void DisablePalCycles();
void EnablePalCycles();
// TODO
void SetPal(uint8_t palNum, uint8_t numPals, const Color* pal);
// TODO
void _0x602AA4C();
void _0x602AA64();
void _0x602AB9E();
// TODO
// NOTE: This should probably only contain functions from AllocSpriteLayerNames to _0x602AC68.
