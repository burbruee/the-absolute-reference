#include "Main.h"
#include "Screen.h"
#include "Player.h"
#include "Save.h"
#include "Ranking.h"
#include "Entity.h"
#include "Input.h"
#include "ShowText.h"
#include "Debug.h"
#include "BuildData.h"
#include "RegionWarning.h"
#include "Fixed.h"
#include "Setting.h"
#include "Sound.h"
#include "Frame.h"
#include "Button.h"
#include "ShowText.h"
#include "ShowObject.h"
#include "SpriteInit.h"
#include "UnknownSprite.h"
#include "Pal.h"
#include "PalNum.h"
#include "Loop.h"
#include "Checksum.h"
#include "HwData.h"
#include "MemCheck.h"
#include "Platform/Util/Macros.h"
#include <stdint.h>
#include <stdbool.h>

// TODO: Init these from the program ROM.
ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
ROMDATA Color PalSmallText[NUMPALCOLORS_4BPP];

// If this is set to REGIONWARNING_SKIP and the EEP-ROM region isn't Japan,
// then the warning screen won't be shown.
static RegionWarningState RegionWarning = REGIONWARNING_JAPAN;

static void InitVideo();
static void SetSystemGraphicDataPtr();
static void _0x6000AEC();

// These messages are just the bare minimum to get across what the original
// messages said. The original messages aren't copied here, as the rights
// holder of them might claim they're copyrighted. They're still all arrays of
// strings, though, matching the format of the original messages. The original
// messages just warned that the game should only be used in the region set in
// EEP-ROM.
//
// I assume these are just standard messages provided by Psikyo for all games
// on the PsikyoSH boards, and their presence doesn't necessarily imply Arika
// planned to release TAP in non-Japan regions. -Brandon McGriff
static const char* RegionMessageJapan[] = {
	"Japan region."
};
static const char* RegionMessageUsaCanada[] = {
	"USA and Canada region."
};
static const char* RegionMessageKorea[] = {
	"Korea region."
};
static const char* RegionMessageHongKong[] = {
	"Hong Kong region."
};
static const char* RegionMessageTaiwan[] = {
	"Taiwan region."
};

// TODO: Change to there not being port-specific main() functions to a setup
// where some extra functions are added for ports (like a Startup() function
// called at the top of main()), with those extras being #define'd out for
// PsikyoSH, etc. Those entry points to the port-specific code would go into
// Platform/PlatformFunctions.h.
// Also, for SDL2, it will be single-threaded, with
// timing/input/rendering/audio done where the code waits on vsync.
int main() {
	SystemGraphicData* graphicData = (SystemGraphicData*)(*SequenceDataTablePtr)[7];

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

	Screen = SCREEN_COPYRIGHT;
	RunMainLoop();
	return 0;
}

static const uint16_t InitScaleRam[0x100] = {
	0xFFFF, 0x8000, 0x5555, 0x4000, 0x3333, 0x2AAB, 0x2492, 0x2000, 0x1C72, 0x199A, 0x1746, 0x1555, 0x13B1, 0x1249, 0x1111, 0x1000,
	0x0F0F, 0x0E39, 0x0D79, 0x0CCD, 0x0C31, 0x0BA3, 0x0B21, 0x0AAB, 0x0A3D, 0x09D9, 0x097B, 0x0925, 0x08D4, 0x0889, 0x0842, 0x0800,
	0x07C2, 0x0788, 0x0750, 0x071C, 0x06EB, 0x06BD, 0x0690, 0x0666, 0x063E, 0x0618, 0x05F4, 0x05D1, 0x05B0, 0x0591, 0x0572, 0x0555,
	0x0539, 0x051F, 0x0505, 0x04EC, 0x04D5, 0x04BE, 0x04A8, 0x0492, 0x047E, 0x046A, 0x0457, 0x0444, 0x0432, 0x0421, 0x0410, 0x0400,
	0x03F0, 0x03E1, 0x03D2, 0x03C4, 0x03B6, 0x03A8, 0x039B, 0x038E, 0x0382, 0x0376, 0x036A, 0x035E, 0x0353, 0x0348, 0x033E, 0x0333,
	0x0329, 0x031F, 0x0316, 0x030C, 0x0303, 0x02FA, 0x02F1, 0x02E9, 0x02E0, 0x02D8, 0x02D0, 0x02C8, 0x02C1, 0x02B9, 0x02B2, 0x02AB,
	0x02A4, 0x029D, 0x0296, 0x028F, 0x0289, 0x0283, 0x027C, 0x0276, 0x0270, 0x026A, 0x0264, 0x025F, 0x0259, 0x0254, 0x024E, 0x0249,
	0x0244, 0x023F, 0x023A, 0x0235, 0x0230, 0x022B, 0x0227, 0x0222, 0x021E, 0x0219, 0x0215, 0x0211, 0x020C, 0x0208, 0x0204, 0x0200,
	0x01FC, 0x01F8, 0x01F4, 0x01F0, 0x01ED, 0x01E9, 0x01E5, 0x01E2, 0x01DE, 0x01DB, 0x01D7, 0x01D4, 0x01D1, 0x01CE, 0x01CA, 0x01C7,
	0x01C4, 0x01C1, 0x01BE, 0x01BB, 0x01B8, 0x01B5, 0x01B2, 0x01AF, 0x01AC, 0x01AA, 0x01A7, 0x01A4, 0x01A1, 0x019F, 0x019C, 0x019A,
	0x0197, 0x0195, 0x0192, 0x0190, 0x018D, 0x018B, 0x0188, 0x0186, 0x0184, 0x0182, 0x017F, 0x017D, 0x017B, 0x0179, 0x0176, 0x0174,
	0x0172, 0x0170, 0x016E, 0x016C, 0x016A, 0x0168, 0x0166, 0x0164, 0x0162, 0x0160, 0x015E, 0x015D, 0x015B, 0x0159, 0x0157, 0x0155,
	0x0154, 0x0152, 0x0150, 0x014E, 0x014D, 0x014B, 0x0149, 0x0148, 0x0146, 0x0144, 0x0143, 0x0141, 0x0140, 0x013E, 0x013D, 0x013B,
	0x013A, 0x0138, 0x0137, 0x0135, 0x0134, 0x0132, 0x0131, 0x012F, 0x012E, 0x012D, 0x012B, 0x012A, 0x0129, 0x0127, 0x0126, 0x0125,
	0x0123, 0x0122, 0x0121, 0x011F, 0x011E, 0x011D, 0x011C, 0x011A, 0x0119, 0x0118, 0x0117, 0x0116, 0x0115, 0x0113, 0x0112, 0x0111,
	0x0110, 0x010F, 0x010E, 0x010D, 0x010B, 0x010A, 0x0109, 0x0108, 0x0107, 0x0106, 0x0105, 0x0104, 0x0103, 0x0102, 0x0101, 0x0100,
};

static void InitVideo() {
	_0x2405FFEA = 0x20u;
	_0x2405FFEC = 0x0Au;
	_0x2405FFED = 0x17u;
	VideoSettings[0] = 0x28u;
	VideoSettings[1] = 0xB8u;
	_0x2405FFF0 = 0x00u;
	_0x2405FFF1 = 0xAAu;
	VideoGraphicsCheckBank[0] = 0x30u;
	IRQCTRL[0] = 0u;
	IRQCTRL[0] |= 0x84u;
	IRQCTRL[1] |= 0x80u;
	_0x2405FFF4 = 0x83u;
	_0x2405FFF5 = 0xFFu;
	_0x2405FFF7 = 0x0Eu;
	BgMapSettings[0] = 0x00u;
	BgMapSettings[1] = 0x00u;
	SpritePriority[0] = 0x35u;
	SpritePriority[1] = 0x67u;
	_0x2405FFEB = 0x87u;
	ScanlinesBank[0] = 0x08u;

	for (size_t i = 0u; i < NUMALPHAVALUES; i++) {
		AlphaValues[i] = 0x0u;
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

static void SetSystemGraphicDataPtr() {
	SystemGraphicDataPtr = (SystemGraphicData*)SequenceDataTablePtr[7];
}

static void _0x6000AEC() {}
