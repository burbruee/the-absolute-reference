#include "Main/TestLoop.h"
#include <stdlib.h>

MainLoopState RunTestLoop() {
	// TODO: As a temporary way to test and have the game actually close, this exit call is placed here, causing the game to close if test mode is entered.
	exit(EXIT_SUCCESS);
	return MAINLOOP_DEMO; // Temporary solution to keep the game running, until test mode is implemented.
}
