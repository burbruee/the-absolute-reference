#include "Video/GameBg.h"
#include "Video/BgMap.h"
#include "Video/Video.h"
#include "Video/Pal.h"
#include "Game/Play/Player.h"
#include "Game/Screen.h"
#include "Main/MainLoop.h"
#include "Lib/Math.h"

GameBg CurrentGameBg;

static uint8_t DemoSection = 0u;

static const Color* const BgPalPtrs[] = {
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

static const BgMap* const BgMapPtrs[] = {
	(const BgMap*)&BgMapSection0,
	(const BgMap*)&BgMapSection1,
	(const BgMap*)&BgMapSection2,
	(const BgMap*)&BgMapSection3,
	(const BgMap*)&BgMapSection4,
	(const BgMap*)&BgMapSection5,
	(const BgMap*)&BgMapSection6,
	(const BgMap*)&BgMapSection7,
	(const BgMap*)&BgMapSection8,
	(const BgMap*)&BgMapSection9,
	(const BgMap*)&BgMapVersus
};


void UNK_60169DC() {
	CurrentGameBg.UNK_10 = 1;
	CurrentGameBg.index = 6;
	CurrentGameBg.UNK_13 = 1;
	for (size_t i = 0; i < 4; i++) {
		CurrentGameBg.UNK_16[i] = 0;
	}

	UNK_6024ED8();
	CurrentGameBg.UNK_1E = UNK_60257EE();
	UNK_6026FDC(CurrentGameBg.UNK_1E, 1);
}

void UNK_6016A30(uint8_t arg0) {
	for (size_t i = 0; i < lengthoffield(GameBg, UNK_16); i++) {
		CurrentGameBg.UNK_16[i] = 0;
	}

	if (arg0 != 0) {
		CurrentGameBg.UNK_13 = 2;
		return;
	}

	CurrentGameBg.UNK_13 = 2;
	CurrentGameBg.UNK_16[0] = 2;
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
		CurrentGameBg.index = CurrentGameBg.UNK_12;
	}

	SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.index]);
	CurrentGameBg.UNK_0[0].UNK_0 = BgMapPtrs[CurrentGameBg.index];
	CurrentGameBg.UNK_0[0].UNK_4 = 1;
	CurrentGameBg.UNK_0[1].UNK_0 = NULL;
	CurrentGameBg.UNK_0[1].UNK_4 = 1;
	SetBgDarkness(CurrentGameBg.UNK_1E, 0);
	UNK_60267E4(CurrentGameBg.UNK_1E);
}

void UNK_6016B40() {
	SetBgDarkness(CurrentGameBg.UNK_1E, CurrentGameBg.UNK_16[1]);
	if (CurrentGameBg.UNK_16[1] + 6 > 63) {
		CurrentGameBg.UNK_16[1] = 63;
		SetBgDarkness(CurrentGameBg.UNK_1E, 63);
		CurrentGameBg.UNK_13 = 3;
		CurrentGameBg.UNK_16[0] = 2;
		if (MainLoop == MAINLOOP_DEMO) {
			CurrentGameBg.index = DemoSection;
		}
		else {
			CurrentGameBg.index = CurrentGameBg.UNK_12;
		}
		SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.index]);
		CurrentGameBg.UNK_0[0].UNK_0 = BgMapPtrs[CurrentGameBg.index];
		CurrentGameBg.UNK_0[0].UNK_4 = 1;
		CurrentGameBg.UNK_0[1].UNK_0 = NULL;
		CurrentGameBg.UNK_0[1].UNK_4 = 1;
		UNK_60267E4(CurrentGameBg.UNK_1E);
	}
	else {
		CurrentGameBg.UNK_16[1] += 6;
	}
}

void UNK_6016BDE() {
	SetBgDarkness(CurrentGameBg.UNK_1E, CurrentGameBg.UNK_16[1]);
	if ((CurrentGameBg.UNK_16[1] -= 6) <= 0) {
		CurrentGameBg.UNK_16[1] = 0;
		SetBgDarkness(CurrentGameBg.UNK_1E, 0);
		CurrentGameBg.UNK_13 = 1;
	}

	if (--CurrentGameBg.UNK_16[0] == 0) {
		CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
		CurrentGameBg.UNK_16[0] = 2;
		int8_t var0 = CurrentGameBg.frame;
		int8_t var1 = var0 % 4;
		if (var1 < 0) {
			var1 += 3;
		}
		UNK_6025B9A(CurrentGameBg.UNK_1E, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * -VIDEO_WIDTH);
		UNK_60267E4(CurrentGameBg.UNK_1E);
	}
}

void UNK_6016C14() {
	if (--CurrentGameBg.UNK_16[0] == 0) {
		CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
		CurrentGameBg.UNK_16[0] = 2;
		int8_t var0 = CurrentGameBg.frame;
		int8_t var1 = var0 % 4;
		if (var0 < 0) {
			var0 += 3;
		}
		UNK_6025B9A(CurrentGameBg.UNK_1E, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * -VIDEO_WIDTH);
		UNK_60267E4(CurrentGameBg.UNK_1E);
	}
}

void UpdateGameBg() {
	if (!(CurrentGameBg.UNK_10 & 0x80u) && GetNextPauseMode() < PAUSEMODE_GAME) {
		if (CurrentGameBg.UNK_10 & 0x01u) {
			UNK_6016A30(0);
		}
		else if (!Demo && CurrentGameBg.UNK_13 == 1 && MainLoop != MAINLOOP_TEST && !(GameFlags & GAME_VERSUS)) {
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
					CurrentGameBg.UNK_12 = section;
					UNK_6016A30(1);
				}
				if (DemoSection < section) {
					DemoSection = section;
				}
			}
		}

		CurrentGameBg.UNK_10 &= ~0x07u;
		switch (CurrentGameBg.UNK_13) {
		case 2:
			SetBgDarkness(CurrentGameBg.UNK_1E, CurrentGameBg.UNK_16[1]);
			if (CurrentGameBg.UNK_16[1] + 6 > 63) {
				CurrentGameBg.UNK_16[1] = 63;
				SetBgDarkness(CurrentGameBg.UNK_1E, 63);
				CurrentGameBg.UNK_13 = 3u;
				CurrentGameBg.UNK_16[0] = 2;
				if (MainLoop == MAINLOOP_DEMO) {
					CurrentGameBg.index = DemoSection;
				}
				else {
					CurrentGameBg.index = CurrentGameBg.UNK_12;
				}
				SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.index]);
				CurrentGameBg.UNK_0[0].UNK_0 = BgMapPtrs[CurrentGameBg.index];
				CurrentGameBg.UNK_0[0].UNK_4 = 1;
				CurrentGameBg.UNK_0[1].UNK_0 = NULL;
				CurrentGameBg.UNK_0[1].UNK_4 = 1;
				UNK_60267E4(CurrentGameBg.UNK_1E);
			}
			else {
				CurrentGameBg.UNK_16[1] += 6;
			}
			break;

		case 3:
			SetBgDarkness(CurrentGameBg.UNK_1E, CurrentGameBg.UNK_16[1]);
			CurrentGameBg.UNK_16[1] -= 6;
			if (CurrentGameBg.UNK_16[1] <= 0) {
				CurrentGameBg.UNK_16[1] = 0;
				SetBgDarkness(CurrentGameBg.UNK_1E, 0);
				CurrentGameBg.UNK_13 = 1;
			}
		case 1:
			CurrentGameBg.UNK_16[0]--;
			if (CurrentGameBg.UNK_16[0] == 0) {
				CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
				CurrentGameBg.UNK_16[0] = 2;
				int8_t var0 = CurrentGameBg.frame;
				int8_t var1 = var0 % 4;
				if (var1 % 4 < 0) {
					var1 += 3;
				}
				UNK_6025B9A(CurrentGameBg.UNK_1E, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * VIDEO_WIDTH);
				UNK_60267E4(CurrentGameBg.UNK_1E);
			}
			break;

		default:
			break;
		}
	}
}
