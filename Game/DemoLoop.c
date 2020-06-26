#include "DemoLoop.h"
#include "Coin.h"
#include "Input.h"
#include "Button.h"
#include "Video.h"
#include "Player.h"
#include "Sound.h"
#include "Frame.h"

// TODO: Just a guess, but this probably returns whether the demo loop should
// show the version title screen.
bool _0x06014234() {
	return
		AddCoin[PLAYER1] || AddCoin[PLAYER2] ||
		AddService ||
		(CanStart(PLAYER1, false) && (ButtonsDown[PLAYER1] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1))) ||
		(CanStart(PLAYER2, false) && (ButtonsDown[PLAYER2] & (BUTTON_START | BUTTON_3 | BUTTON_2 | BUTTON_1)));
}

void InitDemoLoop() {
	_0x6029814(0u, 0u, 0u, 0xFFu);
	SpritePriority[0] = 0x13u;
	SpritePriority[1] = 0x66u;
	_0x602AA4C();
	if (_0x6064750 != NULL) {
		_0x6024030(_0x6064750); // TODO
	}
	_0x6023790();
	_0x602AB9E();
	FreePalCycles(500u);
	_0x602419C();
	_0x60169DC();
	_0x602E72A(0u);
	InitPlayers();
	InitModeCodes(Players);
	InitModeCodes(Players + 1);
	_0x6064750 = NULL;
	Demo = false;
	ScreenTime = 0u;
	DemoScreen = DEMOSCREEN_START;
	DemoWaitTime = 0u;
	GameStartPlayer = STARTPLAYER_NONE;
	UpdateFrame();
	_0x602AA16(); // TODO
}

MainLoopState RunDemoLoop();

ScreenState StartRankingScreen();
ScreenState StartCopyrightScreen();
ScreenState StartTitleScreen();
ScreenState StartDeveloperScreen();
ScreenState StartVersionTitleScreen();
ScreenState StartDemoScreen();
