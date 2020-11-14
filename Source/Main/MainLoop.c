#include "Main/MainLoop.h"
#include "Main/DemoLoop.h"
#include "Main/GameLoop.h"
#include "Main/TestLoop.h"
#include "Main/Frame.h"
#include "Game/Save.h"
#include "Lib/Math.h"

StartPlayerFlag StartPlayers = STARTPLAYER_NONE;
MainLoopState MainLoop = MAINLOOP_DEMO;

void RunMainLoop() {
	// TODO: Change "while (true)" here to something like "while (!QUIT)", so
	// ports to OS's can exit the game. Usage of QUIT will have to be put
	// throughout the code, so a quit event can cause the program to quit. For
	// platforms that shouldn't have a quit condition, just define QUIT to
	// "false".
	while (true) {
		UpdateFrame();

		InitSeed += Rand(1192u) + 1u;

		switch (MainLoop) {
			case MAINLOOP_DEMO:
				MainLoop = RunDemoLoop();
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

		MainLoop = MAINLOOP_DEMO;
	}
}
