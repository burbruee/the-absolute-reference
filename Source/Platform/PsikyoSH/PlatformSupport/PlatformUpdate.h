// TODO: Fix these includes.
#include "Game/Main.h"
#include "Game/Screen.h"
#include "Game/Player.h"
#include "Game/Save.h"
#include "Game/Ranking.h"
#include "Game/Entity.h"
#include "Game/Input.h"
#include "Game/ShowText.h"
#include "Game/Debug.h"
#include "Game/BuildData.h"
#include "Game/RegionWarning.h"
#include "Game/Fixed.h"
#include "Game/Setting.h"
#include "Game/Sound.h"
#include "Game/Frame.h"
#include "Game/Button.h"
#include "Game/ShowText.h"
#include "Game/ShowObject.h"
#include "Game/SpriteInit.h"
#include "Game/UnknownSprite.h"
#include "Game/Pal.h"
#include "Game/PalNum.h"
#include "Game/Loop.h"
#include "Game/Checksum.h"
#include "Game/HwData.h"
#include "Game/MemCheck.h"
#include "Game/Macros.h"
#include "Platform/Util/Macros.h"
#include <stdint.h>
#include <stdbool.h>

// TODO: Init these from the program ROM.
ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
ROMDATA Color PalSmallText[NUMPALCOLORS_4BPP];

// If this is set to REGIONWARNING_SKIP and the EEP-ROM region isn't Japan,
// then the warning screen won't be shown.
static RegionWarningState RegionWarning = REGIONWARNING_JAPAN;

void InitVideo();
void SetSystemGraphicDataPtr();
void _0x6000AEC();

extern const char* RegionMessageJapan[1];
extern const char* RegionMessageUsaCanada[1];
extern const char* RegionMessageKorea[1];
extern const char* RegionMessageHongKong[1];
extern const char* RegionMessageTaiwan[1];

static inline void PlatformInit() {
	SystemGraphicData* graphicData = (SystemGraphicData*)SequenceDataTablePtr[7];

	// System init.
	_0x602ACB0();
	ScreenTime = 0u;
	NumVideoSetters = 0u;
	InitSound();
	PlaySoundEffect(SOUNDEFFECT_READY);
	_0x6023790();
	InitCredits();
	InitVideo();
	_0x6024ED8();
	_0x60169DC();
	InitEntities();
	InitPalCycles();
	_0x602419C();
	SetSystemGraphicDataPtr();
	InitTodaysBestRankings();
	Uptime = 0u;
	_0x606564C = 0u; // Set, but never used.
	DisablePause();
	MainLoop = MAINLOOP_DEMO;
	_0x602AC68(SystemGraphicDataPtr->_0x118);
	SetPal(1u, 1u, Pal1);
	_0x602BC50(0u);
	SetPal(1u, 15u, PALPTR(0xEB));
	SetPal(158u, 1u, PALPTR(0x20F));
	SetPal(149u, 7u, PALPTR(0x207));
	SetPal(156, 1u, PALPTR(0x204));
	SetBackdropColor(COLOR(0x00, 0x00, 0x00, 0x00));
	SetScanlinesBank(0u);

	// Reset save data if the tilt DIP switch is active and 1P button 1 is
	// pressed.
	if ((~INPUTS[INPUT_SERVICE] & SERVICE_TILT) && (~INPUTS[INPUT_BUTTONS1P] & BUTTON_1)) {
		InitSettings();
		InitPlayStatus();
		InitRankings();
		SaveSettings();
		SavePlayStatus();
		SaveRankings();
		SaveProgramChecksum(MemCheckData[MEMCHECK_PROGRAMCHECKSUM]);
	}

	_0x602F8BC();
	MemCheckData[MEMCHECK_EEPROM] = SettingsValid();
	MemCheckData[MEMCHECK_EEPROM] &= LoadPlayStatus();
	MemCheckData[MEMCHECK_EEPROM] &= LoadRankings();
	for (size_t section = 0u; section < 10u; section++) {
		BestMasterSectionTimes[section] = RANKINGDATA_GETVALUE(Save->rankings[RANKINGINDEX_MASTERSECTIONTIMES + section].data);
		BestTaDeathSectionTimes[section] = TIME(0, 42, 0);
	}

	if (Settings[SETTING_SCREENMODE] == SCREENMODE_FLIP) {
		VideoSettings[0] |= 0xC0u; // Set horizontal/vertical screen flip.
	}

	if (!LoadProgramChecksum(MemCheckData[MEMCHECK_PROGRAMCHECKSUM]) && (~INPUTS[INPUT_SERVICE] & SERVICE_TILT)) {
		// Nothing here. There was probably code here that was
		// commented out in the released version of TAP.
		// TODO: Maybe TGM2 has the code?
	}

	uint32_t memCheckDuration;
	bool memCheckOkay =
		MemCheckData[MEMCHECK_WORKRAM] &
		MemCheckData[MEMCHECK_GRAPHICSRAM] &
		MemCheckData[MEMCHECK_PALETTERAM] &
		MemCheckData[MEMCHECK_SCALERAM] &
		MemCheckData[MEMCHECK_EEPROM];
	if (memCheckOkay) {
		memCheckDuration = TIME(0, 10, 0);
	}
	else {
		memCheckDuration = TIME(0, 1, 0);
	}

	bool RegionUSACanada = false;
	if ((EEPROM[0] & REGION_SETTING) == REGION_USACANADA) {
		RegionUSACanada = true;
	}
	if (RegionWarning == REGIONWARNING_SKIP && (EEPROM[0] & REGION_SETTING) == REGION_JAPAN) {
		RegionWarning = REGIONWARNING_JAPAN;
	}

	SetPal(PALNUM_SMALLTEXT, 1, PalSmallText);
	SetPal(PALNUM_10, 1, Pal1);
	SetPal(PALNUM_SYSTEMTEXT, 1, PAL_SYSTEMTEXT);

	bool charSoundMemCheck = false;
	uint32_t *workRamStatus = &MemCheckData[MEMCHECK_WORKRAM];
	uint32_t *graphicsRamStatus = &MemCheckData[MEMCHECK_GRAPHICSRAM];
	uint32_t *paletteRamStatus = &MemCheckData[MEMCHECK_PALETTERAM];
	uint32_t *scaleRamStatus = &MemCheckData[MEMCHECK_SCALERAM];
	uint32_t *eepRomStatus = &MemCheckData[MEMCHECK_EEPROM];

	// Show memory check status.
	//
	// If the tilt DIP switch is active, player 1 start can be pressed to
	// show the character and sound ROMs' memory check statuses.
	//
	// If one or more of the memory checks failed, player 1 start, button
	// 3, button 2, or button 1 can be pressed to immediately exit the
	// memory check screen instead of waiting 10 seconds, regardless of
	// whether the tilt DIP switch is active. The game still starts
	// normally after the memory check screen, though.
	// TODO: The character ROM pairs are named "char0000" to "char0007". Sound
	// ROM is just "sound".
	while (charSoundMemCheck ? _0x60302C4() : memCheckDuration != 0) {
		if ((SystemButtonsDown[PLAYER1] & BUTTON_START) && (~INPUTS[INPUT_SERVICE] & SERVICE_TILT)) {
			charSoundMemCheck = true;
		}

		UpdateFrame();

		_0x602BB0C();
		InitSystemTextPal();
		if (memCheckDuration != 0u) memCheckDuration--;

		if ((SystemButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) && memCheckOkay) {
			memCheckDuration = 0u;
		}

		#define SHOWMEMCHECKSTATUS(status, labelText, row) \
		if (status) { \
			ShowSystemText(20 * (row + 1), 65, (labelText), false); \
			ShowSystemSpecialText(146, 20 * (row + 1), "O", false); \
		} \
		else { \
			ShowSystemText(20 * (row + 1), 65, (labelText), false); \
			ShowSystemSpecialText(146, 20 * (row + 1), "X", false); \
		}
		SHOWMEMCHECKSTATUS(*workRamStatus, "WORK RAM", 0);
		SHOWMEMCHECKSTATUS(*graphicsRamStatus & *paletteRamStatus & *scaleRamStatus, "VIDEO RAM", 1);
		SHOWMEMCHECKSTATUS(*eepRomStatus, "EEP-ROM", 2);
		#undef SHOWMEMCHECKSTATUS
	}

	SetBackdropColor(COLOR(0x00, 0x00, 0x00, 0x00));
	SetScanlinesBank(0);
	int16_t spriteAdderName = AllocSpriteAdder();
	_0x6024C3C(spriteAdderName, 60, 266, graphicData->objectTable);

	if (INPUTS[INPUT_SERVICE] & SERVICE_TEST) {
		switch (RegionWarning) {
			case REGIONWARNING_JAPAN:
				_0x6029814(0x7F00u, 0x7F00u, 0u, 0xFFu);

				for (uint32_t numWarningFrames = 0u; numWarningFrames < 500u; numWarningFrames++) {
					UpdateFrame();

					if (
						(
							(
								(ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) ||
								(ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))
							) &&
							numWarningFrames > 60u
						) ||
						!(INPUTS[INPUT_SERVICE] & SERVICE_TEST)
						) {
						numWarningFrames = 600u;
					}

					ShowSystemTextCentered(30, "- Warning! -", 0u);
					int16_t y = 60;
					const char **line = RegionMessageJapan;
					for (size_t lineNum = 0; lineNum < lengthof(RegionMessageJapan); lineNum++, y += 20, line++) {
						ShowSystemText(55, y, *line, false);
					}
				}
				break;

			case REGIONWARNING_SKIP:
				if ((EEPROM[0] & REGION_SETTING) == REGION_JAPAN) {
					RegionWarning = REGIONWARNING_JAPAN;

					_0x6029814(0x7F00, 0x7F00, 0, 0xFF);

					for (uint32_t numWarningFrames = 0u; numWarningFrames < 500u; numWarningFrames++) {
						UpdateFrame();

						if ((((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) && numWarningFrames > 60u) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
							numWarningFrames = 600u;
						}

						ShowSystemTextCentered(30, "- Warning! -", 0u);
						int16_t y = 60;
						const char **line = RegionMessageJapan;
						for (size_t lineNum = 0; lineNum < lengthof(RegionMessageJapan); lineNum++, y += 20, line++) {
							ShowSystemText(55, y, *line, false);
						}
					}
				}
				break;

			case REGIONWARNING_KOREA:
				_0x6029814(0x7F00, 0x7F00, 0, 0xFF);

				for (uint32_t numWarningFrames = 0u; numWarningFrames < 500u; numWarningFrames++) {
					UpdateFrame();

					if ((((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) && numWarningFrames > 60u) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
						numWarningFrames = 600u;
					}

					ShowSystemTextCentered(30, "- Warning! -", 0u);
					int16_t y = 60;
					const char **line = RegionMessageKorea;
					for (size_t lineNum = 0; lineNum < lengthof(RegionMessageKorea); lineNum++, y += 20, line++) {
						ShowSystemText(55, y, *line, false);
					}
				}
				break;

			case REGIONWARNING_REGIONSETTING:
				_0x6029814(0x7F00, 0x7F00, 0, 0xFF);

				for (uint32_t numWarningFrames = 0u; numWarningFrames < 500u; numWarningFrames++) {
					UpdateFrame();

					if ((((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) && numWarningFrames > 60u) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
						numWarningFrames = 600u;
					}

					switch (EEPROM[0] & REGION_SETTING) {
						case REGION_USACANADA:
							SpriteAdders[spriteAdderName]._0x2C &= 0x7FFF;

							int16_t y = 46;
							const char **line = RegionMessageUsaCanada;
							for (size_t lineNum = 0; lineNum < lengthof(RegionMessageUsaCanada); lineNum++, y += 24, line++) {
								_0x602B7D8(*line, y, 40);
							}
							break;

						case REGION_KOREA:
							SpriteAdders[spriteAdderName]._0x2C &= 0x7FFF;

							int16_t y = 46;
							const char **line = RegionMessageKorea;
							for (size_t lineNum = 0; lineNum < lengthof(RegionMessageKorea); lineNum++, y += 24, line++) {
								_0x602B7D8(*line, y, 40);
							}
							break;

						case REGION_HONGKONG:
							SpriteAdders[spriteAdderName]._0x2C &= 0x7FFF;

							int16_t y = 46;
							const char **line = RegionMessageHongKong;
							for (size_t lineNum = 0; lineNum < lengthof(RegionMessageHongKong); lineNum++, y += 24, line++) {
								_0x602B7D8(*line, y, 40);
							}
							break;

						case REGION_TAIWAN:
							SpriteAdders[spriteAdderName]._0x2C &= 0x7FFF;

							int16_t y = 46;
							const char **line = RegionMessageTaiwan;
							for (size_t lineNum = 0; lineNum < lengthof(RegionMessageTaiwan); lineNum++, y += 24, line++) {
								_0x602B7D8(*line, y, 40);
							}
							break;

						default:
							break;
					}
				}
				break;

			default:
				break;
		}
		SpriteAdders[spriteAdderName]._0x2C |= 0x8000;

		SetScanlinesBank(0u);
		_0x6029814(0x0000, 0x0000, 0, 0xFF);
		SetPal(80u, 1u, PALPTR(0x1E1));

		// TODO: Rename "frames" once it's understood what this is showing.
		for (uint32_t frames = 0u; frames < 500u; frames++) {
			UpdateFrame();

			ShowObject(OBJECTPTR(0x26B), 104, 32, 80u, 125u);
			ShowObject(OBJECTPTR(0x269), 0, 0, 80u, 125u);

			if ((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
				frames = 600u;
			}
		}
		_0x6024B78(spriteAdderName);
		UpdateFrame();
	}
}
#define PLATFORM_INIT() PlatformInit()

static inline void PlatformFrame() {
	while (PauseAllowed) {
		UpdateInputs();
		RandScale = 0u;
		NumVblanks = 0u;
		while (IRQCTRL[1] & 1) {
			RandScale++;
		}
		VblankFinished = false;
		while (!VblankFinished) {
			RandScale++;
		}
		VblankFinished = false;
		if (SystemButtonsDown[PLAYER1] & BUTTON_START) {
			UpdatePlayers();
			break;
		}
	}
	_0x602BA70();
	RandScale = 0u;
	while (IRQCTRL[1] & 1) {
		RandScale++;
	}
	VblankFinished = false;
	while (!VblankFinished) {
		RandScale++;
	}
	SetVideo();
	_0x602AA64();
	UpdatePalCycles();
	VblankFinished = false;
	IRQCTRL[1] |= 1;
}
#define PLATFORM_FRAME() PlatformFrame()

#define PLATFORM_QUIT() while (true)
