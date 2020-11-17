#pragma once
#include "Main/MainLoop.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

bool PlatformInit();
#define PLATFORM_INIT() do { if (!PlatformInit()) { return EXIT_FAILURE; } } while (false)

void PlatformUpdateInputs();
#define PLATFORM_UPDATEINPUTS() PlatformUpdateInputs()

void PlatformFrame();
#define PLATFORM_FRAME() PlatformFrame()

void PlatformFinishUpdate();
#define PLATFORM_FINISHUPDATE() PlatformFinishUpdate()

#define PLATFORM_QUIT() return EXIT_SUCCESS
