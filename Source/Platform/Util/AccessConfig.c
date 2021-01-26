#include "Platform/Util/AccessConfig.h"
#include "Platform/Util/ini.h"
#include "Video/Video.h"
#include "Lib/Macros.h"
#include "physfs.h"
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

DisplayMode DisplayModeSetting = DISPLAY_WINDOW;

int DisplayDimensions[2] = { VIDEO_WIDTH, VIDEO_HEIGHT };

int Vsync = 0;

int VsyncUpdateRate = 0;

const char* const DefaultConfig =
"[INPUT_BUTTONS1P_KEYBOARD]\n"
"BUTTON_START = Return\n"
"BUTTON_3 = ;\n"
"BUTTON_2 = L\n"
"BUTTON_1 = K\n"
"BUTTON_LEFT = A\n"
"BUTTON_RIGHT = D\n"
"BUTTON_DOWN = S\n"
"BUTTON_UP = W\n"
"\n"
"[INPUT_BUTTONS2P_KEYBOARD]\n"
"BUTTON_START = Keypad Enter\n"
"BUTTON_3 = Keypad 3\n"
"BUTTON_2 = Keypad 2\n"
"BUTTON_1 = Keypad 1\n"
"BUTTON_LEFT = Keypad 4\n"
"BUTTON_RIGHT = Keypad 6\n"
"BUTTON_DOWN = Keypad 5\n"
"BUTTON_UP = Keypad 8\n"
"\n"
"[INPUT_SERVICE_KEYBOARD]\n"
"SERVICE_COIN1 = Right Shift\n"
"SERVICE_COIN2 = Keypad +\n"
"SERVICE_ADDSERVICE = Tab\n"
"SERVICE_TEST = Escape\n"
"SERVICE_TILT = Backspace\n";

static int StringCompareNoCase(const char* a, const char* b) {
	int cmp;
	while ((cmp = tolower(*a) - tolower(*b)) == 0 && *a++ && *b++);
	return cmp;
}

static const char* const IniFileName = "taref.ini";

bool OpenConfig() {
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

	DisplayDimensions[0] = VIDEO_WIDTH;
	DisplayDimensions[1] = VIDEO_HEIGHT;

	Vsync = 0;

	VsyncUpdateRate = 0;

	const char* writeDir = PHYSFS_getWriteDir();
	if (!writeDir) {
		fprintf(stderr, "Failed to get write directory.\n\n");
		return false;
	}
	printf("Write directory at \"%s\".\n\n", writeDir);

	ini_t* config = NULL;
	PHYSFS_Stat iniStat;
	if (!PHYSFS_stat(IniFileName, &iniStat) || iniStat.filetype != PHYSFS_FILETYPE_REGULAR || iniStat.filesize < 0) {
		fprintf(stderr, "INI file \"%s\" is invalid, using default configuration.\n\n", IniFileName);
		config = ini_create(DefaultConfig, strlen(DefaultConfig));

		if (!config) {
			fprintf(stderr, "Failed creating default configuration.\n\n");
			return false;
		}

		PHYSFS_file* iniFile = PHYSFS_openWrite(IniFileName);
		if (!iniFile) {
			fprintf(stderr, "Failed opening \"%s\" for writing.\n\n", IniFileName);
			return false;
		}
		if (PHYSFS_write(iniFile, DefaultConfig, strlen(DefaultConfig), 1u) == 0u) {
			fprintf(stderr, "Failed writing \"%s\".\n\n", IniFileName);
			return false;
		}

		printf("Created default configuration.\n\n");
	}
	else {
		const size_t iniSize = iniStat.filesize;

		PHYSFS_File* const iniFile = PHYSFS_openRead(IniFileName);
		if (!iniFile) {
			fprintf(stderr, "Failed opening \"%s\" for reading.\n\n", IniFileName);
			return false;
		}
		char* const iniData = malloc(iniSize);
		assert(iniData != NULL);
		if (iniData == NULL) {
			return false;
		}

		if (PHYSFS_readBytes(iniFile, iniData, iniSize) != iniSize) {
			fprintf(stderr, "Error while reading \"%s\".\n\n", IniFileName);
			PHYSFS_close(iniFile);
			free(iniData);
			return false;
		}
		PHYSFS_close(iniFile);
		config = ini_create(iniData, iniSize);
		free(iniData);

		if (!config) {
			fprintf(stderr, "Failed loading configuration from \"%s\".\n\n", IniFileName);
			return false;
		}
		printf("Opened configuration INI \"%s\".\n\n", IniFileName);
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

		int deviceIndex;
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
							if (!StringCompareNoCase(fields[0], devicePlayers[i])) {
								playerNum = i;
								playerNumSet = true;
								break;
							}
						}
					}
					if (playerNumSet && fieldsRead >= 2) {
						if (!StringCompareNoCase(fields[1], "Button") && fieldsRead >= 3) {
							int button;
							if (sscanf(fields[2], "%d", &button) == 1 && button >= 0) {
								InputConfigJoystickButtons[playerNum][i][j] = button;
							}
						}
						else if (!StringCompareNoCase(fields[1], "Axis") && fieldsRead >= 4) {
							int num;
							float minpos;
							if (sscanf(fields[2], "%d", &num) == 1 && num >= 0 && sscanf(fields[3], "%f%%", &minpos) == 1 && minpos <= 100.0f && minpos >= -100.0f) {
								InputConfigJoystickAxes[playerNum][i][j][0] = num;
								InputConfigJoystickAxes[playerNum][i][j][1] = (Sint16)((minpos / 100.0f) * 32767.0f);
							}
						}
						else if (!StringCompareNoCase(fields[1], "Hat") && fieldsRead >= 4) {
							int num;
							if (sscanf(fields[2], "%d", &num) == 1 && num >= 0) {
								if (!StringCompareNoCase(fields[3], "Up")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_UP;
								}
								else if (!StringCompareNoCase(fields[3], "Right")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHT;
								}
								else if (!StringCompareNoCase(fields[3], "Down")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_DOWN;
								}
								else if (!StringCompareNoCase(fields[3], "Left")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_LEFT;
								}
								else if (!StringCompareNoCase(fields[3], "Rightup")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHTUP;
								}
								else if (!StringCompareNoCase(fields[3], "Rightdown")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_RIGHTDOWN;
								}
								else if (!StringCompareNoCase(fields[3], "Leftup")) {
									InputConfigJoystickHats[playerNum][i][j][0] = num;
									InputConfigJoystickHats[playerNum][i][j][1] = SDL_HAT_LEFTUP;
								}
								else if (!StringCompareNoCase(fields[3], "Leftdown")) {
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

			if (fieldsRead >= 1 && !StringCompareNoCase("Window", fields[0])) {
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
			else if (fieldsRead >= 2 && !StringCompareNoCase("Fullscreen", fields[0])) {
				if (!StringCompareNoCase("Exclusive", fields[1])) {
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
				else if (!StringCompareNoCase("Desktop", fields[1])) {
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

		// Checking for this option must happen after vsync, to ensure vsync gets enabled if this option is enabled.
		if (ini_sget(config, "VIDEO", "VSYNC_UPDATE_RATE", "%d", &VsyncUpdateRate)) {
			VsyncUpdateRate = !!VsyncUpdateRate;
			Vsync = 1;
		}
		else {
			VsyncUpdateRate = 0;
		}
	}

	printf("Finished reading configuration.\n\n");
	return true;
}

void SaveConfig() {
	// TODO: Implement once settings can be changed in-game.
}

void CloseConfig() {
	for (PlayerNum i = PLAYER1; i < lengthof(Joysticks); i++) {
		if (Joysticks[i] && SDL_JoystickGetAttached(Joysticks[i])) {
			SDL_JoystickClose(Joysticks[i]);
		}
	}
}
