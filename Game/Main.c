#include "Screen.h"
#include "Loop.h"
#include "PlatformUpdate.h"
#include "PlatformTypes.h"

MAIN_RETURNTYPE main() {
	PLATFORM_INIT();

	Screen = SCREEN_COPYRIGHT;
	RunMainLoop();

	PLATFORM_QUIT();
}
