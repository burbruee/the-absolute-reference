#include "Platform/Util/Config.h"
#include "Platform/Util/ini.h"
#include "Video/Video.h"
#include "Lib/Macros.h"
#include <assert.h>
#include <stdbool.h>

SDL_Keycode InputConfigKeyboard[NUMINPUTS][8];

SDL_Joystick* Joysticks[NUMPLAYERS];
int InputConfigJoystickButtons[NUMPLAYERS][NUMINPUTS][8] = {
	{
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 }
	},
	{
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1 }
	}
};
int InputConfigJoystickAxes[NUMPLAYERS][NUMINPUTS][8][2] = {
	{
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } }
	},
	{
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } },
		{ { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } }
	}
};
int InputConfigJoystickHats[NUMPLAYERS][NUMINPUTS][8][2] = {
	{
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } }
	},
	{
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, -1 } }
	}
};

DisplayMode DisplayModeSetting;

int DisplayDimensions[2];

int Vsync;

static int strcmp_nocase(const char *a, const char *b) {
	int cmp;
	while ((cmp = tolower(*a) - tolower(*b)) == 0 && *a++ && *b++);
	return cmp;
}

void OpenConfig(const char* const fileName) {
	for (size_t i = 0u; i < lengthof(InputConfigKeyboard); i++) {
		for (size_t j = 0u; j < lengthof(*InputConfigKeyboard); j++) {
			InputConfigKeyboard[i][j] = SDLK_UNKNOWN;
		}
	}

	for (size_t i = 0u; i < lengthof(Joysticks); i++) {
		Joysticks[i] = NULL;
	}

	for (PlayerNum playerNum = PLAYER1; playerNum < lengthof(InputConfigJoystickButtons); playerNum++) {
		for (size_t i = 0u; i < lengthof(*InputConfigJoystickButtons); i++) {
			for (size_t j = 0u; j < lengthof(**InputConfigJoystickButtons); j++) {
				InputConfigJoystickButtons[playerNum][i][j] = -1;
			}
		}
	}

	for (PlayerNum playerNum = PLAYER1; playerNum < lengthof(InputConfigJoystickAxes); playerNum++) {
		for (size_t i = 0u; i < lengthof(*InputConfigJoystickAxes); i++) {
			for (size_t j = 0u; j < lengthof(**InputConfigJoystickAxes); j++) {
				InputConfigJoystickAxes[playerNum][i][j][0] = -1;
				InputConfigJoystickAxes[playerNum][i][j][1] = 0;
			}
		}
	}

	for (PlayerNum playerNum = PLAYER1; playerNum < lengthof(InputConfigJoystickHats); playerNum++) {
		for (size_t i = 0u; i < lengthof(*InputConfigJoystickHats); i++) {
			for (size_t j = 0u; j < lengthof(**InputConfigJoystickHats); j++) {
				InputConfigJoystickHats[playerNum][i][j][0] = -1;
				InputConfigJoystickHats[playerNum][i][j][1] = -1;
			}
		}
	}

	DisplayModeSetting = DISPLAY_WINDOW;

	DisplayDimensions[0] = 0;
	DisplayDimensions[1] = 0;

	Vsync = 0;

	ini_t* const config = ini_load(fileName);
	if (!config) {
		exit(EXIT_FAILURE);
	}

	{
#define GET_KEY(input, button, i) \
		do { \
			const char* const keyName = ini_get(config, #input "_KEYBOARD", #button); \
			if (keyName) { \
				InputConfigKeyboard[input][i] = SDL_GetKeyFromName(keyName); \
			} \
		} while (0)

		GET_KEY(INPUT_BUTTONS1P, BUTTON_START, 0);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_3, 1);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_2, 2);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_1, 3);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_LEFT, 4);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_RIGHT, 5);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_DOWN, 6);
		GET_KEY(INPUT_BUTTONS1P, BUTTON_UP, 7);

		GET_KEY(INPUT_BUTTONS2P, BUTTON_START, 0);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_3, 1);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_2, 2);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_1, 3);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_LEFT, 4);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_RIGHT, 5);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_DOWN, 6);
		GET_KEY(INPUT_BUTTONS2P, BUTTON_UP, 7);

		GET_KEY(INPUT_SERVICE, SERVICE_COIN1, 0);
		GET_KEY(INPUT_SERVICE, SERVICE_COIN2, 1);
		GET_KEY(INPUT_SERVICE, SERVICE_ADDSERVICE, 2);
		GET_KEY(INPUT_SERVICE, SERVICE_TEST, 5);
		GET_KEY(INPUT_SERVICE, SERVICE_TILT, 6);

#undef GET_KEY
	}

	if (SDL_NumJoysticks() > 0) {
		int deviceIndex;
		const char* const joystickSections[NUMPLAYERS] = {
			"INPUT_JOYSTICK1P",
			"INPUT_JOYSTICK2P"
		};
		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			SDL_Joystick* joystick = SDL_JoystickOpen(i);
			bool joystickUsed = false;
			if (!joystick) {
				continue;
			}
			for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++) {
				const char* name = NULL;
				if (!Joysticks[playerNum] && (name = ini_get(config, joystickSections[playerNum], "DEVICE_NAME")) && !strcmp(SDL_JoystickName(joystick), name)) {
					Joysticks[playerNum] = joystick;
					joystickUsed = true;
					break;
				}
			}
			if (!joystickUsed) {
				SDL_JoystickClose(joystick);
			}
		}
		for (PlayerNum playerNum = PLAYER1; playerNum < NUMPLAYERS; playerNum++) {
			if (!Joysticks[playerNum] && ini_sget(config, joystickSections[playerNum], "DEVICE_INDEX", "%d", &deviceIndex) && deviceIndex >= 0 && deviceIndex < SDL_NumJoysticks()) {
				Joysticks[playerNum] = SDL_JoystickOpen(deviceIndex);
			}
		}

		const char* const inputSections[NUMINPUTS] = {
			"INPUT_BUTTONS1P_JOYSTICK",
			"INPUT_BUTTONS2P_JOYSTICK",
			"INPUT_UNUSED_JOYSTICK",
			"INPUT_SERVICE_JOYSTICK"
		};
		const char* const inputKeys[NUMINPUTS][8] = {
			{
				"BUTTON_START",
				"BUTTON_3",
				"BUTTON_2",
				"BUTTON_1",
				"BUTTON_LEFT",
				"BUTTON_RIGHT",
				"BUTTON_DOWN",
				"BUTTON_UP"
			},
			{
				"BUTTON_START",
				"BUTTON_3",
				"BUTTON_2",
				"BUTTON_1",
				"BUTTON_LEFT",
				"BUTTON_RIGHT",
				"BUTTON_DOWN",
				"BUTTON_UP"
			},
			{
				"",
				"",
				"",
				"",
				"",
				"",
				"",
				""
			},
			{
				"SERVICE_COIN1",
				"SERVICE_COIN2",
				"SERVICE_ADDSERVICE",
				"",
				"",
				"SERVICE_TEST",
				"SERVICE_TILT"
				"",
				""
			}
		};
		const char* const devicePlayers[NUMPLAYERS] = {
			"1P",
			"2P"
		};
		for (size_t i = 0u; i < NUMINPUTS; i++) {
			for (size_t j = 0u; j < 8u; j++) {
				const char* const inputSetting = ini_get(config, inputSections[i], inputKeys[i][j]);
				if (inputSetting) {
					const size_t len = strlen(inputSetting);
					char* fields[4];
					for (size_t i = 0u; i < lengthof(fields); i++) {
						fields[i] = malloc(len + 1u);
						assert(fields[i] != NULL);
					}
					const int fieldsRead = sscanf(inputSetting, "%s %s %s %s", fields[0], fields[1], fields[2], fields[3]);

					bool playerNumSet = false;
					PlayerNum playerNum;
					if (fieldsRead >= 1) {
						for (PlayerNum i = PLAYER1; i < NUMPLAYERS; i++) {
							if (!strcmp_nocase(fields[0], devicePlayers[i])) {
								playerNum = i;
								playerNumSet = true;
								break;
							}
						}
					}
					if (playerNumSet && fieldsRead >= 2) {
						if (!strcmp_nocase(fields[1], "Button") && fieldsRead >= 3) {
							int button;
							if (sscanf(fields[2], "%d", &button) == 1 && button >= 0) {
								InputConfigJoystickButtons[playerNum][i][j] = button;
							}
						}
						else if (!strcmp_nocase(fields[1], "Axis") && fieldsRead >= 4) {
							int num;
							float minpos;
							if (sscanf(fields[2], "%d", &num) == 1 && num >= 0 && sscanf(fields[3], "%f%%", &minpos) == 1 && minpos <= 100.0f && minpos >= -100.0f) {
								InputConfigJoystickAxes[playerNum][i][j][0] = num;
								InputConfigJoystickAxes[playerNum][i][j][1] = (Sint16)((minpos / 100.0f) * 32767.0f);
							}
						}
						else if (!strcmp_nocase(fields[1], "Hat") && fieldsRead >= 4) {
							int num;
							if (sscanf(fields[2], "%d", &num) == 1 && num >= 0) {
								if (!strcmp_nocase(fields[3], "Up")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_UP;
								}
								else if (!strcmp_nocase(fields[3], "Right")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHT;
								}
								else if (!strcmp_nocase(fields[3], "Down")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_DOWN;
								}
								else if (!strcmp_nocase(fields[3], "Left")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_LEFT;
								}
								else if (!strcmp_nocase(fields[3], "Rightup")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHTUP;
								}
								else if (!strcmp_nocase(fields[3], "Rightdown")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHTDOWN;
								}
								else if (!strcmp_nocase(fields[3], "Leftup")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_LEFTUP;
								}
								else if (!strcmp_nocase(fields[3], "Leftdown")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_LEFTDOWN;
								}
							}
						}
					}

					for (size_t i = 0u; i < lengthof(fields); i++) {
						free(fields[i]);
					}
				}
			}
		}
	}

	{
		const char* const displayMode = ini_get(config, "VIDEO", "DISPLAY_MODE");
		if (displayMode) {
			const size_t len = strlen(displayMode);
			char* fields[3];
			for (size_t i = 0u; i < lengthof(fields); i++) {
				fields[i] = malloc(len + 1);
				assert(fields[i] != NULL);
			}
			const int fieldsRead = sscanf(displayMode, "%s %s %s", fields[0], fields[1], fields[2]);

			if (fieldsRead >= 1 && !strcmp_nocase("Window", fields[0])) {
				DisplayModeSetting = DISPLAY_WINDOW;
				if (fieldsRead >= 2 && sscanf(fields[1], "%dx%d", &DisplayDimensions[0], &DisplayDimensions[1]) == 2) {
					if (DisplayDimensions[0] <= VIDEO_WIDTH || DisplayDimensions[1] <= VIDEO_HEIGHT) {
						DisplayDimensions[0] = VIDEO_WIDTH;
						DisplayDimensions[1] = VIDEO_HEIGHT;
					}
				}
				else {
					DisplayDimensions[0] = VIDEO_WIDTH;
					DisplayDimensions[1] = VIDEO_HEIGHT;
				}
			}
			else if (fieldsRead >= 2 && !strcmp_nocase("Fullscreen", fields[0])) {
				if (!strcmp_nocase("Exclusive", fields[1])) {
					if (fieldsRead >= 3 && sscanf(fields[2], "%dx%d", &DisplayDimensions[0], &DisplayDimensions[1]) == 2) {
						if (DisplayDimensions[0] <= VIDEO_WIDTH || DisplayDimensions[1] <= VIDEO_HEIGHT) {
							DisplayModeSetting = DISPLAY_WINDOW;
							DisplayDimensions[0] = VIDEO_WIDTH;
							DisplayDimensions[1] = VIDEO_HEIGHT;
						}
						else {
							DisplayModeSetting = DISPLAY_WINDOW;
							int readDimensions[2] = { DisplayDimensions[0], DisplayDimensions[1] };
							DisplayDimensions[0] = VIDEO_WIDTH;
							DisplayDimensions[1] = VIDEO_HEIGHT;

							const int num = SDL_GetNumDisplayModes(0);
							for (int i = 0; i < num; i++) {
								SDL_DisplayMode mode;
								if (SDL_GetDisplayMode(0, i, &mode) == 0 && mode.w == readDimensions[0] && mode.h == readDimensions[1]) {
									DisplayModeSetting = DISPLAY_FULLSCREENEXCLUSIVE;
									DisplayDimensions[0] = readDimensions[0];
									DisplayDimensions[1] = readDimensions[1];
									break;
								}
							}
						}
					}
					else {
						SDL_DisplayMode mode;
						if (SDL_GetDesktopDisplayMode(0, &mode) == 0) {
							DisplayModeSetting = DISPLAY_FULLSCREENEXCLUSIVE;
							DisplayDimensions[0] = mode.w;
							DisplayDimensions[1] = mode.h;
						}
						else {
							DisplayModeSetting = DISPLAY_WINDOW;
							DisplayDimensions[0] = VIDEO_WIDTH;
							DisplayDimensions[1] = VIDEO_HEIGHT;
						}
					}
				}
				else if (!strcmp_nocase("Desktop", fields[1])) {
					SDL_DisplayMode mode;
					if (SDL_GetDesktopDisplayMode(0, &mode) == 0) {
						DisplayModeSetting = DISPLAY_FULLSCREENDESKTOP;
						DisplayDimensions[0] = mode.w;
						DisplayDimensions[1] = mode.h;
					}
					else {
						DisplayModeSetting = DISPLAY_WINDOW;
						DisplayDimensions[0] = VIDEO_WIDTH;
						DisplayDimensions[1] = VIDEO_HEIGHT;
					}
				}
				else {
					DisplayModeSetting = DISPLAY_WINDOW;
					DisplayDimensions[0] = VIDEO_WIDTH;
					DisplayDimensions[1] = VIDEO_HEIGHT;
				}
			}
			else {
				DisplayModeSetting = DISPLAY_WINDOW;
				DisplayDimensions[0] = VIDEO_WIDTH;
				DisplayDimensions[1] = VIDEO_HEIGHT;
			}

			for (size_t i = 0u; i < lengthof(fields); i++) {
				free(fields[i]);
			}
		}
		else {
			DisplayModeSetting = DISPLAY_WINDOW;
			DisplayDimensions[0] = VIDEO_WIDTH;
			DisplayDimensions[1] = VIDEO_HEIGHT;
		}
	}

	{
		if (ini_sget(config, "VIDEO", "VSYNC", "%d", &Vsync)) {
			Vsync = !!Vsync;
		}
		else {
			Vsync = 0;
		}
	}
}

void CloseConfig() {
	for (PlayerNum i = PLAYER1; i < lengthof(Joysticks); i++) {
		if (Joysticks[i] && SDL_JoystickGetAttached(Joysticks[i])) {
			SDL_JoystickClose(Joysticks[i]);
		}
	}
}
