#include "PlatformUpdate.h"
#include "Video/Video.h"
#include "Main/MainLoop.h"
#include "Main/DemoLoop.h"
#include "Main/GameLoop.h"
#include "Main/Frame.h"
#include "Main/DemoReplayInput.h"
#include "Input/Input.h"
#include "Sound/Sound.h"
#include "Video/InitScaleRam.h"
#include "Video/Pal.h"
#include "Video/UnknownSprite.h"
#include "Video/SpriteInit.h"
#include "Video/Object.h"
#include "Game/PalNum.h"
#include "Game/Save.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/ShowText.h"
#include "Eeprom/Eeprom.h"
#include "Eeprom/Setting.h"
#include "Platform/Util/Render.h"
#include "PlatformTypes.h"
#include "HwData.h"
#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
static ROMDATA Color PalSmallText[NUMPALCOLORS_4BPP];

void InitVideo() {
	UNK_2405FFEA = 0x20u;
	UNK_2405FFEC = 0x0Au;
	UNK_2405FFED = 0x17u;
	VideoSetting[0] = 0x28u;
	VideoSetting[1] = 0xB8u;
	UNK_2405FFF0 = 0x00u;
	UNK_2405FFF1 = 0xAAu;
	GRAPHICSCHECK_SETBANK(0, 0x30);
	IRQCTRL_WRITE(0, 0);
	IRQCTRL_WRITE(0, IRQCTRL_READ(0) | 0x84);
	IRQCTRL_WRITE(1, IRQCTRL_READ(1) | 0x80);
	UNK_2405FFF4 = 0x83u;
	UNK_2405FFF5 = 0xFFu;
	UNK_2405FFF7 = 0x0Eu;
	BgMapSetting[0] = 0x00u;
	BgMapSetting[1] = 0x00u;
	SpritePriority[0] = 0x35u;
	SpritePriority[1] = 0x67u;
	UNK_2405FFEB = 0x87u;
	ScanlinesBank[0] = 0x08u;

	for (size_t i = 0u; i < NUMALPHAS; i++) {
		Alpha[i] = 0x0u;
	}

	for (size_t i = 0u; i < lengthof(SCALERAM); i++) {
		SCALERAM[i] = InitScaleRam[i];
	}

	for (size_t i = 0u; i < 0x100u; i++) {
		for (size_t j = 0u; j < NUMPALCOLORS_4BPP; j++) {
			PALRAM[i * NUMPALCOLORS_4BPP + j] = 0xFFFFFFFFu;
		}
	}

	AddSprite(0u, NULL);
	SpriteNames[0] = 0u;
	SpriteNames[1] = SPRITENAME_TERMINATE;
}

void SetSystemGraphicDataPtr() {
	SystemGraphicDataPtr = (SystemGraphicData*)SequenceDataTablePtr[7];
}

// NOTE: For constant data loaded into RAM, take the RAM address and subtract 0x5FFF860 to get the address in program ROM.

#define TILEROM_SIZE 0x200000
#define NUMTILEROMS 16
uint8_t* TileData;
const char * TileRomFileNames[NUMTILEROMS] = {
    "Roms/81ts_3l.u6",
    "Roms/82ts_3h.u14",
    "Roms/83ts_4l.u7",
    "Roms/84ts_4h.u15",
    "Roms/85ts_5l.u8",
    "Roms/86ts_5h.u16",
    "Roms/87ts_6l.u1",
    "Roms/88ts_6h.u2",
    "Roms/89ts_7l.u19",
    "Roms/90ts_7h.u20",
    "Roms/91ts_8l.u28",
    "Roms/92ts_8h.u29",
    "Roms/93ts_9l.u41",
    "Roms/94ts_9h.u42",
    "Roms/95ts_10l.u58",
    "Roms/96ts_10h.u59"
};

#define PROGRAMROM_SIZE 0x80000
#define NUMPROGRAMROMS 2

#define ROMOFFSET_DEMOREPLAYINPUTTWIN    0x33F87u
#define ROMOFFSET_DEMOREPLAYINPUTDOUBLES 0x3495Fu
#define ROMOFFSET_DEMOREPLAYINPUTVERSUS  0x35337u

#define ROMCOLOR(colorData, offset) COLOR((colorData)[(offset) + 0], (colorData)[(offset) + 1], (colorData)[(offset) + 2], (colorData)[(offset) + 3])
#define ROMOFFSET_PAL1             0x3141Cu
#define ROMOFFSET_PALSMALLTEXT     0x3145Cu
#define ROMOFFSET_UNK_6032884      0x33024u
#define ROMOFFSET_UNK_60328C4      0x33064u
#define ROMOFFSET_PALCYCLETEXTPAL0 0x33EE8u
#define ROMOFFSET_UNK_6033790      0x33F30u

#define ROMOFFSET_PAL 0x60088u

// By matching the offset array's order to the original's pointer array, a simple loop can be used for initialization. -Brandon McGriff
#define ROMOFFSET_BGMAPSECTION2 0x68B8Cu
#define ROMOFFSET_BGMAPSECTION5 0x72194u
#define ROMOFFSET_BGMAPSECTION8 0x76C9Cu
#define ROMOFFSET_BGMAPSECTION3 0x7B7A4u
#define ROMOFFSET_BGMAPSECTION1 0x802ACu
#define ROMOFFSET_BGMAPSECTION0 0x84DB4u
#define ROMOFFSET_BGMAPSECTION4 0x898BCu
#define ROMOFFSET_BGMAPSECTION6 0x92EC4u
#define ROMOFFSET_BGMAPSECTION7 0x979CCu
#define ROMOFFSET_BGMAPSECTION9 0x9C4D4u
#define ROMOFFSET_BGMAPVERSUS   0xA0FDCu
static const size_t BgMapRomOffsets[] = {
	ROMOFFSET_BGMAPSECTION2,
	ROMOFFSET_BGMAPSECTION5,
	ROMOFFSET_BGMAPSECTION8,
	ROMOFFSET_BGMAPSECTION3,
	ROMOFFSET_BGMAPSECTION1,
	ROMOFFSET_BGMAPSECTION0,
	ROMOFFSET_BGMAPSECTION4,
	ROMOFFSET_BGMAPSECTION6,
	ROMOFFSET_BGMAPSECTION7,
	ROMOFFSET_BGMAPSECTION9,
	ROMOFFSET_BGMAPVERSUS
};

#define ROMOFFSET_OBJECTDATA 0xA5D00u

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* framebufferTexture = NULL;

void ExitHandler() {
	free(TileData);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

uint32_t PlatformInit() {
	// Non-TAP, platform initialization.
	srand(time(NULL));

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("The Absolute Reference", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_PRESENTVSYNC);
	framebufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
	SDL_SetTextureBlendMode(framebufferTexture, SDL_BLENDMODE_NONE);

	{
		TileData = malloc(NUMTILEROMS * TILEROM_SIZE);
		uint8_t* const tileDataTemp = malloc(NUMTILEROMS * TILEROM_SIZE);
		for (size_t i = 0u; i < NUMTILEROMS; i++) {
			FILE* const file = fopen(TileRomFileNames[i], "rb");
			fread(&tileDataTemp[i * TILEROM_SIZE], TILEROM_SIZE, 1u, file);
			fclose(file);
		}
		for (size_t i = 0u; i < NUMTILEROMS; i += 2u) {
			const uint8_t* tileLow = &tileDataTemp[(i + 0) * TILEROM_SIZE];
			const uint8_t* tileHigh = &tileDataTemp[(i + 1) * TILEROM_SIZE];
			uint8_t* tilePtr = &TileData[i * TILEROM_SIZE];
			for (size_t j = 0u; j < TILEROM_SIZE; j += 4u) {
				*tilePtr++ = *tileLow++;
				*tilePtr++ = *tileLow++;
				*tilePtr++ = *tileHigh++;
				*tilePtr++ = *tileHigh++;
			}
		}
		free(tileDataTemp);
	}

	{
		uint8_t* programData = malloc(PROGRAMROM_SIZE * NUMPROGRAMROMS);
		uint8_t* programTemp = malloc(PROGRAMROM_SIZE * NUMPROGRAMROMS);
		FILE* programHighFile = fopen("Roms/2b.u21", "rb");
		FILE* programLowFile = fopen("Roms/1b.u22", "rb");
		fread(&programTemp[PROGRAMROM_SIZE * 0], PROGRAMROM_SIZE, 1u, programHighFile);
		fread(&programTemp[PROGRAMROM_SIZE * 1], PROGRAMROM_SIZE, 1u, programLowFile);
		fclose(programHighFile);
		fclose(programLowFile);
		uint8_t* programPtr = programData;
		uint8_t* programHigh = &programTemp[PROGRAMROM_SIZE * 0];
		uint8_t* programLow = &programTemp[PROGRAMROM_SIZE * 1];
		for (size_t i = 0u; i < PROGRAMROM_SIZE * NUMPROGRAMROMS; i += 4u) {
			*programPtr++ = programHigh[1];
			*programPtr++ = programHigh[0];
			programHigh += 2;
			*programPtr++ = programLow[1];
			*programPtr++ = programLow[0];
			programLow += 2;
		}
		free(programTemp);

		for (size_t i = 0u; i < DEMOREPLAY_LENGTH * NUMPLAYERS; i++) {
			DemoReplayInputTwin[i] = programData[ROMOFFSET_DEMOREPLAYINPUTTWIN + i];
			DemoReplayInputDoubles[i] = programData[ROMOFFSET_DEMOREPLAYINPUTDOUBLES + i];
			DemoReplayInputVersus[i] = programData[ROMOFFSET_DEMOREPLAYINPUTVERSUS + i];
		}

		for (size_t i = 0u; i < lengthoffield(PalData, header); i++) {
			putchar(Pal.header[i]);
		}
		for (size_t i = 0u; i < lengthof(Pal.data); i++) {
			Pal.data[i] = ROMCOLOR(&programData[ROMOFFSET_PAL + sizeof(Pal.header)], i * 4u);
		}

		for (size_t i = 0u; i < NUMPALCOLORS_4BPP; i++) {
			Pal1[i] = ROMCOLOR(&programData[ROMOFFSET_PAL1], i * 4u);
			PalSmallText[i] = ROMCOLOR(&programData[ROMOFFSET_PALSMALLTEXT], i * 4u);
			UNK_6032884[i] = ROMCOLOR(&programData[ROMOFFSET_UNK_6032884], i * 4u);
			UNK_60328C4[i] = ROMCOLOR(&programData[ROMOFFSET_UNK_60328C4], i * 4u);
			PalCycleTextPal0[i] = ROMCOLOR(&programData[ROMOFFSET_PALCYCLETEXTPAL0], i * 4u);
			UNK_6033790[i] = ROMCOLOR(&programData[ROMOFFSET_UNK_6033790], i * 4u);
		}

		for (size_t i = 0u; i < lengthof(BgMapTable); i++) {
			BgMapTable[i]->header.tileInfo =
				((uint32_t)programData[BgMapRomOffsets[i] + 0] << 24) |
				((uint32_t)programData[BgMapRomOffsets[i] + 1] << 16) |
				((uint32_t)programData[BgMapRomOffsets[i] + 2] <<  8) |
				((uint32_t)programData[BgMapRomOffsets[i] + 3] <<  0);
			BgMapTable[i]->header.UNK_4 =
				((uint16_t)programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + 0] << 8) |
				((uint16_t)programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + 1] << 0);
			BgMapTable[i]->header.UNK_6 =
				((uint16_t)programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + sizeoffield(BgMapHeader, UNK_4) + 0] << 8) |
				((uint16_t)programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + sizeoffield(BgMapHeader, UNK_4) + 1] << 0);

			uint8_t* programPtr = &programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + sizeoffield(BgMapHeader, UNK_4) + sizeoffield(BgMapHeader, UNK_6)];
			if (BgMapTable[i]->header.tileInfo & BGMAPTILEINFO_PERTILEPAL) {
				uint32_t* name = ((BgMap32*)BgMapTable[i])->names;
				for (size_t i = 0u; i < lengthoffield(BgMap32, names); i++, name++, programPtr += sizeof(uint32_t)) {
					*name =
						((uint32_t)programPtr[0] << 24) |
						((uint32_t)programPtr[1] << 16) |
						((uint32_t)programPtr[2] <<  8) |
						((uint32_t)programPtr[3] <<  0);
				}
			}
			else {
				uint16_t* name = ((BgMap16*)BgMapTable[i])->names;
				for (size_t i = 0u; i < lengthoffield(BgMap16, names); i++, name++, programPtr += sizeof(uint16_t)) {
					*name =
						((uint16_t)programPtr[0] << 8) |
						((uint16_t)programPtr[1] << 0);
				}
			}
		}

		memcpy(Objects.header, &programData[ROMOFFSET_OBJECTDATA], sizeof(Objects.header));
		for (size_t i = 0u; i < lengthof(Objects.data); i++) {
			for (size_t j = 0u; j < 6u; j++) {
				Objects.data[i][j] =
					((uint16_t)programData[ROMOFFSET_OBJECTDATA + sizeof(Objects.header) + i * sizeof(ObjectData) + j * sizeof(uint16_t) + 0u] << 8) |
					           programData[ROMOFFSET_OBJECTDATA + sizeof(Objects.header) + i * sizeof(ObjectData) + j * sizeof(uint16_t) + 1u];
			}
		}

		free(programData);
	}
	atexit(ExitHandler);

	// TAP initialization.
	// TODO: Implement more of the initialization like the original, such as ROM checks.
	UNK_602ACB0();
	ScreenTime = 0u;
	NumVideoSetters = 0u;
	InitSound();
	UNK_6023790();
	InitCredits();
	InitVideo();
	UNK_6024ED8();
	UNK_60169DC();
	InitEntities();
	InitPalCycles();
	InitSpriteAdders();
	SetSystemGraphicDataPtr();
	InitTodaysBestRankings();
	Uptime = 0u;
	UNK_606564C = 0u; // Set, but never used.
	DisablePause();
	MainLoop = MAINLOOP_DEMO;
    //UNK_602AC68(SystemGraphicDataPtr->UNK_118); // TODO: Sequence code. Seems to only be necessary in test mode.
	SetPal(1u, 1u, Pal1);
	UNK_602BC50(0u);
	SetPal(1u, 15u, PALPTR(0xEB));
	SetPal(158u, 1u, PALPTR(0x20F));
	SetPal(149u, 7u, PALPTR(0x207));
	SetPal(156, 1u, PALPTR(0x204));
	SetBackdropColor(COLOR(0x00, 0x00, 0x00, 0x00));
	SetScanlinesBank(0u);
	UNK_602F8BC();
	LoadPlayStatus();
	LoadRankings();
	for (size_t section = 0u; section < 10u; section++) {
		BestMasterSectionTimes[section] = RANKINGDATA_GETVALUE(Save->rankings[RANKINGINDEX_MASTERSECTIONTIMES + section].data);
		BestTaDeathSectionTimes[section] = TIME(0, 42, 0);
	}

	if (Settings[SETTING_SCREENMODE] == SCREENMODE_FLIP) {
		VideoSetting[0] |= 0xC0u; // Set horizontal/vertical screen flip.
	}

	Save->programChecksum = 0u; // TODO: Actually load and check this with LoadProgramChecksum.
	SetPal(0u, 1, PalSmallText);
	SetPal(PALNUM_CHECKSUMNG, 1, Pal1);
	SetPal(PALNUM_SYSTEMTEXT, 1, PAL_SYSTEMTEXT);

	InitSystemTextPal();

	SetBackdropColor(COLOR(0x00, 0x00, 0x00, 0x00));
	SetScanlinesBank(0);
	UNK_6029814(0x0000, 0x0000, 0, 0xFF);
	SetPal(80u, 1u, PALPTR(0x1E1));

	// TODO: Return init success status, such as returning false if ROMs have
	// the wrong checksum.
	return 1u;
}

void PlatformUpdateInputs() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit(EXIT_SUCCESS);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_q: INPUTS[INPUT_BUTTONS1P] &= ~BUTTON_START; break;
			case SDLK_c: INPUTS[INPUT_BUTTONS1P] &= ~BUTTON_3; break;
			case SDLK_x: INPUTS[INPUT_BUTTONS1P] &= ~BUTTON_2; break;
			case SDLK_z: INPUTS[INPUT_BUTTONS1P] &= ~BUTTON_1; break;
			case SDLK_a: INPUTS[INPUT_BUTTONS1P] &= ~BUTTON_LEFT; break;
			case SDLK_d: INPUTS[INPUT_BUTTONS1P] &= ~BUTTON_RIGHT; break;
			case SDLK_s: INPUTS[INPUT_BUTTONS1P] &= ~BUTTON_DOWN; break;
			case SDLK_w: INPUTS[INPUT_BUTTONS1P] &= ~BUTTON_UP; break;

			case SDLK_r: INPUTS[INPUT_BUTTONS2P] &= ~BUTTON_START; break;
			case SDLK_n: INPUTS[INPUT_BUTTONS2P] &= ~BUTTON_3; break;
			case SDLK_b: INPUTS[INPUT_BUTTONS2P] &= ~BUTTON_2; break;
			case SDLK_v: INPUTS[INPUT_BUTTONS2P] &= ~BUTTON_1; break;
			case SDLK_f: INPUTS[INPUT_BUTTONS2P] &= ~BUTTON_LEFT; break;
			case SDLK_h: INPUTS[INPUT_BUTTONS2P] &= ~BUTTON_RIGHT; break;
			case SDLK_g: INPUTS[INPUT_BUTTONS2P] &= ~BUTTON_DOWN; break;
			case SDLK_t: INPUTS[INPUT_BUTTONS2P] &= ~BUTTON_UP; break;

			case SDLK_e: INPUTS[INPUT_SERVICE] &= ~SERVICE_COIN1; break;
			case SDLK_y: INPUTS[INPUT_SERVICE] &= ~SERVICE_COIN2; break;
			case SDLK_TAB: INPUTS[INPUT_SERVICE] &= ~SERVICE_ADDSERVICE; break;
			case SDLK_ESCAPE: INPUTS[INPUT_SERVICE] &= ~SERVICE_TEST; break;
			case SDLK_SPACE: INPUTS[INPUT_SERVICE] &= ~SERVICE_TILT; break;

			default: break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_q: INPUTS[INPUT_BUTTONS1P] |= BUTTON_START; break;
			case SDLK_c: INPUTS[INPUT_BUTTONS1P] |= BUTTON_3; break;
			case SDLK_x: INPUTS[INPUT_BUTTONS1P] |= BUTTON_2; break;
			case SDLK_z: INPUTS[INPUT_BUTTONS1P] |= BUTTON_1; break;
			case SDLK_a: INPUTS[INPUT_BUTTONS1P] |= BUTTON_LEFT; break;
			case SDLK_d: INPUTS[INPUT_BUTTONS1P] |= BUTTON_RIGHT; break;
			case SDLK_s: INPUTS[INPUT_BUTTONS1P] |= BUTTON_DOWN; break;
			case SDLK_w: INPUTS[INPUT_BUTTONS1P] |= BUTTON_UP; break;

			case SDLK_r: INPUTS[INPUT_BUTTONS2P] |= BUTTON_START; break;
			case SDLK_n: INPUTS[INPUT_BUTTONS2P] |= BUTTON_3; break;
			case SDLK_b: INPUTS[INPUT_BUTTONS2P] |= BUTTON_2; break;
			case SDLK_v: INPUTS[INPUT_BUTTONS2P] |= BUTTON_1; break;
			case SDLK_f: INPUTS[INPUT_BUTTONS2P] |= BUTTON_LEFT; break;
			case SDLK_h: INPUTS[INPUT_BUTTONS2P] |= BUTTON_RIGHT; break;
			case SDLK_g: INPUTS[INPUT_BUTTONS2P] |= BUTTON_DOWN; break;
			case SDLK_t: INPUTS[INPUT_BUTTONS2P] |= BUTTON_UP; break;

			case SDLK_e: INPUTS[INPUT_SERVICE] |= SERVICE_COIN1; break;
			case SDLK_y: INPUTS[INPUT_SERVICE] |= SERVICE_COIN2; break;
			case SDLK_TAB: INPUTS[INPUT_SERVICE] |= SERVICE_ADDSERVICE; break;
			case SDLK_ESCAPE: INPUTS[INPUT_SERVICE] |= SERVICE_TEST; break;
			case SDLK_SPACE: INPUTS[INPUT_SERVICE] |= SERVICE_TILT; break;

			default: break;
			}
			break;

		default: break;
		}
	}
}

void PlatformFrame() {
	// TODO: Implement more fully. This is just a placeholder to have some level of functionality.
	RandScale += rand();
	NumVblanks++;
}

static Color framebuffer[VIDEO_HEIGHT * VIDEO_WIDTH];
void PlatformFinishUpdate() {
	SDL_RenderClear(renderer);

	Render(framebuffer, TileData);

	void* pixels;
	int pitch;
	SDL_LockTexture(framebufferTexture, NULL, &pixels, &pitch);
	for (size_t y = 0u; y < VIDEO_HEIGHT; y++) {
		// Being able to use memcpy here is why the code uses the Color typedef and
		// COLOR* macros. The game code and frontend have matching pixel formats, so we
		// don't have to do any conversion. The SDL2 texture rows *might* be longer than
		// VIDEO_WIDTH though, so we can't optimize it to a whole-framebuffer copy, but
		// this is pretty good. We also can't directly pass the texture pixels pointer
		// to the Render function, because the Render function has to read the
		// framebuffer for alpha blending, and SDL2 doesn't let us read the pixels of a
		// locked texture.
		// -Brandon McGriff
		memcpy(pixels + y * pitch, framebuffer + y * VIDEO_WIDTH, sizeof(Color) * VIDEO_WIDTH);
	}
	SDL_UnlockTexture(framebufferTexture);

	SDL_RenderCopy(renderer, framebufferTexture, NULL, NULL);

	SDL_RenderPresent(renderer);
}

int PlatformQuit() {
	// TODO
	return EXIT_SUCCESS;
}
