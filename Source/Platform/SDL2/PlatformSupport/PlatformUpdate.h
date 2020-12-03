#pragma once
#include "Main/MainLoop.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

bool PlatformInit(const int argc, const char* const* const argv);
#define PLATFORM_INIT() \
do { \
	if (!PlatformInit(argc, argv)) { \
		return EXIT_FAILURE; \
	} \
	else { \
		printf("Starting game.\n\n"); \
	} \
} while (false)

void PlatformUpdateInputs();
#define PLATFORM_UPDATEINPUTS() PlatformUpdateInputs()

void PlatformFrame();
#define PLATFORM_FRAME() PlatformFrame()

void PlatformFinishUpdate();
#define PLATFORM_FINISHUPDATE() PlatformFinishUpdate()

#define PLATFORM_QUIT() return EXIT_SUCCESS
