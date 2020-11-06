#include "PlatformUpdate.h"
#include "Video/Video.h"
#include "Main/MainLoop.h"
#include "Main/Frame.h"
#include "Input/Input.h"
#include "Sound/Sound.h"
#include "Video/InitScaleRam.h"
#include "Video/Pal.h"
#include "Video/UnknownSprite.h"
#include "Video/SpriteInit.h"
#include "Game/PalNum.h"
#include "Game/Save.h"
#include "Game/Graphics/Entity.h"
#include "Game/Graphics/ShowText.h"
#include "Eeprom/Eeprom.h"
#include "Eeprom/Setting.h"
#include "HwData.h"
#include <stdlib.h>
#include <time.h>

// TODO: Init these from the program ROM.
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

uint32_t PlatformInit() {
	// Non-TAP, platform initialization.
	srand(time(NULL));

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
	UNK_602AC68(SystemGraphicDataPtr->UNK_118);
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
	SetPal(PALNUM_SMALLTEXT, 1, PalSmallText);
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

void PlatformFrame() {
	// TODO: Implement more fully. This is just a placeholder to have some level of functionality.
	RandScale += rand();
	NumVblanks++;
}

int PlatformQuit() {
	// TODO
	return EXIT_SUCCESS;
}
