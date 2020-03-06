#pragma once

#include "HwInput.h"
#include "Pause.h"
#include <stdbool.h>

// Read this to check the current pause mode, not NextPauseMode.
// Only frame handling functions should write this, once, before updating a
// frame.
extern PauseMode CurrentPauseMode;

extern bool NumScreenFramesOdd;

extern uint32_t NumScreenFrames;

// Platforms have to update RandScale by adding a value proportional to the
// amount of time remaining until the next vblank after the code has finished
// processing a frame. A number of CPU cycles would suffice. A time value would
// also work, such as nanoseconds until the next frame after finishing
// processing, but don't use the number of remaining milliseconds; the low
// precision would lead to RandScale not changing by a very random amount each
// frame.
//
// The PsikyoSH version just sits in a busy loop, incrementing each iteration,
// until the next frame starts.
extern uint32_t RandScale;

extern bool TestModeDisabled;

extern uint16_t _0x6060024;
extern uint32_t _0x6060028;
extern uint32_t NumVblanks;
extern bool VBlankFinished;

// TODO: Could be HBlank. Is an interrupt handler. Have to define it for each
// platform.
void _0x602AD0E();

// Must be implemented by frontends.
//
// See the PsikyoSH frontend's VBlank for the specific things the function must
// do.
//
// Not called by the game code. Platforms must either set it up as a vertical
// blank interrupt handler, assume about 60Hz refresh rate and call it before
// drawing every frame, or set up a threaded implementation for the game code
// updating at FRAMETIME_ORIGINAL or FRAMETIME_INTENDED in
// Platform/Util/Video.h separate from the render thread.
void VBlank();

// TODO: Do all these functions, when returning true, indicate execution should
// transition to the test mode screen?
bool UpdateFrame();
bool _0x602AECA();
bool UpdateGameFrame();
bool UpdateAttractFrame(ButtonInput* buttonsDown1p, ButtonInput* buttonsDown2p);
