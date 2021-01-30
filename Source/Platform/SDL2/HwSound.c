#include "HwData.h"
#include "HwSound.h"
#include "Sound/Sound.h"

static uint8_t SoundWaveTableRegs[256];

void SoundControlStartInit() {
	PcmFirstLeftChannel = 14u;
	UNK_60B186A = 0;
}

void SoundControlStatusWait(const SoundStatusFlag flags) {

}

uint8_t SoundControlRead(const uint8_t i) {
	return 0u;
}

void SoundControlWrite(const uint8_t i, const uint8_t value) {

}
