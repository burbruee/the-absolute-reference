#include "PlatformUpdate.h"
#include "Platform/Util/AccessConfig.h"
#include "Platform/Util/AccessData.h"
#include "Platform/SDL2/AccessDisplay.h"
#include "Platform/Util/Render.h"
#include "Main/Frame.h"

Uint64 PlatformCurrentTime;
Uint64 PlatformTimeAccumulator;

static bool KeyWithModsPressed(const KeySetting keySetting, const Uint8* const keyboardState, const SDL_Keymod modState) {
#define CHECK_MOD(settingMod, modName) ((((settingMod) & (modName)) == KMOD_NONE) || (modState & (settingMod) & (modName)))
#define CHECK_ALL_MODS(settingMod) (CHECK_MOD((settingMod), KMOD_SHIFT) && CHECK_MOD((settingMod), KMOD_CTRL) && CHECK_MOD((settingMod), KMOD_ALT) && CHECK_MOD((settingMod), KMOD_GUI))
	if (keySetting.code != SDLK_UNKNOWN) {
		if (keyboardState[SDL_GetScancodeFromKey(keySetting.code)] && CHECK_ALL_MODS(keySetting.mod)) {
			return true;
		}
	}
	else if (keySetting.mod != KMOD_NONE && CHECK_ALL_MODS(keySetting.mod)) {
		return true;
	}
#undef CHECK_ALL_MODS
#undef CHECK_MOD
	return false;
}

void PlatformUpdateInputs() {
	SDL_PumpEvents();

	bool pressed[NUMINPUTS][8] = { 0 };

	const Uint8* const keyboardState = SDL_GetKeyboardState(NULL);
	SDL_Keymod modState = SDL_GetModState() & ~(KMOD_NUM | KMOD_CAPS);
	for (size_t i = 0u; i < NUMINPUTS; i++) {
		for (size_t j = 0u; j < 8u; j++) {
			if (KeyWithModsPressed(InputConfigKeyboard[i][j], keyboardState, modState)) {
				pressed[i][j] = true;
			}
		}
	}

	if (SDL_QuitRequested() || KeyWithModsPressed(InputApplicationKeyboardQuit, keyboardState, modState)) {
		exit(EXIT_SUCCESS);
	}

	for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++) {
		if (Joysticks[playerNum]) {
			for (size_t i = 0u; i < NUMINPUTS; i++) {
				for (size_t j = 0u; j < 8u; j++) {
					if (InputConfigJoystickButtons[playerNum][i][j] >= 0) {
						if (SDL_JoystickGetButton(Joysticks[playerNum], InputConfigJoystickButtons[playerNum][i][j])) {
							pressed[i][j] = true;
						}
					}
					if (InputConfigJoystickAxes[playerNum][i][j][0] >= 0) {
						int minpos = InputConfigJoystickAxes[playerNum][i][j][1];
						int position = SDL_JoystickGetAxis(Joysticks[playerNum], InputConfigJoystickAxes[playerNum][i][j][0]);
						if (minpos > 0 && position > 0 && position >= minpos) {
							pressed[i][j] = true;
						}
						else if (minpos < 0 && position < 0 && position <= minpos) {
							pressed[i][j] = true;
						}
					}
					if (InputConfigJoystickHats[playerNum][i][j][0] >= 0) {
						if (SDL_JoystickGetHat(Joysticks[playerNum], InputConfigJoystickHats[playerNum][i][j][0]) == InputConfigJoystickHats[playerNum][i][j][1]) {
							pressed[i][j] = true;
						}
					}
				}
			}
		}
	}

	for (size_t i = 0u; i < NUMINPUTS; i++) {
		for (size_t j = 0u; j < 8u; j++) {
			if (pressed[i][j]) {
				INPUTS[i] &= ~(1 << j);
			}
			else {
				INPUTS[i] |= 1 << j;
			}
		}
	}
}

void PlatformFrame() {
	// TODO: Implement more fully. This is just a placeholder to have some level of functionality.
	RandScale += (uint32_t)SDL_GetPerformanceCounter();

	NumVblanks++;

	SDL_Delay(1);
}

static Color Framebuffer[VIDEO_HEIGHT * VIDEO_WIDTH];
void PlatformFinishUpdate() {
	const Uint64 gameFrameDuration = (Uint64)(FRAME_DURATION * SDL_GetPerformanceFrequency());

	if (PlatformTimeAccumulator < gameFrameDuration || VsyncUpdateRate) {
		Render(Framebuffer, TileData);
		void* pixels;
		int pitch;
		if (SDL_LockTexture(FramebufferTexture, NULL, &pixels, &pitch) < 0) {
			fprintf(stderr, "Failed locking framebuffer texture: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
		for (size_t y = 0u; y < VIDEO_HEIGHT; y++) {
			// Being able to use memcpy here is why the code uses the Color typedef and
			// COLOR* macros. The game code and frontend have matching pixel formats, so we
			// don't have to do any conversion. The SDL2 texture rows *might* be longer than
			// VIDEO_WIDTH though, so we can't optimize it to a whole-framebuffer copy, but
			// this is pretty good. We also can't directly pass the texture pixels pointer
			// to the Render function, because the Render function has to read the
			// framebuffer for alpha blending, and SDL2 doesn't let us read the pixels of a
			// locked texture.
			// -Brandon McGriff
			memcpy((uint8_t*)pixels + y * pitch, Framebuffer + y * VIDEO_WIDTH, sizeof(Color) * VIDEO_WIDTH);
		}
		SDL_UnlockTexture(FramebufferTexture);
	}

	while (PlatformTimeAccumulator < gameFrameDuration || VsyncUpdateRate) {
		if (SDL_RenderClear(Renderer) < 0) {
			fprintf(stderr, "Failed clearing screen: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}

		if (SDL_RenderCopy(Renderer, FramebufferTexture, NULL, NULL) < 0) {
			fprintf(stderr, "Failed copying frame buffer texture to the screen: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}

		SDL_RenderPresent(Renderer);
		if (!Vsync) {
			SDL_Delay(1);
		}

		if (VsyncUpdateRate) {
			return;
		}

		Uint64 newTime = SDL_GetPerformanceCounter();
		Uint64 renderFrameDuration = newTime - PlatformCurrentTime;
		if (renderFrameDuration > SDL_GetPerformanceFrequency() / 4u) {
			renderFrameDuration = SDL_GetPerformanceFrequency() / 4u;
		}
		PlatformCurrentTime = newTime;

		PlatformTimeAccumulator += renderFrameDuration;
	}

	PlatformTimeAccumulator -= gameFrameDuration;
}
