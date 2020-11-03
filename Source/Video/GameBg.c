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
	CurrentGameBg.bgMapIndex = 6;
	CurrentGameBg.UNK_13 = 1;
	for (size_t i = 0; i < 4; i++) {
		CurrentGameBg.darknesses[i] = 0;
	}

	UNK_6024ED8();
	CurrentGameBg.bgIndex = UNK_60257EE();
	UNK_6026FDC(CurrentGameBg.bgIndex, 1);
}

void UNK_6016A30(uint8_t arg0) {
	for (size_t i = 0; i < lengthoffield(GameBg, darknesses); i++) {
		CurrentGameBg.darknesses[i] = 0;
	}

	if (arg0 != 0) {
		CurrentGameBg.UNK_13 = 2;
		return;
	}

	CurrentGameBg.UNK_13 = 2;
	CurrentGameBg.darknesses[0] = 2;
	if (MainLoop == MAINLOOP_DEMO) {
		if (Screen == SCREEN_VERSUSDEMO) {
			CurrentGameBg.bgMapIndex = 10;
		}
		else if (Screen == SCREEN_TWINDEMO) {
			CurrentGameBg.bgMapIndex = Rand(9u);
		}
		else {
			CurrentGameBg.bgMapIndex = DemoSection;
		}
	}
	else {
		CurrentGameBg.bgMapIndex = CurrentGameBg.nextBgMapIndex;
	}

	SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.bgMapIndex]);
	CurrentGameBg.UNK_0[0].UNK_0 = BgMapPtrs[CurrentGameBg.bgMapIndex];
	CurrentGameBg.UNK_0[0].UNK_4 = 1;
	CurrentGameBg.UNK_0[1].UNK_0 = NULL;
	CurrentGameBg.UNK_0[1].UNK_4 = 1;
	SetBgDarkness(CurrentGameBg.bgIndex, 0);
	UNK_60267E4(CurrentGameBg.bgIndex);
}

void UNK_6016B40() {
	SetBgDarkness(CurrentGameBg.bgIndex, CurrentGameBg.darknesses[1]);
	if (CurrentGameBg.darknesses[1] + 6 > 63) {
		CurrentGameBg.darknesses[1] = 63;
		SetBgDarkness(CurrentGameBg.bgIndex, 63);
		CurrentGameBg.UNK_13 = 3;
		CurrentGameBg.darknesses[0] = 2;
		if (MainLoop == MAINLOOP_DEMO) {
			CurrentGameBg.bgMapIndex = DemoSection;
		}
		else {
			CurrentGameBg.bgMapIndex = CurrentGameBg.nextBgMapIndex;
		}
		SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.bgMapIndex]);
		CurrentGameBg.UNK_0[0].UNK_0 = BgMapPtrs[CurrentGameBg.bgMapIndex];
		CurrentGameBg.UNK_0[0].UNK_4 = 1;
		CurrentGameBg.UNK_0[1].UNK_0 = NULL;
		CurrentGameBg.UNK_0[1].UNK_4 = 1;
		UNK_60267E4(CurrentGameBg.bgIndex);
	}
	else {
		CurrentGameBg.darknesses[1] += 6;
	}
}

void UNK_6016BDE() {
	SetBgDarkness(CurrentGameBg.bgIndex, CurrentGameBg.darknesses[1]);
	if ((CurrentGameBg.darknesses[1] -= 6) <= 0) {
		CurrentGameBg.darknesses[1] = 0;
		SetBgDarkness(CurrentGameBg.bgIndex, 0);
		CurrentGameBg.UNK_13 = 1;
	}

	if (--CurrentGameBg.darknesses[0] == 0) {
		CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
		CurrentGameBg.darknesses[0] = 2;
		int8_t var0 = CurrentGameBg.frame;
		int8_t var1 = var0 % 4;
		if (var1 < 0) {
			var1 += 3;
		}
		UNK_6025B9A(CurrentGameBg.bgIndex, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * -VIDEO_WIDTH);
		UNK_60267E4(CurrentGameBg.bgIndex);
	}
}

void UNK_6016C14() {
	if (--CurrentGameBg.darknesses[0] == 0) {
		CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
		CurrentGameBg.darknesses[0] = 2;
		int8_t var0 = CurrentGameBg.frame;
		int8_t var1 = var0 % 4;
		if (var0 < 0) {
			var0 += 3;
		}
		UNK_6025B9A(CurrentGameBg.bgIndex, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * -VIDEO_WIDTH);
		UNK_60267E4(CurrentGameBg.bgIndex);
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
				if (CurrentGameBg.bgMapIndex != section) {
					CurrentGameBg.nextBgMapIndex = section;
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
			SetBgDarkness(CurrentGameBg.bgIndex, CurrentGameBg.darknesses[1]);
			if (CurrentGameBg.darknesses[1] + 6 > 63) {
				CurrentGameBg.darknesses[1] = 63;
				SetBgDarkness(CurrentGameBg.bgIndex, 63);
				CurrentGameBg.UNK_13 = 3u;
				CurrentGameBg.darknesses[0] = 2;
				if (MainLoop == MAINLOOP_DEMO) {
					CurrentGameBg.bgMapIndex = DemoSection;
				}
				else {
					CurrentGameBg.bgMapIndex = CurrentGameBg.nextBgMapIndex;
				}
				SetPal(160u, NUMPALCOLORS_8BPP / NUMPALCOLORS_4BPP, BgPalPtrs[CurrentGameBg.bgMapIndex]);
				CurrentGameBg.UNK_0[0].UNK_0 = BgMapPtrs[CurrentGameBg.bgMapIndex];
				CurrentGameBg.UNK_0[0].UNK_4 = 1;
				CurrentGameBg.UNK_0[1].UNK_0 = NULL;
				CurrentGameBg.UNK_0[1].UNK_4 = 1;
				UNK_60267E4(CurrentGameBg.bgIndex);
			}
			else {
				CurrentGameBg.darknesses[1] += 6;
			}
			break;

		case 3:
			SetBgDarkness(CurrentGameBg.bgIndex, CurrentGameBg.darknesses[1]);
			CurrentGameBg.darknesses[1] -= 6;
			if (CurrentGameBg.darknesses[1] <= 0) {
				CurrentGameBg.darknesses[1] = 0;
				SetBgDarkness(CurrentGameBg.bgIndex, 0);
				CurrentGameBg.UNK_13 = 1;
			}
		case 1:
			CurrentGameBg.darknesses[0]--;
			if (CurrentGameBg.darknesses[0] == 0) {
				CurrentGameBg.frame = (CurrentGameBg.frame + 1) % 32;
				CurrentGameBg.darknesses[0] = 2;
				int8_t var0 = CurrentGameBg.frame;
				int8_t var1 = var0 % 4;
				if (var1 % 4 < 0) {
					var1 += 3;
				}
				UNK_6025B9A(CurrentGameBg.bgIndex, &CurrentGameBg, (var0 / 4) * -VIDEO_HEIGHT, var1 * -VIDEO_WIDTH);
				UNK_60267E4(CurrentGameBg.bgIndex);
			}
			break;

		default:
			break;
		}
	}
}
