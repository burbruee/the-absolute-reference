#pragma once

#include "Video/GameBg.h"
#include "Video/VideoDefs.h"
#include "Video/HwSprite.h"
#include "Game/Graphics/DisplayObject.h"
#include "Lib/Fixed.h"
#include "PlatformTypes.h"
#include "HwData.h"
#include <stdint.h>
#include <stdbool.h>

extern const int16_t UNK_60356C8[4];

typedef enum AddSpriteType {
	ADDSPRITE_0,
	ADDSPRITE_1,
	ADDSPRITE_2,
	ADDSPRITE_3,
	ADDSPRITE_4,
	ADDSPRITE_5,
	ADDSPRITE_ANIM,
	ADDSPRITE_7,
	ADDSPRITE_8,
	ADDSPRITE_9,
	ADDSPRITE_10,
	ADDSPRITE_11
} AddSpriteType;

// TODO: Could be data for setting sprites with AddSprite and other functions
// (so name it SetSpriteData). Might only be for setting one sprite at a time,
// unlike the object functions that write multiple sprites for a single object.
// For AddSprite, data either points to one ObjectData for a static
// sprite or an ObjectData array with one ObjectData per animation frame.
// SH-2: sizeof(AddSpriteData) == 0x40
typedef struct AddSpriteData {
	const ObjectData* objectTable;
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
	int16_t UNK_20;

	uint16_t verticalHorizontal;
	int16_t UNK_28;
	uint8_t layer;
	AddSpriteType type;
	uint16_t UNK_2C;
	uint8_t UNK_2E;
	uint8_t UNK_2F;
	int16_t UNK_30;
	int16_t UNK_32;
	int16_t lineHeight;
	uint16_t UNK_36;
	int16_t UNK_38;
} AddSpriteData;

// The number of sprites written to Sprites.
extern int16_t NumSprites;

typedef struct STRUCT_607D218_300 {
	void (*UNK_0[4])();
} STRUCT_607D218_300;

typedef struct STRUCT_607D218_300_0 {
	void (*UNK_0)();
	uint16_t UNK_4[5];
} STRUCT_607D218_300_0;

typedef struct STRUCT_607D218 STRUCT_607D218;
struct STRUCT_607D218 {
	STRUCT_607D218* UNK_0;
	STRUCT_607D218* UNK_4;
	STRUCT_607D218* UNK_8;
	STRUCT_607D218* UNK_C;
	STRUCT_607D218* UNK_10;
	STRUCT_607D218* UNK_14; // NOTE: Appears to be a linked list next node pointer in UNK_60237DE.
	uint16_t UNK_18;
	uint16_t UNK_1A;
	uint8_t UNK_1C[4];
	Color backdropColor;
	int16_t UNK_24;
	Fixed32 UNK_28;
	Fixed32 UNK_2C;
	uint16_t UNK_30; // TODO: Might be bool.
	int16_t UNK_52;
	uint32_t UNK_80[4];
	void* UNK_100[4][32];
	void* UNK_300[16 * 4];
};

#define STRUCT_607D218_300_END(s) (&(s)->UNK_300[15])

extern STRUCT_607D218* UNK_606005C;

extern AddSpriteData SpriteAdders[64];

extern int16_t SpriteAdderNameTable[64];

extern uint32_t UNK_60618F0[16];

typedef struct STRUCT_6061932 {
	SpriteData tempSprite;
	uint16_t UNK_10;
	uint16_t UNK_12;
	uint16_t UNK_14;
	uint16_t UNK_16;
	uint16_t UNK_18;
} STRUCT_6061932;

extern STRUCT_6061932 UNK_6061932;

#define MAXVIDEOSETTERS 32
extern uint16_t NumVideoSetters;
typedef struct VideoSetter {
	// NOTE: Video set functions must have three arguments, each of which is either
	// a pointer or intptr_t/uintptr_t.

	void (* set)(void*, void*, void*);
	void* args[3];
} VideoSetter;

extern VideoSetter VideoSetters[MAXVIDEOSETTERS];

extern STRUCT_607D218* UNK_6064750;

// SH-2: sizeof(STRUCT_60AD228) == 0x68
typedef struct STRUCT_60AD228 {
	uint16_t UNK_0;
	RAMDATA uint32_t* UNK_4[2];
	int16_t UNK_C[2];
	STRUCT_GameBg_0* UNK_10;
	int32_t UNK_14;
	int32_t UNK_18[2];
	int32_t UNK_20[2];
	int32_t UNK_28[2];
	int16_t UNK_30[2];
	int16_t UNK_34[2];
	int16_t UNK_38[2];
	int16_t UNK_3C[2];
	int16_t UNK_40[2];
	int16_t UNK_44;
	int16_t UNK_46[2];
	int16_t UNK_4A;
	int32_t UNK_4C;
	int32_t UNK_50;
	int16_t UNK_54;
	int16_t UNK_56;
	int16_t UNK_58;
	int16_t UNK_5A[2];
	STRUCT_GameBg_0* UNK_60;
	int32_t UNK_64;
} STRUCT_60AD228;

extern STRUCT_60AD228 UNK_60AD228[11];

struct Bg;
typedef struct Bg {
	int16_t UNK_0;
	int16_t UNK_2;
	int16_t UNK_4;
	int16_t UNK_6;
	int16_t UNK_8;
	int16_t UNK_A;
	int16_t UNK_C;
	int16_t darkness;
	int16_t UNK_10;
	int8_t UNK_12[2];
	int32_t UNK_14;
	int16_t UNK_18[11];
	int8_t UNK_2E[2];
	void (*UNK_30)(struct Bg*);
	void (*UNK_34)(struct Bg*);
	int16_t UNK_38[4];
	int32_t UNK_40;
	int32_t UNK_44;
	int16_t UNK_48;
	int32_t UNK_4C;
	uint32_t UNK_50[3][224];
	uint16_t UNK_AD0[2][224];
} Bg;

extern Bg Bgs[4];

void UNK_6023788(Bg*);

void UNK_602378C();

// Inits the STRUCT_607D218 data.
void UNK_6023790();

void UNK_60237DE();

void UNK_602392E(uintptr_t, void (*)());

void UNK_60239B8(uintptr_t, uintptr_t);

bool UNK_6023A0E(STRUCT_607D218* arg0, void (*arg1)(), uint32_t arg2, uint32_t arg3, uint32_t arg4);

void UNK_6023A98(STRUCT_607D218*, void (*)(), void*, void*);

void UNK_6023B76(STRUCT_607D218*);

void UNK_6023BC4(STRUCT_607D218*, void (*)());

void UNK_6023C3E(STRUCT_607D218*, void**);

void* UNK_6023C8A(STRUCT_607D218*, void*);

uint32_t UNK_6023CBC(void (*)(), uint32_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);

STRUCT_607D218* UNK_6023DAE(STRUCT_607D218*);

void UNK_6023E5A(STRUCT_607D218*);

STRUCT_607D218* UNK_6023EFE(void (**)(), STRUCT_607D218*);

void* UNK_6023FA4(void**, STRUCT_607D218*);

void UNK_6024030(STRUCT_607D218*);

void UNK_602406E();

void UNK_60240A8(void*);

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
void InitSpriteAdders();
void UNK_6024244();
void UNK_60243E8(AddSpriteData* arg0);
void UNK_602471C(AddSpriteData* arg0);
void WriteSpriteLayers();
void InitSpriteLayers();
int16_t AllocSpriteAdder();
void FreeSpriteAdder(int16_t i);
void UNK_6024C00(int16_t);
void UNK_6024C3C(int16_t spriteAdderName, int16_t y, int16_t x, const ObjectData* objectTable);
void UNK_6024C82(int16_t spriteAdderName, const ObjectData* objectTable);
void UNK_6024CBC(int16_t spriteAdderName, int16_t y, int16_t x, const ObjectData* objectTable, int16_t animFrame);
void UNK_6024CF4(int16_t spriteAdderName, int16_t y, int16_t x, const ObjectData* objectTable, int16_t animFrame, int16_t arg5, uint16_t arg6);
void UNK_6024D74(int16_t, int16_t, int16_t, const ObjectData*, int16_t, int16_t, uint16_t);
bool UNK_6024DF0(int16_t);
void UNK_6024E5C(int16_t);

// TODO: Could be InitBgs().
void UNK_6024ED8();

void UNK_6025078();
void UNK_60251B8(int16_t, uint32_t*, uint32_t*);
void UNK_602523C();
void UNK_602526A(void*, void*, void*);
int32_t UNK_60257EE();
void UNK_602585E(int16_t);
int16_t UNK_6025918();
void UNK_60259D6(int16_t);
int16_t UNK_60259EA(int16_t, STRUCT_GameBg_0*);
void UNK_6025AAC(int16_t, STRUCT_GameBg_0*, int32_t, int32_t);
int32_t UNK_6025AE4(int16_t, GameBg*);
void UNK_6025B9A(int16_t, GameBg*, int32_t, int32_t);
void UNK_6025BDC(int16_t);
void UNK_6025C1E(int16_t, int16_t, int16_t);
void UNK_6025DFC(int16_t);
void UNK_6026530(int16_t, STRUCT_60AD228*, int16_t, int16_t, int16_t);
void UNK_6026698(int16_t bgNum, STRUCT_GameBg_0*, STRUCT_60AD228*, int16_t, int16_t, int16_t, int16_t, int16_t);
void UNK_60267E4(int16_t bgIndex, int32_t);
void UNK_602682A(int16_t, int16_t, int16_t, bool);
void UNK_6026870(int16_t bgIndex, int16_t, int16_t bank);
STRUCT_GameBg_0* UNK_6026AAC(int16_t, int16_t, int16_t*, int16_t*);
//UNK_6026BBA

typedef struct STRUCT_6026D40 {
	uint16_t UNK_0;
	uint16_t UNK_2;
	uint8_t UNK_4;
	uint8_t UNK_6;
	uint8_t UNK_8;
	uint8_t UNK_9;
	uint16_t UNK_A;
} STRUCT_6026D40;
void UNK_6026BBA(int16_t, STRUCT_6026D40*, int32_t, int32_t);

//UNK_6026F1E
//UNK_6026F68
void UNK_6026FCA(int16_t bgIndex, int16_t arg1);
void UNK_6026FDC(int16_t, int16_t);
//UNK_6026FEE
//UNK_6027000
void SetBgDarkness(int16_t bgIndex, int16_t darkness);
//UNK_602703E
//UNK_6027050
//UNK_60270E2
//UNK_6027140
//UNK_60273D0
//UNK_60276F0
//UNK_6027700
//UNK_6027710
//UNK_602790A
//UNK_6027BB4
//UNK_6027F60
//UNK_602820E
//UNK_6028212
//UNK_6028408
//UNK_60285E6
//UNK_6028868
//UNK_6028ABA
//UNK_6028D36
//UNK_6028F36
void SetRastersBank(uint8_t bankNum); // NOTE: I assume the argument was the bank number, but VideoSetRastersBank doesn't use it. -Brandon McGriff
void VideoSetRastersBank(void*, void*, void*);
void UNK_6029498(int16_t);
//UNK_6029498
void UNK_60294C0(void* arg0, void* unused1, void* unused2);
void SetOverlayRastersColor(Color color);
void UNK_6029546(int16_t arg0, int16_t arg1, int16_t arg2, int16_t arg3);
// Runs all the video setters.
void SetVideo();
// Disables all video setters.
void InitVideoSetters();
void UNK_6029814(uint32_t arg0, uint32_t arg1, uint8_t bgStart, uint8_t bgEnd);
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
void NewPalCycle(uint8_t palNum, const Color *pal0, const Color *pal1, int16_t perPalDelay, PalCycleType type, int8_t stride, uint8_t endStep);
// Pass this to FreePalCycles to free all cycles.
#define FREEALLPALCYCLES 500u
void FreePalCycles(uint16_t palNum);
void DisablePalCycles();
void EnablePalCycles();
//UNK_602A45E
//UNK_602A4CC
void SetPal(const uint8_t palNum, const uint8_t numPals, const Color* pal);
void SetPalReverse(const uint8_t palNum, const uint8_t numPals, const Color* pal);
void SetPalList(uint8_t palNum, const Color** palList);
void UNK_602AA16();
void UNK_602AA4C();
void UNK_602AA64();
void UNK_602AB9E();
//UNK_602ABB8
//UNK_602AC1E

typedef struct STRUCT_602AC68 {
	uint16_t UNK_0;
	uint16_t palNum;
	const Color** palList;
} STRUCT_602AC68;

void UNK_602AC68(const STRUCT_602AC68*);
// NOTE: This should probably only contain functions from AllocSpriteLayerNames to UNK_602AC68.
