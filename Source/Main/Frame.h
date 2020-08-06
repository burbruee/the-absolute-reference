#pragma once

#include "Input/InputTypes.h"
#include "Video/Video.h"
#include <stdbool.h>

// Read this to check the current pause mode, not NextPauseMode. Only
// frame-update functions should write to this, once, before updating a frame.
extern PauseMode CurrentPauseMode;

// True if ScreenTime is odd.
extern bool ScreenTimeOdd;

// Number of frames in the current screen. Reset when the screen transitions to
// another screen.
extern uint32_t ScreenTime;

// Total number of frames since startup.
extern uint32_t Uptime;

// Total number of frames spent in the demo; it's really just the time not
// spent in-game, but is copied into Save->demoWaitTime.
extern uint32_t DemoWaitTime;

extern uint16_t _0x606564C;

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
extern bool VblankFinished;

void _0x602ACB0();
void _0x602ACE0(uint16_t);

// TODO: Could be Hblank. Is an interrupt handler. Have to define it for each
// platform.
void _0x602AD0E();

// Must be implemented by frontends.
//
// See the PsikyoSH frontend's Vblank for the specific things the function must
// do.
//
// Not called by the game code. Platforms must either set it up as a vertical
// blank interrupt handler, assume about 60Hz refresh rate and call it before
// drawing every frame, or set up a threaded implementation for the game code
// updating at FRAMETIME_ORIGINAL or FRAMETIME_INTENDED in
// Platform/Util/Video.h separate from the render thread.
void Vblank();

// When any of these frame-update functions return true, the program must be
// transitioned to the test mode screen.
bool UpdateFrame();
bool _0x602AECA();
bool UpdateGame();
bool UpdateDemo(ButtonInput* buttonsDown1p, ButtonInput* buttonsDown2p);
