#include "GameBg.h"
#include "Video.h"
#include "Loop.h"
#include "Player.h"

GameBg CurrentGameBg;

void _0x60169DC() {
	//TODO
}

void _0x6016A30(uint8_t arg0) {
	//TODO
}

void _0x6016B40() {
	//TODO
}

void _0x6016BDE() {
	//TODO
}

void _0x6016C14() {
	//TODO
}

void UpdateGameBg() {
	if (!(CurrentGameBg._0x10 & 0x80u) && GetNextPauseMode() < PAUSEMODE_GAME) {
		if (CurrentGameBg._0x10 % 2) {
			_0x6016A30(0);
		}
		else if (!Attract && CurrentGameBg._0x13 == 1 && CurrentMainLoopState != MAINLOOP_TEST && !(GameFlags & GAME_VERSUS)) {
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
				if (_0x6079540 < section) {
					_0x6079540 = section;
				}
			}
		}

		CurrentGameBg._0x10 &= 0xF8u;
		switch (CurrentGameBg._0x13) {
		case 2:
			SetBgDarkness(CurrentGameBg._0x1E, CurrentGameBg._0x16[1]);
			if (CurrentGameBg._0x16[1] + 6 > 63) {
				CurrentGameBg._0x16[1] = 63;
				SetBgDarkness(CurrentGameBg._0x1E, 63);
				CurrentGameBg._0x13 = 3u;
				CurrentGameBg._0x16[0] = 2;
				if (CurrentMainLoopState == MAINLOOP_ATTRACT) {
					CurrentGameBg.index = _0x6079540;
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
