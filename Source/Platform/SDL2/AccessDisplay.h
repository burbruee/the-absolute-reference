#pragma once

#include "SDL.h"
#include <stdbool.h>

bool OpenDisplay();
void CloseDisplay();

extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern SDL_Texture* FramebufferTexture;
