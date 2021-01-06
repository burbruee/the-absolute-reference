#pragma once

#include "Input/InputTypes.h"
#include "SDL.h"
#include <stdbool.h>

bool OpenConfig();
void SaveConfig();
void CloseConfig();

extern SDL_Keycode InputConfigKeyboard[NUMINPUTS][8];

extern SDL_Joystick* Joysticks[NUMPLAYERS];
extern int InputConfigJoystickButtons[NUMPLAYERS][NUMINPUTS][8];
extern int InputConfigJoystickAxes[NUMPLAYERS][NUMINPUTS][8][2];
extern int InputConfigJoystickHats[NUMPLAYERS][NUMINPUTS][8][2];

typedef enum DisplayMode {
	DISPLAY_WINDOW,
	DISPLAY_FULLSCREENEXCLUSIVE,
	DISPLAY_FULLSCREENDESKTOP
} DisplayMode;
extern DisplayMode DisplayModeSetting;

extern int DisplayDimensions[2];

extern int Vsync;

extern int VsyncUpdateRate;
