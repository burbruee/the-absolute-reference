#include "Main/MainLoop.h"
#include "Game/Screen.h"
#include "PlatformUpdate.h"
#include "PlatformTypes.h"

MAIN_DECLARATION {
	PLATFORM_INIT();

	Screen = SCREEN_COPYRIGHT;
	RunMainLoop();

	PLATFORM_QUIT();
}
