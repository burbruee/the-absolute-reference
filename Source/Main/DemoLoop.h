#pragma once

#include "Main/MainLoop.h"
#include "Game/Screen.h"
#include "Video/Video.h"
#include "PlatformTypes.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum DemoScreenState {
	DEMOSCREEN_START,
	DEMOSCREEN_TWIN0,
	DEMOSCREEN_VERSUS1,
	DEMOSCREEN_DOUBLES1,
	DEMOSCREEN_TWIN1,
	DEMOSCREEN_VERSUS2,
	DEMOSCREEN_DOUBLES2,
	DEMOSCREEN_DOUBLES3,
	NUMDEMOSCREENS
} DemoScreenState;

extern ROMDATA Color UNK_6033790[NUMPALCOLORS_4BPP];

extern DemoScreenState DemoScreen;

bool NextScreenVersionTitle();

MainLoopState RunDemoLoop();
