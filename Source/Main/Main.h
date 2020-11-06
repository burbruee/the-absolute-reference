#pragma once

#include "Main/MainLoop.h"
#include "PlatformTypes.h"
#include "Video/VideoDefs.h"

// Must be initialized from the program ROM. This palette isn't part of
// Pal.data.
// TODO: Rename this something more specific.
extern ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
