#include "Platform/SDL2/Display.h"
#include "Platform/Util/Config.h"
#include "Platform/Util/Data.h"
#include "Video/Video.h"

SDL_Window* Window;
SDL_Renderer* Renderer;
SDL_Texture* FramebufferTexture;

void OpenDisplay() {
	Uint32 windowFlags = SDL_WINDOW_SHOWN;
	switch (DisplayModeSetting) {
	default:
	case DISPLAY_WINDOW:
		windowFlags |= SDL_WINDOW_RESIZABLE;
		break;

	case DISPLAY_FULLSCREENEXCLUSIVE:
		windowFlags |= SDL_WINDOW_FULLSCREEN;
		break;

	case DISPLAY_FULLSCREENDESKTOP:
		windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		break;
	}
	if (!(Window = SDL_CreateWindow("The Absolute Reference", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DisplayDimensions[0], DisplayDimensions[1], windowFlags))) {
		exit(EXIT_FAILURE);
	}
	if (!(Renderer = SDL_CreateRenderer(Window, 0, SDL_RENDERER_ACCELERATED | (Vsync ? SDL_RENDERER_PRESENTVSYNC : 0u)))) {
		exit(EXIT_FAILURE);
	}
	SDL_RenderSetLogicalSize(Renderer, VIDEO_WIDTH, VIDEO_HEIGHT);
	// TODO: Make the whole-display clearing match the backdrop line colors set in the game code.
	SDL_SetRenderDrawColor(Renderer, 128u, 128u, 128u, SDL_ALPHA_OPAQUE);

	SDL_RenderClear(Renderer);
	SDL_RenderPresent(Renderer);

	FramebufferTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
	SDL_SetTextureBlendMode(FramebufferTexture, SDL_BLENDMODE_NONE);
}

void CloseDisplay() {
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
}
