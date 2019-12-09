#pragma once

#include <stdint.h>

typedef enum ScreenState {
	SCREEN_MEMCHECK,
	SCREEN_COPYRIGHT,
	SCREEN_TITLE,
	SCREEN_DEVELOPER,
	SCREEN_TWINDEMO,
	SCREEN_VERSUSDEMO,
	SCREEN_DOUBLESDEMO,
	SCREEN_NORMALRANKING,
	SCREEN_MASTERRANKING,
	SCREEN_MASTERSECTIONTIMERANKING,
	SCREEN_DOUBLESRANKING,
	SCREEN_TESTMODE,
	SCREEN_VERSIONTITLE,
	SCREEN_GAME
} ScreenState;
extern ScreenState CurrentScreen;

// TODO: Move these?
int16_t _0x6008E38(int16_t level);
int16_t _0x6008EC8(int16_t level);
int16_t _0x6008EEC(int16_t level);

typedef enum MainLoopState {
	MAINLOOP_ATTRACT,
	MAINLOOP_GAME,
	MAINLOOP_TEST
} MainLoopState;
extern MainLoopState CurrentMainLoopState;
void RunMainLoop();
// TODO
uint8_t NumVersusRoundsSetting();