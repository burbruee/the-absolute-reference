#pragma once

#include "SDL_stdinc.h"

extern Uint64 PlatformCurrentTime;
extern Uint64 PlatformTimeAccumulator;

void PlatformUpdateInputs();
#define PLATFORM_UPDATEINPUTS() PlatformUpdateInputs()

void PlatformFrame();
#define PLATFORM_FRAME() PlatformFrame()

void PlatformFinishUpdate();
#define PLATFORM_FINISHUPDATE() PlatformFinishUpdate()
