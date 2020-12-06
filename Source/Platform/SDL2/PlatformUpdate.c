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
#include "Platform/SDL2/Display.h"
#include "Platform/Util/Render.h"
#include "Platform/Util/Config.h"
#include "Platform/Util/Data.h"
#include "PlatformTypes.h"
#include "HwData.h"
#include "SDL.h"
#include "physfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>

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

	for (size_t i = 0u; i < NUMPALS; i++) {
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

void ExitHandler(void) {
	printf("Starting shutdown.\n\n");
	CloseDisplay();
	CloseData();
	CloseConfig();
	if (!PHYSFS_deinit()) {
		fprintf(stderr, "Failed PhysicsFS deinit: %s\n", PHYSFS_getLastError());
	}
	SDL_Quit();
	printf("Finished shutdown, exiting.\n");
}

static Uint64 CurrentTime;
static Uint64 TimeAccumulator;

bool PlatformInit(const int argc, const char* const* const argv) {
	// Non-TAP, platform initialization.
	printf("Starting SDL2 platform init.\n\n");
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		fprintf(stderr, "Error with SDL_Init: %s\n", SDL_GetError());
		return false;
	}

	if (!PHYSFS_init(argv[0])) {
		fprintf(stderr, "Error with PHYSFS_init: %s\n", PHYSFS_getLastError());
		return false;
	}

	if (!PHYSFS_setSaneConfig("nightmareci", "taref", "ZIP", 0, 0)) {
		fprintf(stderr, "Error setting sane PhysicsFS config: %s\n", PHYSFS_getLastError());
		return false;
	}

	printf("Search path directories:\n");
	for (char** searchPath = PHYSFS_getSearchPath(); searchPath != NULL && *searchPath != NULL; searchPath++) {
		printf("%s\n", *searchPath);
	}
	printf("\n");

	if (!OpenConfig("taref.ini")) {
		fprintf(stderr, "Failed opening INI \"taref.ini\"\n");
		return false;
	}

	if (!OpenData()) {
		fprintf(stderr, "Failed opening data\n");
		return false;
	}

	if (!OpenDisplay()) {
		fprintf(stderr, "Failed opening display\n");
		return false;
	}

	atexit(ExitHandler);

	CurrentTime = SDL_GetPerformanceCounter();
	TimeAccumulator = 0u;

	printf("Finished SDL2 platform init.\n\n");

	// TAP initialization.
	// TODO: Implement more of the initialization like the original, such as ROM checks.
	printf("Starting game init.\n\n");
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

	printf("Finished game init.\n\n");

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

#define FRAME_DURATION (1.0 / (57272700.0 / 8.0 / 443.0 / 262.0))

static Color Framebuffer[VIDEO_HEIGHT * VIDEO_WIDTH];
void PlatformFinishUpdate() {
	const Uint64 gameFrameDuration = (Uint64)(FRAME_DURATION * SDL_GetPerformanceFrequency());

	if (TimeAccumulator < gameFrameDuration || VsyncUpdateRate) {
		Render(Framebuffer, TileData);
		void* pixels;
		int pitch;
		if (SDL_LockTexture(FramebufferTexture, NULL, &pixels, &pitch) < 0) {
			fprintf(stderr, "Failed locking framebuffer texture: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
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
			memcpy((uint8_t*)pixels + y * pitch, Framebuffer + y * VIDEO_WIDTH, sizeof(Color) * VIDEO_WIDTH);
		}
		SDL_UnlockTexture(FramebufferTexture);
	}

	while (TimeAccumulator < gameFrameDuration || VsyncUpdateRate) {
		if (SDL_RenderClear(Renderer) < 0) {
			fprintf(stderr, "Failed clearing screen: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}

		if (SDL_RenderCopy(Renderer, FramebufferTexture, NULL, NULL) < 0) {
			fprintf(stderr, "Failed copying frame buffer texture to the screen: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}

		SDL_RenderPresent(Renderer);
		if (!Vsync) {
			SDL_Delay(1);
		}

		if (VsyncUpdateRate) {
			return;
		}

		Uint64 newTime = SDL_GetPerformanceCounter();
		Uint64 renderFrameDuration = newTime - CurrentTime;
		if (renderFrameDuration > SDL_GetPerformanceFrequency() / 4u) {
			renderFrameDuration = SDL_GetPerformanceFrequency() / 4u;
		}
		CurrentTime = newTime;

		TimeAccumulator += renderFrameDuration;
	}

	TimeAccumulator -= gameFrameDuration;
}
