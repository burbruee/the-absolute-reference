#include "Main/TestLoop.h"
#include "Main/Frame.h"
#include "Game/Screen.h"
#include "HwSound.h"
#include <stdlib.h>

MainLoopState RunTestLoop() {
	InitSound();

	// This keeps execution in this function while the test input is held down.
	while (UpdateFrame());

	// For now, just restart back at the copyright screen, until test mode is implemented.
	Screen = SCREEN_COPYRIGHT;
	return MAINLOOP_ATTRACT;
}
