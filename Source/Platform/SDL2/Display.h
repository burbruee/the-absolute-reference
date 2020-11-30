#pragma once

#include "SDL.h"

void OpenDisplay();
void CloseDisplay();

extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern SDL_Texture* FramebufferTexture;
