#include "Platform/SDL2/Display.h"
#include "Platform/Util/Config.h"
#include "Platform/Util/Data.h"
#include "Video/Video.h"

SDL_Window* Window;
SDL_Renderer* Renderer;
SDL_Texture* FramebufferTexture;

bool OpenDisplay() {
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
		fprintf(stderr, "Failed creating window: %s\n", SDL_GetError());
		return false;
	}
	if (!(Renderer = SDL_CreateRenderer(Window, 0, SDL_RENDERER_ACCELERATED | (Vsync ? SDL_RENDERER_PRESENTVSYNC : 0u)))) {
		fprintf(stderr, "Failed creating renderer: %s\n", SDL_GetError());
		return false;
	}
	if (SDL_RenderSetLogicalSize(Renderer, VIDEO_WIDTH, VIDEO_HEIGHT) < 0) {
		fprintf(stderr, "Failed setting renderer logical size: %s\n", SDL_GetError());
		return false;
	}
	// TODO: Make the whole-display clearing match the backdrop line colors set in the game code.
	if (SDL_SetRenderDrawColor(Renderer, 128u, 128u, 128u, SDL_ALPHA_OPAQUE) < 0) {
		fprintf(stderr, "Failed setting render draw color: %s\n", SDL_GetError());
		return false;
	}

	if (SDL_RenderClear(Renderer) < 0) {
		fprintf(stderr, "Initial render clear failed: %s\n", SDL_GetError());
		return false;
	}
	SDL_RenderPresent(Renderer);

	if ((FramebufferTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT)) == NULL) {
		fprintf(stderr, "Failed creating framebuffer texture: %s\n", SDL_GetError());
		return false;
	}
	if (SDL_SetTextureBlendMode(FramebufferTexture, SDL_BLENDMODE_NONE) < 0) {
		fprintf(stderr, "Failed setting framebuffer texture blend mode: %s\n", SDL_GetError());
		return false;
	}

	printf("Successfully opened display.\n\n");
	return true;
}

void CloseDisplay() {
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
}
