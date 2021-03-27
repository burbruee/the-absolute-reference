#pragma once

#include "Input/InputTypes.h"
#include "SDL.h"
#include <stdbool.h>

bool OpenConfig();
void SaveConfig();
void CloseConfig();

typedef struct KeySetting {
	SDL_Keymod mod;
	SDL_Keycode code;
} KeySetting;

extern KeySetting InputConfigKeyboard[NUMINPUTS][8];

extern KeySetting InputApplicationKeyboardQuit;

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

extern bool AudioMuted;

typedef enum MixingSetting {
	MIXING_MONO,
	MIXING_STEREO
} MixingSetting;
extern MixingSetting AudioMixing;

typedef enum SpeakersSetting {
	SPEAKERS_LEFT  = 1 << 0,
	SPEAKERS_RIGHT = 1 << 1,
	SPEAKERS_BOTH  = SPEAKERS_LEFT | SPEAKERS_RIGHT
} SpeakersSetting;
extern SpeakersSetting AudioSpeakers;
