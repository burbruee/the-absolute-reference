#include "Main/MainLoop.h"
#include "Game/Screen.h"
#include "PlatformUpdate.h"
#include "PlatformTypes.h"

#include "Video/SpriteInit.h"
MAIN_RETURNTYPE main() {
#if 0
	AddSprite(ADDSPRITE_1, NULL);
#else
	PLATFORM_INIT();

	Screen = SCREEN_COPYRIGHT;
	RunMainLoop();

	PLATFORM_QUIT();
#endif
}
