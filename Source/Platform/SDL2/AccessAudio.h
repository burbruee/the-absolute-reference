#pragma once

#include "Sound/SoundDefs.h"
#include "SDL.h"
#include "Lib/Fixed.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

extern SDL_AudioDeviceID AudioDevice;
extern SDL_mutex* AudioMutex;

bool OpenAudio();
void CloseAudio();
