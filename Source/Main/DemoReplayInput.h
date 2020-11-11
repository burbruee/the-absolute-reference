#include "Input/Button.h"
#include "PlatformTypes.h"
#include "Lib/Macros.h"

#define DEMOREPLAY_LENGTH TIME(0, 21, 0)
#define DEMOREPLAY_FRAMES TIME(0, 19, 1)

extern ROMDATA ButtonInput DemoReplayInputTwin[DEMOREPLAY_LENGTH * NUMPLAYERS];
extern ROMDATA ButtonInput DemoReplayInputDoubles[DEMOREPLAY_LENGTH * NUMPLAYERS];
extern ROMDATA ButtonInput DemoReplayInputVersus[DEMOREPLAY_LENGTH * NUMPLAYERS];
