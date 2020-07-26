#pragma once
#include "Game/Loop.h"
#include <stdlib.h>
#include <stdint.h>

uint32_t PlatformInit();
#define PLATFORM_INIT() do { if (!PlatformInit()) { return EXIT_FAILURE; } } while (false)

void PlatformFrame();
#define PLATFORM_FRAME() PlatformFrame()

int PlatformQuit();
#define PLATFORM_QUIT() return PlatformQuit()
