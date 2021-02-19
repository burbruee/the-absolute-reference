#include "Platform/Util/AccessEeprom.h"
#include "Platform/Util/AccessConfig.h"
#include "Platform/Util/AccessData.h"
#include "Platform/SDL2/AccessDisplay.h"
#include "Platform/Util/Render.h"
#include "Video/Pal.h"
#include "Game/PalNum.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/ShowText.h"
#include "Game/Save.h"
#include "Video/Video.h"
#include "Video/UnknownSprite.h"
#include "Main/Frame.h"
#include "Main/InitVideo.h"
#include "Main/MainLoop.h"
#include "Input/Input.h"
#include "PlatformUpdate.h"
#include "SDL.h"
#include "physfs.h"
#include <stdio.h>
#include <stdbool.h>

static void ExitHandler(void) {
	printf("Starting shutdown.\n\n");
	CloseDisplay();
	CloseConfig();
	SaveEeprom();
	CloseData();
	if (!PHYSFS_deinit()) {
		fprintf(stderr, "Failed PhysicsFS deinit: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
	SDL_Quit();
	printf("Finished shutdown, exiting.\n");
}

static bool Init(int argc, char** argv) {
	// Non-TAP, platform initialization.
	printf("Starting SDL2 platform init.\n\n");

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		fprintf(stderr, "Error with SDL_Init: %s\n", SDL_GetError());
		return false;
	}

	if (!PHYSFS_init(argv[0])) {
		fprintf(stderr, "Error with PHYSFS_init: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	if (!PHYSFS_setSaneConfig("nightmareci", "taref", "ZIP", 0, 0)) {
		fprintf(stderr, "Error setting sane PhysicsFS config: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	printf("Search path directories:\n");
	for (char** searchPath = PHYSFS_getSearchPath(); searchPath != NULL && *searchPath != NULL; searchPath++) {
		printf("%s\n", *searchPath);
	}
	printf("\n");

	if (!OpenData()) {
		fprintf(stderr, "Failed opening data\n");
		return false;
	}

	if (!OpenEeprom()) {
		fprintf(stderr, "Failed opening EEP-ROM save data\n");
		return false;
	}

	if (!OpenConfig()) {
		fprintf(stderr, "Failed opening configuration\n");
		return false;
	}

	if (!OpenDisplay()) {
		fprintf(stderr, "Failed opening display\n");
		return false;
	}

	atexit(ExitHandler);

	PlatformCurrentTime = SDL_GetPerformanceCounter();
	PlatformTimeAccumulator = 0u;

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
	SetPal(156u, 1u, PALPTR(0x204));
	SetOverlayRastersColor(COLOR(0x00, 0x00, 0x00, 0x00));
	SetRastersBank(0u);
	ReadSettings();
	LoadPlayStatus();
	LoadRankings();
	for (size_t section = 0u; section < 10u; section++) {
		BestMasterSectionTimes[section] = RANKINGDATA_GETVALUE(Save.rankings[RANKINGINDEX_MASTERSECTIONTIMES + section].data);
		BestTaDeathSectionTimes[section] = TIME(0, 42, 0);
	}

	if (Settings[SETTING_SCREENMODE] == SCREENMODE_REVERSE) {
		VideoSetting[0] |= 0xC0u; // Set horizontal/vertical screen flip.
	}

	Save.programChecksum = 0u; // TODO: Actually load and check this with LoadProgramChecksum.
	SetPal(0u, 1, PalSmallText);
	SetPal(PALNUM_CHECKSUMNG, 1, Pal1);
	SetPal(PALNUM_SYSTEMTEXT, 1, PAL_SYSTEMTEXT);
	UpdateFrame();

	InitSystemTextPal();
	SetOverlayRastersColor(COLOR(0x00, 0x00, 0x00, 0x00));
	SetRastersBank(0);
	UNK_6029814(0x0000, 0x0000, 0, 0xFF);
	SetPal(80u, 1u, PALPTR(0x1E1));
	UpdateFrame();

	printf("Finished game init.\n\n");

	// TODO: Return init success status, such as returning false if ROMs have
	// the wrong checksum.
	return true;
}

int main(int argc, char** argv) {
	if (!Init(argc, argv)) {
		return EXIT_FAILURE;
	}
	else {
		printf("Starting game.\n\n");
	}

	Screen = SCREEN_COPYRIGHT;
	RunMainLoop();

	return EXIT_SUCCESS;
}
