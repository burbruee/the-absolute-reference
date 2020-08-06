#pragma once

#include "Input/InputTypes.h"
#include <stdint.h>

// TODO: Move these?
int16_t _0x6008E38(int16_t level);
int16_t _0x6008EC8(int16_t level);
int16_t _0x6008EEC(int16_t level);

typedef enum StartPlayerNum {
	STARTPLAYER_NONE = 0,
	STARTPLAYER_1P = 1 << PLAYER1,
	STARTPLAYER_2P = 1 << PLAYER2
} StartPlayerNum;
extern uint16_t GameStartPlayer;

typedef enum MainLoopState {
	MAINLOOP_DEMO,
	MAINLOOP_GAME,
	MAINLOOP_TEST
} MainLoopState;
extern MainLoopState MainLoop;
void RunMainLoop();
// TODO