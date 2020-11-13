#pragma once
#include "PlatformTypes.h"

// Fully self-hosted, platform-agnostic, only-standard-C renderer. Directly
// renders at the original resolution into the framebuffer. Of course it's
// slower than a renderer implemented to take advantage of some
// hardware-accelerated API, but this can quickly be used on new platforms, and
// if fast enough, it can be just what's used with those ports.
void Render(Color* const framebuffer, const uint8_t* const tileData);
