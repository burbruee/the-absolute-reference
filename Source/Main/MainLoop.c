#include "Main/MainLoop.h"
#include "Main/AttractLoop.h"
#include "Main/GameLoop.h"
#include "Main/TestLoop.h"
#include "Main/Frame.h"
#include "Game/Save.h"
#include "Lib/Math.h"

StartPlayerFlag StartPlayers = STARTPLAYER_NONE;
MainLoopState MainLoop = MAINLOOP_ATTRACT;

void RunMainLoop() {
	while (true) {
		UpdateFrame();

		InitSeed += Rand(1192u) + 1u;

		switch (MainLoop) {
			case MAINLOOP_ATTRACT:
				MainLoop = RunAttractLoop();
				continue;

			case MAINLOOP_GAME:
				MainLoop = RunGameLoop();
				continue;

			case MAINLOOP_TEST:
				MainLoop = RunTestLoop();
				break;

			default:
				break;
		}

		MainLoop = MAINLOOP_ATTRACT;
	}
}
