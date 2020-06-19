#pragma once

#include <stdint.h>

// TODO: Move these?
int16_t _0x6008E38(int16_t level);
int16_t _0x6008EC8(int16_t level);
int16_t _0x6008EEC(int16_t level);

typedef enum MainLoopState {
	MAINLOOP_DEMO,
	MAINLOOP_GAME,
	MAINLOOP_TEST
} MainLoopState;
extern MainLoopState MainLoop;
void RunMainLoop();
// TODO
uint8_t NumVersusRoundsSetting();