#pragma once

#include "Game/Player.h"
#include "Game/Frame.h"
#include "Game/Input.h"
#include "Game/Button.h"
#include "Game/UnknownSprite.h"
#include "Game/Video.h"
#include "Game/HwData.h"
#include <stdbool.h>

#define UPDATE() \
	while (PauseAllowed) { \
		UpdateInputs(); \
		RandScale = 0u; \
		NumVblanks = 0u; \
		while (IRQCTRL[1] & 1) { \
			RandScale++; \
		} \
		VblankFinished = false; \
		while (!VblankFinished) { \
			RandScale++; \
		} \
		VblankFinished = false; \
		if (SystemButtonsDown[PLAYER1] & BUTTON_START) { \
			UpdatePlayers(); \
			break; \
		} \
	} \
	_0x602BA70(); \
	RandScale = 0u; \
	while (IRQCTRL[1] & 1) { \
		RandScale++; \
	} \
	VblankFinished = false; \
	while (!VblankFinished) { \
		RandScale++; \
	} \
	SetVideo(); \
	_0x602AA64(); \
	UpdatePalCycles(); \
	VblankFinished = false; \
	IRQCTRL[1] |= 1;

