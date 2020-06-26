#pragma once

#include "Loop.h"
#include "Screen.h"
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
	NUMDEMOSCREENS = DEMOSCREEN_DOUBLES3
} DemoScreenState;

DemoScreenState DemoScreen;

bool _0x06014234();

void InitDemoLoop();
MainLoopState RunDemoLoop();

ScreenState StartRankingScreen();
ScreenState StartCopyrightScreen();
ScreenState StartTitleScreen();
ScreenState StartDeveloperScreen();
ScreenState StartVersionTitleScreen();
ScreenState StartDemoScreen();
