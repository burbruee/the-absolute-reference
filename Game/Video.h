#pragma once

#include "VideoDefs.h"
#include "HwData.h"
#include "HwSprite.h"
#include "PlatformTypes.h"
#include <stdint.h>
#include <stdbool.h>

// TODO: Background code, and a lot more.

// The number of sprites written to the sprite table.
extern int16_t NumSprites;

extern uint32_t _0x60618F0[16];

extern SpriteData TempSprite;

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

// Runs all the video setters.
void SetVideo();

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

void SetPal(uint8_t palNum, uint8_t numPals, const Color* pal);

// NOTE: This should probably only contain functions from 0x6024128 to 0x602ACE0.
