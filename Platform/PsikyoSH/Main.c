#include "Game/Player.h"
#include "Game/Text.h"
#include "Game/Debug.h"
#include "Game/BuildData.h"
#include "Game/RegionWarning.h"
#include "Game/Fixed.h"
#include "Game/Setting.h"
#include "Game/Sound.h"
#include "Game/HwData.h"
#include "Platform/Util/Macros.h"
#include <stdint.h>
#include <stdbool.h>

RegionWarning CurrentRegionWarning;

// TODO: Lots of undefined things here.
void main() {
	bool canExitMemCheck = false;
	SequenceEntry *sequenceEntry = (*BuildData.sequence)[7];

	// System init.
	_0x602ACB0();
	NumScreenFrames = 0u;
	InitSound();
	NumVideoOps = 0u;
	PlaySoundEffect(SOUNDEFFECT_READY);
	_0x6023790();
	InitCredits();
	InitVideo();
	_0x6024ED8();
	_0x60169DC();
	InitEntities();
	InitPalCycles();
	_0x602419C();
	_0x6000AE0();
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
		_0x6012BF8();
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

					ShowCenteredSystemText(30, "- Warning! -", 0u);
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

						ShowCenteredSystemText(30, "- Warning! -", 0u);
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

					ShowCenteredSystemText(30, "- Warning! -", 0u);
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

