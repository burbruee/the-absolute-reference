#pragma once

#include "PlatformTypes.h"
#include "Video/Video.h"
#include <stdint.h>
#include <stdbool.h>

bool MountRoms();

// "Open" functions need their corresponding "Close" function called when you're done with the data they open.
// "Load" functions don't need anything closed, they merely read from programData and write to statically-allocated global data.

const uint8_t* OpenProgramData();
void CloseProgramData(const uint8_t* const programData);

#define TILEROM_SIZE 0x200000
#define NUMTILEROMS 16
#define TILEDATA_SIZE (TILEROM_SIZE * NUMTILEROMS)

extern uint8_t* TileData;
bool OpenTiles();
void CloseTiles();

void LoadReplays(const uint8_t* const programData);

extern ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
extern ROMDATA Color PalSmallText[NUMPALCOLORS_4BPP];
void LoadPals(const uint8_t* const programData);

void LoadBgMaps(const uint8_t* const programData);

void LoadObjects(const uint8_t* const programData);

#define SNDROM_SIZE 0x400000
extern uint8_t* SoundRomData;
bool OpenSound(const uint8_t* const programData);
void CloseSound();

// Convenience function that opens all data.
bool OpenData();

// Convenience function that closes all data opened by OpenData().
void CloseData();
