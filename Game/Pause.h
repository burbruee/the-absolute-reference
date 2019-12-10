#pragma once

// All lesser pause modes are active for a given pause mode. So PAUSEMODE_GAME
// enables pausing of the background, too.
typedef enum PauseMode {
	PAUSEMODE_NOPAUSE = 0,
	PAUSEMODE_BG = 30,
	PAUSEMODE_GAME = 40
} PauseMode;

// Only read, and only to set CurrentPauseMode.
// Read/write in Pause.c
// To check the current pause mode, read CurrentPauseMode.
extern PauseMode NextPauseMode;

void SetNextPauseMode(PauseMode nextPauseMode);
void DisablePause();
void GetNextPauseMode();
