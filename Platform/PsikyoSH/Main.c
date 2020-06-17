#include "Game/Player.h"
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
#include "Game/SpriteInit.h"
#include "Game/HwData.h"
#include "Platform/Util/Macros.h"
#include <stdint.h>
#include <stdbool.h>

RegionWarning CurrentRegionWarning;

static void InitVideo();
static void _0x6000AE0();
static void _0x6000AEC();

// TODO: Lots of undefined things here.
// TODO: Change to there not being port-specific main() functions to a setup
// where some extra functions are added for ports (like a Startup() function
// called at the top of main()), with those extras being #define'd out for
// PsikyoSH, etc. Those entry points to the port-specific code would go into
// Platform/PlatformFunctions.h.
// Also, for SDL2, it will be single-threaded, with
// timing/input/rendering/audio done where the code waits on vsync.
void main() {
	bool canExitMemCheck = false;
	void* sequenceEntry = (*BuildData.sequence)[7];

	// System init.
	_0x602ACB0();
	NumScreenFrames = 0u;
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
	_0x6000AE0(); // TODO
	InitTodaysBestRankings();
	_0x6065644 = 0u;
	_0x606564C = 0u;
	InitNextPauseMode();
	CurrentLoop = LOOP_DEMO;
	_0x602AC68((*_0x607926C)->_0x118);
	SetPal(1u, 1u, _0x6030C7C);
	_0x602BC50(0u);
	SetPal(1u, 15u, _0x63B50);
	SetPal(158u, 1u, _0x68450);
	SetPal(149u, 7u, _0x68250);
	SetPal(156, 1u, _0x68190);
	// TODO: Might be better named SetBackdropColor, if no back scanlines
	// color setting is available.
	SetFrontScanlinesColor(COLOR(0x00, 0x00, 0x00));
	InitScanlinesBank(0u);

	// Save data reset, if the tilt DIP switch is active, and player 1 button 1 is pressed.
	if ((~INPUTS[INPUT_SERVICE] & SERVICE_TILT) && (~INPUTS[INPUT_BUTTONS1P] & BUTTON_1)) {
		_0x600ABD2();
		InitPlayStatus();
		InitBestScore();
		_0x600ABEE();
		BackupPlayStatus();
		ForceSaveRankings();
		BackupROMChecksum((uint16_t)MemCheckData[MEMCHECK_ROMCHECKSUM]);
	}

	_0x602F8BC();
	MemCheckData[4] = _0x600AB74();
	MemCheckData[4] &= RestorePlayStatus();
	MemCheckData[4] &= RestoreBestScore();
	for (size_t section = 0u; section < 10u; section++) {
		BestMasterSectionTimes[section] = Save.rankings[RANKINGINDEX_MASTERSECTIONTIMES + section].data & 0xFFFFF;
		TADeathSectionTimes[section] = TIME(0, 42, 0);
	}

	if (Settings[SETTING_SCREEN] == SCREENSETTING_REVERSE) {
		VideoRegisters.screen |= 0xC0; // Set horizontal/vertical screen flip.
	}

	if (!RestoreROMChecksum((uint16_t)MemCheckData[MEMCHECK_ROMCHECKSUM]) && (~INPUTS[INPUT_SERVICE] & SERVICE_TILT)) {
		// Nothing here. There was probably code here that was
		// commented out in the released version of TAP.
		// TODO: Maybe TGM2 has the code?
	}

	uint32_t numMemCheckFrames;
	if ((MemCheckData[MEMCHECK_WORKRAM] & MemCheckData[MEMCHECK_GRAPHICSRAM] & MemCheckData[MEMCHECK_PALETTERAM] & MemCheckData[MEMCHECK_SCALERAM] & MemCheckData[MEMCHECK_EEPROM]) != MEMCHECKSTATUS_NOGOOD) {
		numMemCheckFrames = TIME(0, 1, 0);
	}
	else {
		numMemCheckFrames = TIME(0, 10, 0);
		canExitMemCheck = true;
	}

	RegionUSACanada = false;
	if ((EEPROM[0] & REGION_SETTING) == REGION_USACANADA) {
		RegionUSACanada = true;
	}
	if (CurrentRegionWarning == REGIONWARNING_SKIP && (EEPROM[0] & REGION_SETTING) == REGION_JAPAN) {
		CurrentRegionWarning = REGIONWARNING_JAPAN;
	}

	SetPal(0, 1, _0x6030CBC);
	SetPal(10, 1, _0x6030C7C);
	SetPal(81, 1, _0x67910);

	bool charSoundMemCheck = false;
	uint32_t *workRAMStatus = &MemCheckData[MEMCHECK_WORKRAM];
	uint32_t *graphicsRAMStatus = &MemCheckData[MEMCHECK_GRAPHICSRAM];
	uint32_t *paletteRAMStatus = &MemCheckData[MEMCHECK_PALETTERAM];
	uint32_t *scaleRAMStatus = &MemCheckData[MEMCHECK_SCALERAM];
	uint32_t *eepROMStatus = &MemCheckData[MEMCHECK_EEPROM];

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
	// TODO: The character ROM pairs are named char0000 to char0007. Sound
	// ROM is just "sound".
	if (charSoundMemCheck ? _0x60302C4() : numMemCheckFrames != 0) {
		if ((SystemButtonsDown[PLAYER1] & BUTTON_START) && (~INPUTS[INPUT_SERVICE] & SERVICE_TILT)) {
			charSoundMemCheck = true;
		}

		UpdateFrame();

		_0x602BB0C();
		_0x600DC38();
		if (numMemCheckFrames != 0u) numMemCheckFrames--;

		if ((SystemButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) && canExitMemCheck) {
			numMemCheckFrames = 0u;
		}

		const char *memCheckOK;
		#define SHOWMEMCHECKSTATUS(status, labelText, row) \
		if ((status) != MEMCHECKSTATUS_NOGOOD) { \
			ShowSystemText(memCheckLabelX, memCheckY, (labelText), false); \
			memCheckOK = "O"; \
		} \
		else { \
			ShowSystemText(memCheckLabelX, memCheckY, (labelText), false); \
			memCheckOK = "X"; \
		} \
		_0x600DDA0(146, 20 + 20 * row, memCheckOK, false);
		SHOWMEMCHECKSTATUS(*workRAMStatus, "WORK RAM", 0);
		SHOWMEMCHECKSTATUS(*graphicsRAMStatus & *paletteRAMStatus & *scaleRAMStatus, "VIDEO RAM", 1);
		SHOWMEMCHECKSTATUS(*eepROMStatus, "EEP-ROM", 2);
	}

	SetFrontScanlinesColor(COLOR(0x00, 0x00, 0x00));
	InitScanlinesBank(0);
	int16_t var_13C = _0x6024B0C();
	int16_t var_128 = var_13C;
	_0x6024C3C(var_13C, 60, 266, sequenceEntry->_0x0);

	if (INPUTS[INPUT_SERVICE] & SERVICE_TEST) {
		const char **messageUseOnly;
		switch (CurrentRegionWarning) {
			case REGIONWARNING_JAPAN: // 0
				_0x6029814(0x7F00, 0x7F00, 0, 0xFF);

				for (uint32_t numWarningFrames = 0u; numWarningFrames < 500u; numWarningFrames++) {
					UpdateFrame();

					if ((((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) && numWarningFrames > 60u) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
						numWarningFrames = 600u;
					}

					ShowSystemTextCentered(30, "- Warning! -", 0u);
					int16_t y = 60;
					const char **messageLine = MessageJapanUseOnly;
					for (size_t lineNum = 0; lineNum < lengthof(MessageJapanUseOnly); lineNum++, y += 20, messageLine++) {
						ShowSystemText(55, y, *messageLine, false);
					}
				}
				break;

			case REGIONWARNING_SKIP: // 1
				if ((EEPROM[0] & REGION_SETTING) == REGION_JAPAN /* 0 */) {
					CurrentRegionWarning = REGIONWARNING_JAPAN;

					_0x6029814(0x7F00, 0x7F00, 0, 0xFF);

					for (uint32_t numWarningFrames = 0u; numWarningFrames < 500u; numWarningFrames++) {
						UpdateFrame();

						if ((((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) && numWarningFrames > 60u) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
							numWarningFrames = 600u;
						}

						ShowSystemTextCentered(30, "- Warning! -", 0u);
						int16_t y = 60;
						const char **messageLine = MessageJapanUseOnly;
						for (size_t lineNum = 0; lineNum < lengthof(MessageJapanUseOnly); lineNum++, y += 20, messageLine++) {
							ShowSystemText(55, y, *messageLine, false);
						}
					}
				}
				break;

			case REGIONWARNING_KOREA: // 2
				_0x6029814(0x7F00, 0x7F00, 0, 0xFF);

				for (uint32_t numWarningFrames = 0u; numWarningFrames < 500u; numWarningFrames++) {
					UpdateFrame();

					if ((((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) && numWarningFrames > 60u) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
						numWarningFrames = 600u;
					}

					ShowSystemTextCentered(30, "- Warning! -", 0u);
					int16_t y = 60;
					const char **messageLine = MessageKoreaUseOnly;
					for (size_t lineNum = 0; lineNum < lengthof(MessageKoreaUseOnly); lineNum++, y += 20, messageLine++) {
						ShowSystemText(55, y, *messageLine, false);
					}
				}
				break;

			case REGIONWARNING_REGIONSETTING: // 3
				_0x6029814(0x7F00, 0x7F00, 0, 0xFF);

				for (uint32_t numWarningFrames = 0u; numWarningFrames < 500u; numWarningFrames++) {
					UpdateFrame();

					if ((((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) && numWarningFrames > 60u) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
						numWarningFrames = 600u;
					}

					switch (EEPROM[0] & REGION_SETTING) {
						case REGION_USACANADA: // 1
							_0x606006C[var_13C]._0x2C &= 0x7FFF;

							int16_t y = 46;
							const char **messageLine = MessageUSACanadaUseOnly;
							for (size_t lineNum = 0; lineNum < lengthof(MessageUSACanadaUseOnly); lineNum++, y += 24, messageLine++) {
								_0x602B7D8(*messageLine, y, 40);
							}
							break;

						case REGION_KOREA: // 2
							_0x606006C[var_13C]._0x2C &= 0x7FFF;

							int16_t y = 46;
							const char **messageLine = MessageKoreaUseOnly;
							for (size_t lineNum = 0; lineNum < lengthof(MessageKoreaUseOnly); lineNum++, y += 24, messageLine++) {
								_0x602B7D8(*messageLine, y, 40);
							}
							break;

						case REGION_HONGKONG: // 4
							_0x606006C[var_13C]._0x2C &= 0x7FFF;

							int16_t y = 46;
							const char **messageLine = MessageHongKongUseOnly;
							for (size_t lineNum = 0; lineNum < lengthof(MessageHongKongUseOnly); lineNum++, y += 24, messageLine++) {
								_0x602B7D8(*messageLine, y, 40);
							}
							break;

						case REGION_TAIWAN: // 8
							_0x606006C[var_13C]._0x2C &= 0x7FFF;

							int16_t y = 46;
							const char **messageLine = MessageTaiwanUseOnly;
							for (size_t lineNum = 0; lineNum < lengthof(MessageTaiwanUseOnly); lineNum++, y += 24, messageLine++) {
								_0x602B7D8(*messageLine, y, 40);
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
		_0x606006C[var_13C]._0x2C |= 0x8000;

		InitScanlinesBank(0u);
		_0x6029814(0x0000, 0x0000, 0, 0xFF);
		SetPal(80u, 1u, _0x678D0);

		// TODO: Rename "frames" once it's understood what this is showing.
		for (uint32_t frames = 0u; frames < 500u; frames++) {
			UpdateFrame();

			ShowObject(_0xA7A0C, 104, 32, 80u, 125u);
			ShowObject(_0xA79F4, 0, 0, 80u, 125u);

			if ((ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)) || !(INPUTS[INPUT_SERVICE] & SERVICE_TEST)) {
				frames = 600u;
			}
		}
	}

	_0x6024B78(var_128);
	UpdateFrame();

	CurrentScreen = SCREEN_DEVELOPER;
	_0x6009410(); // TODO: Reimplement this function next. Rename this Loop().
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
	_0x2405FFF2 = 0x30u;
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

	SetSprite(0u, NULL);
	SpriteNames[0] = 0u;
	SpriteNames[1] = SPRITENAME_TERMINATE;
}

static void _0x6000AE0() {
	_0x607926C = (*BuildData.sequence)[7];
}

static void _0x6000AEC() {}
