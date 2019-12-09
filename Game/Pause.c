#include "Pause.h"

PauseMode NextPauseMode;

void SetNextPauseMode(PauseMode nextPauseMode) {
	NextPauseMode = nextPauseMode;
}

void DisablePause() {
	NextPauseMode = PAUSEMODE_NOPAUSE;
}

void GetNextPauseMode() {
	return NextPauseMode;
}
