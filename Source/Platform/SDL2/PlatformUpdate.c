#include "PlatformUpdate.h"
#include "Video/Video.h"
#include "Main/MainLoop.h"
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
#include "HwData.h"
#include <stdio.h>
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

// TODO: Enable tile ROM loading once rendering is implemented.
//#define LOAD_TILEROMS
#ifdef LOAD_TILEROMS
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
#endif

// NOTE: For constant data loaded into RAM, take the RAM address and subtract 0x5FFF860 to get the address in program ROM.
#define ROMOFFSET_DEMOREPLAYINPUTTWIN 0x33F87u
#define ROMOFFSET_DEMOREPLAYINPUTDOUBLES 0x3495Fu
#define ROMOFFSET_DEMOREPLAYINPUTVERSUS 0x35337u
#define ROMOFFSET_OBJECTDATA 0xA5D00u

uint32_t PlatformInit() {
	// Non-TAP, platform initialization.
	srand(time(NULL));

#ifdef LOAD_TILEROMS
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
			uint8_t* tile = &TileData[i * TILEROM_SIZE];
			for (size_t j = 0u; j < TILEROM_SIZE; j += 4u) {
				*tile++ = *tileLow++;
				*tile++ = *tileLow++;
				*tile++ = *tileHigh++;
				*tile++ = *tileHigh++;
			}
		}
		free(tileDataTemp);
	}
#endif

	{
		uint8_t* programData = malloc(0x80000 * 2);
		FILE* upperFile = fopen("Roms/2b.u21", "rb");
		FILE* lowerFile = fopen("Roms/1b.u22", "rb");
		uint8_t* upperData = malloc(0x80000);
		uint8_t* lowerData = malloc(0x80000);
		fread(upperData, 0x80000u, 1u, upperFile);
		fread(lowerData, 0x80000u, 1u, lowerFile);
		fclose(upperFile);
		fclose(lowerFile);
		uint8_t* programPtr = programData;
		uint8_t* upperPtr = upperData;
		uint8_t* lowerPtr = lowerData;
		for (size_t i = 0u; i < 0x80000u * 2u; i += 4u) {
			*programPtr++ = upperPtr[1];
			*programPtr++ = upperPtr[0];
			upperPtr += 2;
			*programPtr++ = lowerPtr[1];
			*programPtr++ = lowerPtr[0];
			lowerPtr += 2;
		}
		free(upperData);
		free(lowerData);

		for (size_t i = 0u; i < DEMOREPLAY_LENGTH * NUMPLAYERS; i++) {
			DemoReplayInputTwin[i] = programData[ROMOFFSET_DEMOREPLAYINPUTTWIN + i];
			DemoReplayInputDoubles[i] = programData[ROMOFFSET_DEMOREPLAYINPUTDOUBLES + i];
			DemoReplayInputVersus[i] = programData[ROMOFFSET_DEMOREPLAYINPUTVERSUS + i];
		}

		memcpy(Objects.header, &programData[ROMOFFSET_OBJECTDATA], lengthof(Objects.header));
		for (size_t i = 0u; i < lengthof(Objects.data); i++) {
			for (size_t j = 0u; j < 6u; j++) {
				Objects.data[i][j] =
					((uint16_t)programData[ROMOFFSET_OBJECTDATA + sizeof(Objects.header) + i * sizeof(ObjectData) + j * sizeof(uint16_t) + 0u] << 8) |
					           programData[ROMOFFSET_OBJECTDATA + sizeof(Objects.header) + i * sizeof(ObjectData) + j * sizeof(uint16_t) + 1u];
			}
		}

		free(programData);
	}

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
