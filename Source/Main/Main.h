#pragma once

#include "PlatformTypes.h"
#include "Video/VideoDefs.h"

extern MainLoopState MainLoop;

// Must be initialized from the program ROM. This palette isn't part of
// Pal.data.
// TODO: Rename this something more specific.
extern ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
