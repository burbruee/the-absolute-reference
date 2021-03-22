#pragma once

#include "SDL_stdinc.h"

extern Uint64 PlatformCurrentTime;
extern Uint64 PlatformTimeAccumulator;

void PlatformUpdateInputs();
#define PLATFORM_UPDATEINPUTS() PlatformUpdateInputs()

void PlatformFrame();
#define PLATFORM_FRAME() PlatformFrame()

#define FRAME_DURATION (1.0 / (57272700.0 / 8.0 / 443.0 / 262.0))

void PlatformFinishUpdate();
#define PLATFORM_FINISHUPDATE() PlatformFinishUpdate()
