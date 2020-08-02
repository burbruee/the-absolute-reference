#include "Input.h"
#include "Debug.h"
#include "SoundEffect.h"
#include "Setting.h"
#include "Button.h"
#include "Credit.h"
#include "Coin.h"
#include "Save.h"
#include "HwData.h"

bool PauseAllowed;

void InitCredits() {
	_0x6064765 = 20u;
	_0x6064766 = 10u;
	PayCoins[PLAYER2] = 0u;
	PayCoins[PLAYER1] = 0u;
	Credits[PLAYER2] = 0u;
	Credits[PLAYER1] = 0u;
	Services = 0u;
}

static inline void NormalCoinMode(uint16_t price, uint8_t* credits, uint8_t* coins) {
	switch (price) {
		case 0u:
			while (*coins > 0u) {
				*credits += 1u;
				*coins -= 1u;
			}
			break;

		case 1u:
			while (*coins > 1u) {
				*credits += 1u;
				*coins -= 2u;
			}
			break;

		case 2u:
			while (*coins > 2u) {
				*credits += 1u;
				*coins -= 3u;
			}
			break;

		case 3u:
			while (*coins > 3u) {
				*credits += 1u;
				*coins -= 4u;
			}
			break;

		case 4u:
			while (*coins > 4u) {
				*credits += 1u;
				*coins -= 5u;
			}
			break;

		case 5u:
			while (*coins > 5u) {
				*credits += 1u;
				*coins -= 6u;
			}
			break;

		case 6u:
			while (*coins > 0u) {
				*credits += 2u;
				*coins -= 1u;
			}
			break;

		case 7u:
			while (*coins > 0u) {
				*credits += 3u;
				*coins -= 1u;
			}
			break;

		case 8u:
			while (*coins > 0u) {
				*credits += 4u;
				*coins -= 1u;
			}
			break;

		default:
			break;
	}
}

void UpdateInputs() {
	_0x6064756 = (~INPUTS[INPUT_UNUSED] >> 4) & 0xF;
	_0x6064757 = ~INPUTS[INPUT_UNUSED] & 0xF;

	SystemButtonsDown[PLAYER1] = ~INPUTS[INPUT_BUTTONS1P];
	SystemButtonsDown[PLAYER2] = ~INPUTS[INPUT_BUTTONS2P];

	ServiceInput serviceInput = ~INPUTS[INPUT_SERVICE];

	if ((~serviceInput & SERVICE_ADDSERVICE) == AddServiceInputOld) {
		AddService = false;
	}
	else {
		AddServiceInputOld = ~INPUTS[INPUT_SERVICE] & SERVICE_ADDSERVICE;
		if (NumAddServiceFrames > 8u) {
			AddService = true;
			PlaySoundEffect(SOUNDEFFECT_COIN);
			NumAddServiceFrames = 0u;
		}
	}

	bool updatePlayers = true;
	if (~INPUTS[INPUT_SERVICE] & SERVICE_ADDSERVICE) {
		NumAddServiceFrames++;
	}
	else {
		NumAddServiceFrames = 0u;
	}

	if (PauseAllowed && !(SystemButtonsDown[PLAYER1] & BUTTON_START)) {
		updatePlayers = false;
	}

	if (updatePlayers) {
		GameButtonsOld[PLAYER1] = GameButtonsDown[PLAYER1];
		GameButtonsOld[PLAYER2] = GameButtonsDown[PLAYER2];

		GameButtonsDown[PLAYER1] = ~INPUTS[INPUT_BUTTONS1P];
		GameButtonsDown[PLAYER2] = ~INPUTS[INPUT_BUTTONS2P];

		GameButtonsNew[PLAYER1] = GameButtonsDown[PLAYER1] & ~GameButtonsOld[PLAYER1];
		GameButtonsNew[PLAYER2] = GameButtonsDown[PLAYER2] & ~GameButtonsOld[PLAYER2];
	}

	if (AddService && (Settings[SETTING_COINMODE] != COINMODE_FREE)) {
		if (Settings[SETTING_COINSLOT] == COINSLOT_INDIVIDUAL) {
			Services++;
		}
		else {
			(*Credits)++;
		}
	}

	serviceInput |= ~INPUTS[INPUT_SERVICE];
	if (Settings[SETTING_COINMODE] != COINMODE_FREE) {
		if (serviceInput & SERVICE_COIN1) {
			*(uint8_t*)&AddCoin[PLAYER1] = true;
		}
		else {
			if (AddCoin[PLAYER1]) {
				CoinCount++;
				PayCoins[PLAYER1]++;
				PlaySoundEffect(SOUNDEFFECT_COIN);
			}
			AddCoin[PLAYER1] = false;
		}

		if (serviceInput & SERVICE_COIN2) {
			*(uint8_t*)&AddCoin[PLAYER2] = true;
		}
		else {
			if (AddCoin[PLAYER2]) {
				CoinCount++;
				PayCoins[PLAYER2]++;
				PlaySoundEffect(SOUNDEFFECT_COIN);
			}
			AddCoin[PLAYER2] = false;
		}
	}

	if (PayCoins[PLAYER1] != 0u) {
		if (Settings[SETTING_COINMODE] == COINMODE_NORMAL) {
			NormalCoinMode(Settings[SETTING_PRICE1P], &Credits[PLAYER1], &PayCoins[PLAYER1]);
		}
		else if (Settings[SETTING_COINMODE] == COINMODE_DOUBLE) {
			while (PayCoins[PLAYER1] > 0u) {
				Credits[PLAYER1]++;
				PayCoins[PLAYER1]--;
			}
		}
		else if (Settings[SETTING_COINMODE] == COINMODE_FREE) {
			Credits[PLAYER1] = 0u;
		}
	}

	if (PayCoins[PLAYER2] != 0u) {
		if (Settings[SETTING_COINSLOT] == COINSLOT_SAME) {
			if (Settings[SETTING_COINMODE] == COINMODE_FREE) {
				PayCoins[PLAYER2] = 0u;
			}
			else if (Settings[SETTING_COINMODE] == COINMODE_DOUBLE) {
				while (PayCoins[PLAYER2] > 0u) {
					(*Credits)++;
					PayCoins[PLAYER2]--;
				}
			}
			else {
				NormalCoinMode(Settings[SETTING_PRICE2P], Credits, &PayCoins[PLAYER2]);
			}
		}
		else if (Settings[SETTING_COINMODE] == COINMODE_FREE) {
			PayCoins[PLAYER2] = 0u;
		}
		else if (Settings[SETTING_COINMODE] == COINMODE_DOUBLE) {
			while (PayCoins[PLAYER2] > 0u) {
				Credits[PLAYER2]++;
				PayCoins[PLAYER2]--;
			}
		}
		else {
			NormalCoinMode(Settings[SETTING_PRICE1P], &Credits[PLAYER2], &PayCoins[PLAYER2]);
		}
	}

	if (Credits[PLAYER1] > 9u) {
		Credits[PLAYER1] = 9u;
	}

	if (Credits[PLAYER2] > 9u) {
		Credits[PLAYER2] = 9u;
	}

	if (Services > 9u) {
		Services = 9u;
	}

	if (Debug && (~ServiceInputOld & serviceInput & SERVICE_TILT)) {
		if (Debug) {
			if (PauseAllowed) {
				PauseAllowed = false;
			}
			else {
				PauseAllowed = true;
			}
		}
	}

	ButtonsDown[PLAYER1] = ~INPUTS[INPUT_BUTTONS1P];
	ButtonsDown[PLAYER2] = ~INPUTS[INPUT_BUTTONS2P];
	ServiceInputOld = serviceInput;
}

bool CanStart(PlayerNum playerNum, bool noDouble) {
	if (Settings[SETTING_COINMODE] == COINMODE_FREE) {
		return true;
	}
	else {
		// BUG: In the original's SH-2 code, "credits" would be set to
		// point to SH-2 address 0x00000000 for playerNum ==
		// PLAYER1, or 0x00000001 for playerNum == PLAYER2. The bug
		// produces correct behavior, because those SH-2 addresses
		// store a zero, as the code expects.
		//
		// The bug only occurs when the coin slot setting is individual
		// and no services nor credits are available.
		uint8_t noCredits[NUMPLAYERS] = { 0u, 0u };
		uint8_t* credits = &noCredits[playerNum];

		if (Settings[SETTING_COINSLOT] == COINSLOT_INDIVIDUAL) {
			if (playerNum == PLAYER2) {
				if (Services > 0u) {
					credits = &Services;
				}
				else if (Credits[PLAYER2] > 0u) {
					credits = &Credits[PLAYER2];
				}
			}
			else {
				if (Services > 0u) {
					credits = &Services;
				}
				else if (Credits[PLAYER1] > 0u) {
					credits = &Credits[PLAYER1];
				}
			}
		}
		else if (Services > 0u) {
			credits = &Services;
		}
		else {
			credits = Credits;
		}

		if (Settings[SETTING_COINMODE] == COINMODE_DOUBLE) {
			if (noDouble) {
				return *credits > 0u;
			}
			else {
				uint8_t minCredits;
				if (Settings[SETTING_COINSLOT] == COINSLOT_INDIVIDUAL) {
					if (credits == &Services) {
						minCredits = 0u;
					}
					else {
						minCredits = 1u;
					}
				}
				else {
					minCredits = 1u;
				}

				return *credits > minCredits;
			}
		}
		else {
			return *credits > 0u;
		}
	}
}

void CheckBuyGame(PlayerNum playerNum, bool oneCreditPerGame) {
	if (Settings[SETTING_COINMODE] != COINMODE_FREE) {
		// BUG: See the BUG comment in CanStart.
		uint8_t noCredits[NUMPLAYERS] = { 0u, 0u };
		uint8_t *credits = &noCredits[playerNum];

		if (Settings[SETTING_COINSLOT] == COINSLOT_INDIVIDUAL) {
			if (playerNum == PLAYER2) {
				if (Services > 0u) {
					credits = &Services;
				}
				else if (Credits[PLAYER2] > 0u) {
					credits = &Credits[PLAYER2];
				}
			}
			else {
				if (Services > 0u) {
					credits = &Services;
				}
				else if (Credits[PLAYER1] > 0u) {
					credits = &Credits[PLAYER1];
				}
			}
		}
		else if (Services > 0u) {
			credits = &Services;
		}
		else {
			credits = Credits;
		}

		if (Settings[SETTING_COINMODE] == COINMODE_DOUBLE && !oneCreditPerGame && credits != &Services) {
			*credits -= 2u;
		}
		else {
			*credits -= 1u;
		}

		if ((int8_t)*credits < 0) {
			*credits = 0u;
		}
	}
}
