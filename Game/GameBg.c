#include "GameBg.h"
#include "BgMap.h"
#include "Video.h"
#include "Pal.h"
#include "MainLoop.h"
#include "Math.h"
#include "Player.h"
#include "Screen.h"

GameBg CurrentGameBg;

static uint8_t DemoSection = 0u;

static ROMDATA Color* const BgPalPtrs[] = {
	PAL_BGMAPSECTION0,
	PAL_BGMAPSECTION1,
	PAL_BGMAPSECTION2,
	PAL_BGMAPSECTION3,
	PAL_BGMAPSECTION4,
	PAL_BGMAPSECTION5,
	PAL_BGMAPSECTION6,
	PAL_BGMAPSECTION7,
	PAL_BGMAPSECTION8,
	PAL_BGMAPSECTION9,
	PAL_BGMAPVERSUS
};

static ROMDATA BgMap* const BgMapPtrs[] = {
	(ROMDATA BgMap*)&BgMapSection0,
	(ROMDATA BgMap*)&BgMapSection1,
	(ROMDATA BgMap*)&BgMapSection2,
	(ROMDATA BgMap*)&BgMapSection3,
	(ROMDATA BgMap*)&BgMapSection4,
	(ROMDATA BgMap*)&BgMapSection5,
	(ROMDATA BgMap*)&BgMapSection6,
	(ROMDATA BgMap*)&BgMapSection7,
	(ROMDATA BgMap*)&BgMapSection8,
	(ROMDATA BgMap*)&BgMapSection9,
	(ROMDATA BgMap*)&BgMapVersus
};


void _0x60169DC() {
	CurrentGameBg._0x10 = 1;
	CurrentGameBg.index = 6;
	CurrentGameBg._0x13 = 1;
	for (size_t i = 0; i < 4; i++) {
		CurrentGameBg._0x16[i] = 0;
	}

	_0x6024ED8();
	CurrentGameBg._0x1E = _0x60257EE();
	_0x6026FDC(CurrentGameBg._0x1E, 1);
}

void _0x6016A30(uint8_t arg0) {
	for (size_t i = 0; i < 4; i++) {
		CurrentGameBg._0x16[i] = 0;
	}

	if (arg0 != 0) {
		CurrentGameBg._0x13 = 2;
		return;
	}

	CurrentGameBg._0x13 = 2;
	CurrentGameBg._0x16[0] = 2;
	if (MainLoop == MAINLOOP_DEMO) {
		if (Screen == SCREEN_VERSUSDEMO) {
			CurrentGameBg.index = 10;
		}
		else if (Screen == SCREEN_TWINDEMO) {
			CurrentGameBg.index = Rand(9u);
		}
		else {
			CurrentGameBg.index = DemoSection;
		}
	}
	else {
		CurrentGameBg.index = CurrentGameBg._0x12;
	}

	SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.index]);
	CurrentGameBg._0x0 = BgMapPtrs[CurrentGameBg.index];
	CurrentGameBg._0x4 = 1;
	CurrentGameBg._0x8 = NULL;
	CurrentGameBg._0xC = 1;
	SetBgDarkness(CurrentGameBg._0x1E, 0);
	_0x60267E4(CurrentGameBg._0x1E);
}

void _0x6016B40() {
	SetBgDarkness(CurrentGameBg._0x1E, CurrentGameBg._0x16[1]);
	if (CurrentGameBg._0x16[1] + 6 > 63) {
		CurrentGameBg._0x16[1] = 63;
		SetBgDarkness(CurrentGameBg._0x1E, 63);
		CurrentGameBg._0x13 = 3;
		CurrentGameBg._0x16[0] = 2;
		if (MainLoop == MAINLOOP_DEMO) {
			CurrentGameBg.index = DemoSection;
		}
		else {
			CurrentGameBg.index = CurrentGameBg._0x12;
		}
		SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.index]);
		CurrentGameBg._0x0 = BgMapPtrs[CurrentGameBg.index];
		CurrentGameBg._0x4 = 1;
		CurrentGameBg._0x8 = NULL;
		CurrentGameBg._0xC = 1;
		_0x60267E4(CurrentGameBg._0x1E);
	}
	else {
		CurrentGameBg._0x16[1] += 6;
	}
}

void _0x6016BDE() {
	SetBgDarkness(CurrentGameBg._0x1E, CurrentGameBg._0x16[1]);
	if ((CurrentGameBg._0x16[1] -= 6) <= 0) {
		CurrentGameBg._0x16[1] = 0;
		SetBgDarkness(CurrentGameBg._0x1E, 0);
		CurrentGameBg._0x13 = 1;
	}

	if (--CurrentGameBg._0x16[0] == 0) {
		CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
		CurrentGameBg._0x16[0] = 2;
		int8_t var0 = CurrentGameBg.frame;
		int8_t var1 = var0 % 4;
		if (var1 < 0) {
			var1 += 3;
		}
		_0x6025B9A(CurrentGameBg._0x1E, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * -VIDEO_WIDTH);
		_0x60267E4(CurrentGameBg._0x1E);
	}
}

void _0x6016C14() {
	if (--CurrentGameBg._0x16[0] == 0) {
		CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
		CurrentGameBg._0x16[0] = 2;
		int8_t var0 = CurrentGameBg.frame;
		int8_t var1 = var0 % 4;
		if (var0 < 0) {
			var0 += 3;
		}
		_0x6025B9A(CurrentGameBg._0x1E, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * -VIDEO_WIDTH);
		_0x60267E4(CurrentGameBg._0x1E);
	}
}

void UpdateGameBg() {
	if (!(CurrentGameBg._0x10 & 0x80u) && GetNextPauseMode() < PAUSEMODE_GAME) {
		if (CurrentGameBg._0x10 & 0x01u) {
			_0x6016A30(0);
		}
		else if (!Demo && CurrentGameBg._0x13 == 1 && MainLoop != MAINLOOP_TEST && !(GameFlags & GAME_VERSUS)) {
			int16_t section1p = -1;
			int16_t section2p = -1;
			if (
				(Players[PLAYER1].nowFlags & NOW_STARTED) &&
				Players[PLAYER1].play.state != PLAYSTATE_GAMEOVER) {
				section1p = Players[PLAYER1].section;
				if ((Players[PLAYER1].modeFlags & MODE_TADEATH) && !(Players[PLAYER1].nowFlags & NOW_SELECTING) && section1p > 9) {
					section1p = 9;
				}
			}
			if (
				(Players[PLAYER2].nowFlags & NOW_STARTED) &&
				Players[PLAYER2].play.state != PLAYSTATE_GAMEOVER) {
				section2p = Players[PLAYER2].section;
				if ((Players[PLAYER2].modeFlags & MODE_TADEATH) && !(Players[PLAYER2].nowFlags & NOW_SELECTING) && section2p > 9) {
					section2p = 9;
				}
			}
			if (section1p >= 0 || section2p >= 0) {
				int16_t section = section1p;
				if (section2p > section1p) {
					section = section2p;
				}
				if (CurrentGameBg.index != section) {
					CurrentGameBg._0x12 = section;
					_0x6016A30(1);
				}
				if (DemoSection < section) {
					DemoSection = section;
				}
			}
		}

		CurrentGameBg._0x10 &= ~0x07u;
		switch (CurrentGameBg._0x13) {
		case 2:
			SetBgDarkness(CurrentGameBg._0x1E, CurrentGameBg._0x16[1]);
			if (CurrentGameBg._0x16[1] + 6 > 63) {
				CurrentGameBg._0x16[1] = 63;
				SetBgDarkness(CurrentGameBg._0x1E, 63);
				CurrentGameBg._0x13 = 3u;
				CurrentGameBg._0x16[0] = 2;
				if (MainLoop == MAINLOOP_DEMO) {
					CurrentGameBg.index = DemoSection;
				}
				else {
					CurrentGameBg.index = CurrentGameBg._0x12;
				}
				SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.index]);
				CurrentGameBg._0x0 = BgMapPtrs[CurrentGameBg.index];
				CurrentGameBg._0x4 = 1;
				CurrentGameBg._0x8 = NULL;
				CurrentGameBg._0xC = 1;
				_0x60267E4(CurrentGameBg._0x1E);
			}
			else {
				CurrentGameBg._0x16[1] += 6;
			}
			break;

		case 3:
			SetBgDarkness(CurrentGameBg._0x1E, CurrentGameBg._0x16[1]);
			CurrentGameBg._0x16[1] -= 6;
			if (CurrentGameBg._0x16[1] <= 0) {
				CurrentGameBg._0x16[1] = 0;
				SetBgDarkness(CurrentGameBg._0x1E, 0);
				CurrentGameBg._0x13 = 1;
			}
		case 1:
			CurrentGameBg._0x16[0]--;
			if (CurrentGameBg._0x16[0] == 0) {
				CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
				CurrentGameBg._0x16[0] = 2;
				int8_t var0 = CurrentGameBg.frame;
				int8_t var1 = var0 % 4;
				if (var1 % 4 < 0) {
					var1 += 3;
				}
				_0x6025B9A(CurrentGameBg._0x1E, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * VIDEO_WIDTH);
				_0x60267E4(CurrentGameBg._0x1E);
			}
			break;

		default:
			break;
		}
	}
}
