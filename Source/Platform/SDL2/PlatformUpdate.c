#include "PlatformUpdate.h"
#include "Video/Video.h"
#include "Main/MainLoop.h"
#include "Main/AttractLoop.h"
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
#include "Platform/Util/ini.h"
#include "Platform/Util/Render.h"
#include "PlatformTypes.h"
#include "HwData.h"
#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>

static ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
static ROMDATA Color PalSmallText[NUMPALCOLORS_4BPP];

static int strcmp_nocase(const char *a, const char *b) {
	int cmp;
	while ((cmp = tolower(*a) - tolower(*b)) == 0 && *a++ && *b++);
	return cmp;
}

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

static ini_t* Config = NULL;

static SDL_Window* Window = NULL;
static SDL_Renderer* Renderer = NULL;
static SDL_Texture* FramebufferTexture = NULL;

#define FRAME_DURATION (1.0 / (57272700.0 / 8.0 / 443.0 / 262.0))
static Uint64 CurrentTime;
static Uint64 TimeAccumulator;

static SDL_Keycode InputConfigKeyboard[NUMINPUTS][8];

static SDL_Joystick* Joysticks[NUMPLAYERS];
static int InputConfigJoystickButtons[NUMPLAYERS][NUMINPUTS][8] = {
	{
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 }
	},
	{
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 }
	}
};
static int InputConfigJoystickAxes[NUMPLAYERS][NUMINPUTS][8][2] = {
	{
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } }
	},
	{
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } }
	}
};
static int InputConfigJoystickHats[NUMPLAYERS][NUMINPUTS][8][2] = {
	{
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } }
	},
	{
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } }
	}
};

void ExitHandler(void) {
	ini_free(Config);
	free(TileData);

	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	for (PlayerNum i = PLAYER1; i < lengthof(Joysticks); i++) {
		if (Joysticks[i] && SDL_JoystickGetAttached(Joysticks[i])) {
			SDL_JoystickClose(Joysticks[i]);
		}
	}
	SDL_Quit();
}

bool PlatformInit() {
	// Non-TAP, platform initialization.
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	Config = ini_load("taref.ini");
	if (!Config) {
		exit(EXIT_FAILURE);
	}

	{
		const char* value;
#define GET_KEY(input, button, i) \
		do { \
			value = ini_get(Config, #input "_KEYBOARD", #button); \
			if (value) { \
				InputConfigKeyboard[input][i] = SDL_GetKeyFromName(value); \
			} \
		} while (0)

		GET_KEY(INPUT_BUTTONS1P, BUTTON_START, 0);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_3, 1);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_2, 2);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_1, 3);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_LEFT, 4);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_RIGHT, 5);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_DOWN, 6);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_UP, 7);

		GET_KEY(INPUT_BUTTONS2P, BUTTON_START, 0);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_3, 1);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_2, 2);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_1, 3);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_LEFT, 4);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_RIGHT, 5);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_DOWN, 6);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_UP, 7);

		GET_KEY(INPUT_SERVICE, SERVICE_COIN1, 0);
		GET_KEY(INPUT_SERVICE, SERVICE_COIN2, 1);
		GET_KEY(INPUT_SERVICE, SERVICE_ADDSERVICE, 2);
		GET_KEY(INPUT_SERVICE, SERVICE_TEST, 5);
		GET_KEY(INPUT_SERVICE, SERVICE_TILT, 6);
	}

	if (SDL_NumJoysticks() > 0) {
		int deviceIndex;
		const char* const joystickSections[NUMPLAYERS] = {
			"INPUT_JOYSTICK1P",
			"INPUT_JOYSTICK2P"
		};
		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			SDL_Joystick* joystick = SDL_JoystickOpen(i);
			bool joystickUsed = false;
			if (!joystick) {
				continue;
			}
			for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++) {
				const char* name = NULL;
				if (!Joysticks[playerNum] && (name = ini_get(Config, joystickSections[playerNum], "DEVICE_NAME")) && !strcmp(SDL_JoystickName(joystick), name)) {
					Joysticks[playerNum] = joystick;
					joystickUsed = true;
					break;
				}
			}
			if (!joystickUsed) {
				SDL_JoystickClose(joystick);
			}
		}
		for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++) {
			if (!Joysticks[playerNum] && ini_sget(Config, joystickSections[playerNum], "DEVICE_INDEX", "%d", &deviceIndex) && deviceIndex >= 0 && deviceIndex < SDL_NumJoysticks()) {
				Joysticks[playerNum] = SDL_JoystickOpen(deviceIndex);
			}
		}

		const char* const inputSections[NUMINPUTS] = {
			"INPUT_BUTTONS1P_JOYSTICK",
			"INPUT_BUTTONS2P_JOYSTICK",
			"INPUT_UNUSED_JOYSTICK",
			"INPUT_SERVICE_JOYSTICK"
		};
		const char* const inputKeys[NUMINPUTS][8] = {
			{
				"BUTTON_START",
				"BUTTON_3",
				"BUTTON_2",
				"BUTTON_1",
				"BUTTON_LEFT",
				"BUTTON_RIGHT",
				"BUTTON_DOWN",
				"BUTTON_UP"
			},
			{
				"BUTTON_START",
				"BUTTON_3",
				"BUTTON_2",
				"BUTTON_1",
				"BUTTON_LEFT",
				"BUTTON_RIGHT",
				"BUTTON_DOWN",
				"BUTTON_UP"
			},
			{
				"",
				"",
				"",
				"",
				"",
				"",
				"",
				""
			},
			{
				"SERVICE_COIN1",
				"SERVICE_COIN2",
				"SERVICE_ADDSERVICE",
				"",
				"",
				"SERVICE_TEST",
				"SERVICE_TILT"
				"",
				""
			}
		};
		const char* const devicePlayers[NUMPLAYERS] = {
			"1P",
			"2P"
		};
		for (size_t i = 0u; i < NUMINPUTS; i++) {
			for (size_t j = 0u; j < 8u; j++) {
				const char* const value = ini_get(Config, inputSections[i], inputKeys[i][j]);
				if (value) {
					const size_t len = strlen(value);
					char* fields[4];
					for (size_t i = 0u; i < lengthof(fields); i++) {
						fields[i] = malloc(len + 1u);
						assert(fields[i] != NULL);
					}
					const int fieldsRead = sscanf(value, "%s %s %s %s", fields[0], fields[1], fields[2], fields[3]);

					bool playerNumSet = false;
					PlayerNum playerNum;
					if (fieldsRead >= 1) {
						for (PlayerNum i = PLAYER1; i < NUMPLAYERS; i++) {
							if (!strcmp_nocase(fields[0], devicePlayers[i])) {
								playerNum = i;
								playerNumSet = true;
								break;
							}
						}
					}
					if (playerNumSet && fieldsRead >= 2) {
						if (!strcmp_nocase(fields[1], "Button") && fieldsRead >= 3) {
							int button;
							if (sscanf(fields[2], "%d", &button) == 1 && button >= 0) {
								InputConfigJoystickButtons[playerNum][i][j] = button;
							}
						}
						else if (!strcmp_nocase(fields[1], "Axis") && fieldsRead >= 4) {
							int num;
							float minpos;
							if (sscanf(fields[2], "%d", &num) == 1 && num >= 0 && sscanf(fields[3], "%f%%", &minpos) == 1 && minpos <= 100.0f && minpos >= -100.0f) {
								InputConfigJoystickAxes[playerNum][i][j][0] = num;
								InputConfigJoystickAxes[playerNum][i][j][1] = (Sint16)((minpos / 100.0f) * 32767.0f);
							}
						}
						else if (!strcmp_nocase(fields[1], "Hat") && fieldsRead >= 4) {
							int num;
							if (sscanf(fields[2], "%d", &num) == 1 && num >= 0) {
								if (!strcmp_nocase(fields[3], "Up")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_UP;
								}
								else if (!strcmp_nocase(fields[3], "Right")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHT;
								}
								else if (!strcmp_nocase(fields[3], "Down")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_DOWN;
								}
								else if (!strcmp_nocase(fields[3], "Left")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_LEFT;
								}
								else if (!strcmp_nocase(fields[3], "Rightup")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHTUP;
								}
								else if (!strcmp_nocase(fields[3], "Rightdown")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHTDOWN;
								}
								else if (!strcmp_nocase(fields[3], "Leftup")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_LEFTUP;
								}
								else if (!strcmp_nocase(fields[3], "Leftdown")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_LEFTDOWN;
								}
							}
						}
					}

					for (size_t i = 0u; i < lengthof(fields); i++) {
						free(fields[i]);
					}
				}
			}
		}
	}

	Window = SDL_CreateWindow("The Absolute Reference", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	Renderer = SDL_CreateRenderer(Window, 0, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(Renderer, VIDEO_WIDTH, VIDEO_HEIGHT);
	SDL_SetRenderDrawColor(Renderer, 128u, 128u, 128u, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Renderer);
	SDL_RenderPresent(Renderer);

	FramebufferTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
	SDL_SetTextureBlendMode(FramebufferTexture, SDL_BLENDMODE_NONE);

	{
		TileData = malloc(NUMTILEROMS * TILEROM_SIZE);
		if (!TileData) {
			exit(EXIT_FAILURE);
		}
		uint8_t* const tileDataTemp = malloc(NUMTILEROMS * TILEROM_SIZE);
		if (!tileDataTemp) {
			exit(EXIT_FAILURE);
		}
		for (size_t i = 0u; i < NUMTILEROMS; i++) {
			FILE* const file = fopen(TileRomFileNames[i], "rb");
			if (!file) {
				exit(EXIT_FAILURE);
			}
			fread(&tileDataTemp[i * TILEROM_SIZE], TILEROM_SIZE, 1u, file);
			fclose(file);
		}
		uint8_t* tilePtr = TileData;
		for (size_t i = 0u; i < NUMTILEROMS; i += 2u) {
			const uint8_t* tileLow = &tileDataTemp[(i + 0) * TILEROM_SIZE];
			const uint8_t* tileHigh = &tileDataTemp[(i + 1) * TILEROM_SIZE];
			for (size_t j = 0u; j < TILEROM_SIZE * 2u; j += 4u) {
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
		if (!programData) {
			exit(EXIT_FAILURE);
		}
		uint8_t* programTemp = malloc(PROGRAMROM_SIZE * NUMPROGRAMROMS);
		if (!programTemp) {
			exit(EXIT_FAILURE);
		}
		FILE* programHighFile = fopen("Roms/2b.u21", "rb");
		if (!programHighFile) {
			exit(EXIT_FAILURE);
		}
		FILE* programLowFile = fopen("Roms/1b.u22", "rb");
		if (!programLowFile) {
			exit(EXIT_FAILURE);
		}
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

		memcpy(Pal.header, &programData[ROMOFFSET_PAL], sizeof(Pal.header));
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
			assert(BgMapTable[i]->header.UNK_4 == 0x78u && BgMapTable[i]->header.UNK_6 == 0x50u);

			uint8_t* programPtr = &programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + sizeoffield(BgMapHeader, UNK_4) + sizeoffield(BgMapHeader, UNK_6)];
			if (BgMapTable[i]->header.tileInfo & BGMAPTILEINFO_PERTILEPAL) {
				assert(((BgMap32*)BgMapTable[i])->header.UNK_4 == 0x78u && ((BgMap32*)BgMapTable[i])->header.UNK_6 == 0x50u);
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
				assert(((BgMap16*)BgMapTable[i])->header.UNK_4 == 0x78u && ((BgMap16*)BgMapTable[i])->header.UNK_6 == 0x50u);
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

	CurrentTime = SDL_GetPerformanceCounter();
	TimeAccumulator = 0u;

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
	MainLoop = MAINLOOP_ATTRACT;
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
	UpdateFrame();

	InitSystemTextPal();
	SetBackdropColor(COLOR(0x00, 0x00, 0x00, 0x00));
	SetScanlinesBank(0);
	UNK_6029814(0x0000, 0x0000, 0, 0xFF);
	SetPal(80u, 1u, PALPTR(0x1E1));
	UpdateFrame();

	// TODO: Return init success status, such as returning false if ROMs have
	// the wrong checksum.
	return true;
}

void PlatformUpdateInputs() {
	SDL_PumpEvents();

	if (SDL_QuitRequested()) {
		exit(EXIT_SUCCESS);
	}

	bool pressed[NUMINPUTS][8] = { 0 };
	const Uint8* const keyboardState = SDL_GetKeyboardState(NULL);
	for (size_t i = 0u; i < NUMINPUTS; i++) {
		for (size_t j = 0u; j < 8u; j++) {
			if (InputConfigKeyboard[i][j] != SDLK_UNKNOWN) {
				SDL_Scancode scancode = SDL_GetScancodeFromKey(InputConfigKeyboard[i][j]);
				if (keyboardState[scancode]) {
					pressed[i][j] = true;
				}
			}
		}
	}

	for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++) {
		if (Joysticks[playerNum]) {
			for (size_t i = 0u; i < NUMINPUTS; i++) {
				for (size_t j = 0u; j < 8u; j++) {
					if (InputConfigJoystickButtons[playerNum][i][j] >= 0) {
						if (SDL_JoystickGetButton(Joysticks[playerNum], InputConfigJoystickButtons[playerNum][i][j])) {
							pressed[i][j] = true;
						}
					}
					if (InputConfigJoystickAxes[playerNum][i][j][0] >= 0) {
						int minpos = InputConfigJoystickAxes[playerNum][i][j][1];
						int position = SDL_JoystickGetAxis(Joysticks[playerNum], InputConfigJoystickAxes[playerNum][i][j][0]);
						if (minpos > 0 && position > 0 && position >= minpos) {
							pressed[i][j] = true;
						}
						else if (minpos < 0 && position < 0 && position <= minpos) {
							pressed[i][j] = true;
						}
					}
					if (InputConfigJoystickHats[playerNum][i][j][0] >= 0) {
						if (SDL_JoystickGetHat(Joysticks[playerNum], InputConfigJoystickHats[playerNum][i][j][0]) == InputConfigJoystickHats[playerNum][i][j][1]) {
							pressed[i][j] = true;
						}
					}
				}
			}
		}
	}

	for (size_t i = 0u; i < NUMINPUTS; i++) {
		for (size_t j = 0u; j < 8u; j++) {
			if (pressed[i][j]) {
				INPUTS[i] &= ~(1 << j);
			}
			else {
				INPUTS[i] |= 1 << j;
			}
		}
	}
}

void PlatformFrame() {
	// TODO: Implement more fully. This is just a placeholder to have some level of functionality.
	RandScale += (uint32_t)SDL_GetPerformanceCounter();

	NumVblanks++;

	SDL_Delay(0);
}

static Color framebuffer[VIDEO_HEIGHT * VIDEO_WIDTH];
void PlatformFinishUpdate() {
	const Uint64 gameFrameDuration = (Uint64)(FRAME_DURATION * SDL_GetPerformanceFrequency());
	bool firstFrame = true;

	goto nextFrame;
	while (true) {
		{
			Uint64 newTime = SDL_GetPerformanceCounter();
			Uint64 renderFrameDuration = newTime - CurrentTime;
			if (renderFrameDuration > SDL_GetPerformanceFrequency() / 4u) {
				renderFrameDuration = SDL_GetPerformanceFrequency() / 4u;
			}
			CurrentTime = newTime;

			TimeAccumulator += renderFrameDuration;
		}

		nextFrame:
		if (TimeAccumulator >= gameFrameDuration) {
			TimeAccumulator -= gameFrameDuration;
			return;
		}

		if (firstFrame) {
			Render(framebuffer, TileData);
			void* pixels;
			int pitch;
			SDL_LockTexture(FramebufferTexture, NULL, &pixels, &pitch);
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
				memcpy(&((uint8_t*)pixels)[y * pitch], framebuffer + y * VIDEO_WIDTH, sizeof(Color) * VIDEO_WIDTH);
			}
			SDL_UnlockTexture(FramebufferTexture);
			firstFrame = false;
		}

		SDL_RenderClear(Renderer);

		SDL_RenderCopy(Renderer, FramebufferTexture, NULL, NULL);

		SDL_RenderPresent(Renderer);
		SDL_Delay(1);
	}
}
