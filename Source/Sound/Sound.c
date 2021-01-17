#include "Sound/Sound.h"
#include "Main/Frame.h"
#include "Input/Credit.h"
#include "Eeprom/Eeprom.h"
#include "BuildData/BuildData.h"
#include "SoundControl.h"
#include <stdbool.h>

extern ROMDATA int16_t UNK_6035990[17][12];

typedef struct STRUCT_60B1638 {
	int8_t UNK_0;
	uint8_t* UNK_4;
	uint8_t* UNK_8;
} STRUCT_60B1638;
static STRUCT_60B1638 UNK_60B1638[16];
static uint8_t UNK_60B16F8[16][3];

#define SOUND_NUMVOICES 24
static uint8_t UNK_60B1728[SOUND_NUMVOICES][2];
static int8_t UNK_60B1758[SOUND_NUMVOICES];
static uint8_t UNK_60B1770[SOUND_NUMVOICES];
static uint8_t UNK_60B1788[SOUND_NUMVOICES];
static uint16_t UNK_60B17A0[SOUND_NUMVOICES];
static uint16_t UNK_60B17D0[SOUND_NUMVOICES];
static uint8_t UNK_60B1800[SOUND_NUMVOICES];
static uint8_t UNK_60B1818;
bool NoSound;
int16_t UNK_60B181C;
static uint16_t UNK_60B181E;
static int16_t UNK_60B1820[SOUND_NUMVOICES];
static uint32_t UNK_60B1850;
static uint32_t UNK_60B1858;
static uint8_t UNK_60B1860;
static uint16_t UNK_60B1862;
static uint8_t UNK_60B1865;
static uint8_t UNK_60B1866;
static uint8_t UNK_60B1867;
static uint8_t UNK_60B1868;
static uint8_t UNK_60B1869; // TODO: Appears voice numbers are assigned to this.
int16_t UNK_60B186A;
static uint16_t UNK_60B186C;
static uint8_t UNK_60B186E;
static uint8_t UNK_60B186F;
static uint32_t UNK_60B1870;
static uint32_t UNK_60B1874;
static uint32_t UNK_60B1878;
static uint32_t UNK_60B187C;

static void UNK_602DBE2(STRUCT_60B1638* const arg0);

void UNK_602DA38(uint32_t arg0) {
	UNK_60B1870 = arg0;
}

uint32_t UNK_602DA48(int32_t arg0) {
	return UNK_60B1858 = (arg0 * 48) / 60;
}

void UNK_602DA6E(uint32_t arg0) {
	UNK_60B187C = arg0;
}

void UNK_602DA90() {
	UNK_60B181C = 0x000Eu;
	UNK_602E586();
	UNK_602EB84();
	StopMusic();
	UNK_602EADC();
	UNK_60B1870 = 0u;
	UNK_60B1874 = 0u;
}

void UNK_602DAD4() {
	UNK_602EF9C();
	UNK_602E166();
	UNK_60B1850 += UNK_60B1858 * NumVblanks;
	NumVblanks = 0u;
	UNK_60B1860 = UNK_60B1850 / 60;
	UNK_60B1850 %= 60;
	UNK_6060028 = 0u;

	for (int8_t i = 0; i < 16; i++) {
		if (UNK_60B1638[i].UNK_4 != NULL) {
			UNK_602DBE2(&UNK_60B1638[i]);
		}
	}

	if (UNK_60B1878 != 0u && UNK_60B1870 != 0u && UNK_60B1874 != 0u) {
		UNK_6064767 = 0xFFu;
		PlayMusic(UNK_60B1874 - 1);
		UNK_60B1874 = 0u;
	}

	if (UNK_60B181E != 0u) {
		UNK_60B181E--;
	}
	UNK_60B1878 = 0u;
}

static void UNK_602DBE2(STRUCT_60B1638* const arg0) {
	for (int8_t var0 = UNK_60B1860, var1; var0 != 0;) {
		if (var0 < arg0->UNK_0) {
			arg0->UNK_0 -= var0;
			return;
		}
		var0 -= arg0->UNK_0;

		for (var1 = *arg0->UNK_4; var1 & 0x80u; var1 = *arg0->UNK_4) {
			arg0->UNK_4++;
			uint8_t var2 = var1 & 0x0Fu;
			uint8_t var3 = var1 & 0xF0u;
			if (var3 == 0x80u) {
				UNK_60B1865 = *arg0->UNK_4++;
				UNK_60B1866 = *arg0->UNK_4++;
				UNK_60B16F8[var2][1] = UNK_60B1866;
				if (UNK_60B16F8[var2][2] == 0) {
					UNK_60B186A = var2;
					if (var2 == 9u) {
						UNK_602F480();
					}
					else {
						UNK_602F0D2(UNK_60B16F8[var2][0]);
					}
				}
			}
			else if (var3 == 0x90u) {
				UNK_60B1865 = *arg0->UNK_4++;
				if (var2 == 9u) {
					UNK_602F57E();
				}
				else {
					UNK_602F2B0(UNK_60B16F8[var2][0]);
				}
			}
			else if (var3 == 0xC0u) {
				UNK_60B16F8[var2][0] = *arg0->UNK_4++;
			}
			else if (var1 == 0xD3u) {
				arg0->UNK_4 = NULL;
				return;
			}
			else if (var3 == 0xA0u) {
				arg0->UNK_8 = arg0->UNK_4;
			}
			else if (var3 == 0xB0u) {
				arg0->UNK_4 = arg0->UNK_8;
				if (var2 == lengthof(UNK_60B16F8) - 1) {
					UNK_60B1878 = 1u;
				}
			}
			else if (var1 == 0xD0u) {
				UNK_60B1858 = *arg0->UNK_4++;
				UNK_60B1850 = 0u;
				UNK_6060028 = 0u;
				NumVblanks = 0u;
			}
		}
		arg0->UNK_0 = var1;
		arg0->UNK_4++;
	}
}

void PlayMusic(Music music) {
	if (NoSound) {
		return;
	}
	if (UNK_60B1870 != 0u && UNK_6064767 != 0xFFu) {
		UNK_60B1874 = music + 1u;
		return;
	}

	UNK_60B1850 = 0u;
	UNK_60B1858 = 60u;
	UNK_6064767 = music;
	UNK_602EB4C();

	for (int32_t i = 0; i < lengthof(UNK_60B16F8); i++) {
		UNK_60B16F8[i][0] = 0u;
		UNK_60B16F8[i][1] = 0u;
		UNK_60B1638[i].UNK_0 = 0u;
		UNK_60B1638[i].UNK_4 = MidiPtr->UNK_F00[music][i];
		UNK_60B1638[i].UNK_8 = UNK_60B1638[i].UNK_4;
	}

	UNK_6060028 = 0u;
	NumVblanks = 0u;
}

void PlaySoundEffect(SoundEffect soundEffect) {
	if (soundEffect == SOUNDEFFECT_COIN) {
		UNK_602E56A();
		UNK_60B181E = 0x3Cu;
	}
	else if (NoSound) {
		return;
	}

	UNK_60B186C = soundEffect & 0x8000u;
	const uint16_t soundEffectNum = soundEffect & 0x7FFFu;

	for (int32_t i = UNK_60B181C; i < SOUND_NUMVOICES; i++) {
		if (UNK_60B17D0[i] == soundEffectNum) {
			UNK_602E0FC(i, soundEffectNum);
			return;
		}
	}

	for (int32_t i = 0; i < SOUND_NUMVOICES; i++) {
		for (int32_t j = UNK_60B181C; j < SOUND_NUMVOICES; j++) {
			if (UNK_60B1800[j] == (MidiPtr->UNK_300[soundEffectNum % lengthoffield(MidiData, UNK_300)].UNK_4 & 7) && UNK_60B1758[j] <= 0) {
				UNK_602E0FC(j, soundEffectNum);
				return;
			}
		}

		for (int32_t j = UNK_60B181C; j < SOUND_NUMVOICES; j++) {
			if (UNK_60B1800[j] == (MidiPtr->UNK_300[soundEffectNum % lengthoffield(MidiData, UNK_300)].UNK_4 & 7) && UNK_60B1758[j] != 0) {
				UNK_60B1758[j]--;
			}
		}
	}
}

void UNK_602E0FC(uint8_t voiceNum, uint16_t soundEffectNum) {
	if (UNK_60B17A0[voiceNum] == 0u) {
		UNK_60B1869 = voiceNum;
		UNK_602ECCA();
	}

	if (UNK_60B186C != 0u) {
		UNK_60B1758[voiceNum] = 0;
		UNK_60B17D0[voiceNum] = 0xFFFFu;
		UNK_60B17A0[voiceNum] = 0u;
	}
	else {
		UNK_60B17A0[voiceNum] = soundEffectNum + 1u;
	}
}

void UNK_602E166() {
	for (int32_t i = UNK_60B181C; i < SOUND_NUMVOICES; i++) {
		if (UNK_60B17A0[i] == 0u) {
			if (UNK_60B1820[i] != 0u) {
				UNK_60B1820[i]--;
				if (UNK_60B1820[i] < 4) {
					UNK_60B1869 = i;
					UNK_602ECCA();
				}
			}
		}
		else {
			uint16_t var0 = UNK_60B17A0[i] - 1;
			UNK_60B17A0[i] = 0;
			UNK_60B17D0[i] = var0;
			if (UNK_60B1800[i] == 0 && UNK_60B181E != 0 && var0 != 0) {
				UNK_60B1758[i] = 0;
				return;
			}
			UNK_60B1758[i] = SOUND_NUMVOICES - UNK_60B181C;
			UNK_60B1866 = MidiPtr->UNK_300[var0].UNK_2;
			UNK_60B1865 = MidiPtr->UNK_300[var0].UNK_5;
			UNK_60B1862 = MidiPtr->UNK_300[var0].UNK_0;
			UNK_60B1867 = MidiPtr->UNK_300[var0].UNK_3;
			UNK_60B1868 = 0x20u;
			UNK_60B186A = 0x10u;
			UNK_60B1869 = i;
			UNK_602E30A();
			UNK_60B1820[i] = 0x1A4u; // TODO: This might be a time value, TIME(0, 7, 0).
		}
	}
}

void UNK_602E30A() {
	const int32_t var0 = (int8_t)UNK_60B1869;

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, UNK_60B1869 + 0x68u);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, 0x38u);

	const int32_t var1 = UNK_60B1865 / 12;
	const int32_t var2 = UNK_60B1865 % 12;

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var0 + 0x38u);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, ((UNK_6035990[UNK_60B186A][var2] >> 7) & 7) | ((var1 - 5) << 4));

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var0 + 0x20u);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, (UNK_6035990[UNK_60B186A][var2] << 1) | ((UNK_60B1862 >> 8) & 1));

	// BUG: The intent was probably to check whether var3 is negative here, but the original code ended up doing an unsigned comparison.
	uint32_t var3 = UNK_60B1866;
	if (var3 < 0) {
		var3 += 3;
	}

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var0 + 0x50u);
	if (UNK_60B1818 != 0) {
		UNK_60B1866 = ((var3 >> 2) + (UNK_60B1866 >> 1)) | 1;
	}
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, ~(UNK_60B1866 << 1));

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var0 + 8);
	UNK_60B1770[var0] = 0u;
	if (var0 < UNK_60B181C) {
		UNK_60B1788[var0] = UNK_60B1868 | 0x87u;
	}
	else {
		UNK_60B1788[var0] = UNK_60B1868 | 0x89u;
	}
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, UNK_60B1862);
}

void UNK_602E560() {
	UNK_60B1818 = 1u;
}

void UNK_602E56A() {
	UNK_60B1818 = 0u;
}

void PlaySoundEffectCoin() {
	UNK_602E56A();
	PlaySoundEffect(SOUNDEFFECT_COIN);
}

void UNK_602E586() {
	for (UNK_60B1869 = (uint8_t)UNK_60B181C; UNK_60B1869 < 0x16u; UNK_60B1869++) {
		UNK_602ECCA();
	}
}

void StopMusic() {
	for (UNK_60B1869 = 0u; UNK_60B1869 < UNK_60B181C; UNK_60B1869++) {
		UNK_602ECCA();
	}

	for (int32_t i = 0; i < lengthof(UNK_60B1638); i++) {
		UNK_60B1638[i].UNK_4 = NULL;
	}

	UNK_6064767 = 0xFFu;
	UNK_60B1874 = 0u;
}

void DisableSound() {
	NoSound = true;
}

void EnableSound() {
	NoSound = false;
}

void UNK_602E64C(uint8_t arg0, uint8_t arg1) {
	UNK_60B16F8[arg0][2] = arg1 & 1u;
}


uint8_t UNK_602E68C(uint8_t arg0) {
	return UNK_60B16F8[arg0][2];
}

void UNK_602E6B8() {
	for (int8_t i = 0; i < 16; i++) {
		UNK_60B16F8[i][2] = 0u;
	}
}

void UNK_602E6F4() {
	for (int8_t i = 0; i < 16; i++) {
		UNK_60B16F8[i][2] = 1u;
	}
}

void UNK_602E72A(uint8_t arg0) {
	UNK_60B186E = (7 - (arg0 & 7)) * 8;

	UNK_602EC6C();
	SOUNDCTRL_WRITE(4, 0xF9u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(5, UNK_60B186E | UNK_60B186F);
}

void UNK_602E780(uint8_t arg0) {
	UNK_60B186F = 7 - (arg0 & 7);

	UNK_602EC6C();
	SOUNDCTRL_WRITE(4, 0xF9u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(5, UNK_60B186E | UNK_60B186F);
}

void InitSound() {
	SOUNDCTRL_STARTINIT();

	UNK_602EC6C();
	SOUNDCTRL_WRITE(2, 5u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(3, 3u);

	UNK_602EC6C();
	SOUNDCTRL_WRITE(4, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(5, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(4, 1u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(5, 0u);

	UNK_602EC6C();
	SOUNDCTRL_WRITE(0, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(1, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(0, 1u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(1, 0u);
	UNK_602EC6C();

	SOUNDCTRL_WRITE(2, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(3, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(2, 1u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(3, 0u);

	UNK_602EC6C();
	SOUNDCTRL_WRITE(0, 4u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(1, 0xE0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(0, 2u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(1, 0xFFu);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(0, 3u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(1, 0xFFu);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(0, 5u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(1, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(0, 8u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(1, 0u);

	UNK_602EC6C();
	SOUNDCTRL_WRITE(2, 2u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(3, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(2, 3u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(3, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(2, 8u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(3, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(2, 0xBDu);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(3, 0u);

	for (int8_t i = 0u; i < 22; i++) {
		UNK_602EC6C();
		SOUNDCTRL_WRITE(0, i + 0x40u);
		UNK_602EC6C();
		SOUNDCTRL_WRITE(1, 0x3Fu);
		UNK_602EC6C();
		SOUNDCTRL_WRITE(2, i + 0x40u);
		UNK_602EC6C();
		SOUNDCTRL_WRITE(3, 0x3Fu);
	}

	UNK_602EC6C();
	SOUNDCTRL_WRITE(4, 2u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(5, 0u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(4, 7u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(5, 0u);

	for (int8_t i = 0; i < SOUND_NUMVOICES; i++) {
		UNK_60B1869 = i;

		UNK_602ECCA();

		UNK_602EC6C();
		SOUNDCTRL_WRITE(4, i + 0x38u);
		UNK_602EC6C();
		SOUNDCTRL_WRITE(5, 0u);

		UNK_602EC6C();
		SOUNDCTRL_WRITE(4, i + 0x20u);
		UNK_602EC6C();
		SOUNDCTRL_WRITE(5, 0u);

		UNK_602EC6C();
		SOUNDCTRL_WRITE(4, i + 8u);
		UNK_602EC6C();
		SOUNDCTRL_WRITE(5, 0u);

		UNK_602EC98();
	}

	UNK_602EC6C();
	SOUNDCTRL_WRITE(4, 0xF8u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(5, 0x3Fu);

	UNK_602EC6C();
	SOUNDCTRL_WRITE(4, 0xF9u);
	UNK_602EC6C();
	SOUNDCTRL_WRITE(5, 0u);

	UNK_602EB4C();
	UNK_602EC5C();
	UNK_602E6B8();

	UNK_60B1818 = 0u;
	NoSound = false;
	UNK_60B186E = 0u;
	UNK_60B186F = 0u;
	UNK_60B187C = 0u;
}

void UNK_602EADC() {
	for (int32_t i = UNK_60B181C; i < SOUND_NUMVOICES; i++) {
		UNK_60B1788[i] = 0u;
		UNK_60B17A0[i] = 0u;
		UNK_60B1758[i] = 0u;
		UNK_60B1770[i] = 1u;
	}
	UNK_6060028 = 0u;
}

void UNK_602EB4C() {
	StopMusic();
	UNK_602EADC();
	UNK_60B1870 = 0u;
	UNK_60B1874 = 0u;
}

void UNK_602EB84() {
	UNK_60B1800[23] = 0u;
	UNK_60B1800[22] = 0u;
	UNK_60B1800[21] = 2u;
	UNK_60B1800[20] = 2u;
	UNK_60B1800[19] = 2u;
	UNK_60B1800[18] = 3u;
	UNK_60B1800[17] = 3u;
	UNK_60B1800[16] = 3u;
	UNK_60B1800[15] = 5u;
	UNK_60B1800[14] = 5u;
	UNK_60B181E = 0u;
	for (int32_t i = UNK_60B181C; i < SOUND_NUMVOICES; i++) {
		UNK_60B1758[i] = 0;
		UNK_60B17D0[i] = 0xFFFFu;
		UNK_60B1728[i][0] = 0xFFu;
		UNK_60B1728[i][1] = 0xFFu;
		UNK_60B1820[i] = 0;
	}
}

void UNK_602EC5C() {
	UNK_602E586();
	UNK_602EB84();
}

void UNK_602EC6C() {
	SOUNDCTRL_STARTWAIT();
	SOUNDCTRL_WAIT(1);
}

void UNK_602EC98() {
	SOUNDCTRL_STARTWAIT();
	SOUNDCTRL_WAIT(2);
}

void UNK_602ECCA() {
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, UNK_60B1869 + 0x50u);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, 0xFFu);

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, UNK_60B1869 + 0x68u);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, 0x78u);
}

void UNK_602ED40() {
	const int8_t var0 = UNK_60B1869;

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, UNK_60B1869 + 0x50u);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, 0xFFu);

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, UNK_60B1869 + 0x68u);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, 0x30u);

	const int32_t var1 = UNK_60B1865 / 12;
	const int32_t var2 = UNK_60B1865 % 12;

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var0 + 0x38);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, ((UNK_6035990[UNK_60B186A][var2] >> 7) & 7) | ((var1 - 5) << 4));

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var0 + 0x20u);
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, (UNK_6035990[UNK_60B186A][var2] << 1) | ((UNK_60B1862 >> 8) & 1));

	// BUG: The intent was probably to check whether var3 is negative here, but the original code ended up doing an unsigned comparison.
	uint32_t var3 = UNK_60B1866;
	if (var3 < 0) {
		var3 += 3;
	}

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var0 + 0x50u);
	if (UNK_60B1818 != 0) {
		UNK_60B1866 = (var3 >> 2) + (UNK_60B1866 >> 1);
	}
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, ~(UNK_60B1866 << 1));

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var0 + 8u);
	UNK_60B1770[var0] = 0u;
	if (var0 < UNK_60B181C) {
		UNK_60B1788[var0] = UNK_60B1868 | 0x87u;
	}
	else {
		UNK_60B1788[var0] = UNK_60B1868 | 0x89u;
	}
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, UNK_60B1862);
}

void UNK_602EF9C() {
	UNK_602EC98();
	uint8_t j = 0x68u;
	int32_t i = 0;
	while (i < SOUND_NUMVOICES) {
		if (UNK_60B1788[i] != 0u) {
			SOUNDCTRL_WAIT(1);
			SOUNDCTRL_WRITE(4, j);
			SOUNDCTRL_WAIT(1);
			SOUNDCTRL_WRITE(5, UNK_60B1788[i]);
			UNK_60B1788[i] = 0u;
		}
		j++;
		i++;
	}
}

void UNK_602F026(uint8_t arg0) {
	
	int8_t var0 = UNK_60B1869;
	uint8_t var1 = UNK_60B1869 + 0x68u;
	UNK_60B1728[UNK_60B1869][1] = 0xFFu;
	UNK_60B1728[var0][0] = 0xFFu;

	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(4, var1);
	UNK_60B1758[var0] = 0;
	UNK_60B1770[var0] = 1u;

	uint8_t var2;
	if (var0 < UNK_60B181C) {
		var2 = arg0 | 7u;
	}
	else {
		var2 = arg0 | 9u;
	}
	SOUNDCTRL_WAIT(1);
	SOUNDCTRL_WRITE(5, var2);
}

void UNK_602F0D2(uint8_t arg0) {
	if (MidiPtr->UNK_0[arg0] == 0xFFFFu) {
		return;
	}
	UNK_60B1869 = UNK_602F1BC();
	UNK_60B1728[UNK_60B1869][1] = UNK_60B1865 + UNK_60B187C;
	UNK_60B1758[UNK_60B1869] = UNK_60B181C + 1u;
	UNK_60B1728[UNK_60B1869][0] = arg0;
	UNK_60B1862 = MidiPtr->UNK_0[arg0];
	UNK_60B1868 = 0u;
	UNK_602ED40();
	for (uint8_t i = 0u; i < UNK_60B181C - 1; i++) {
		if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
			UNK_60B1770[i] = 1u;
		}
	}
}

uint8_t UNK_602F1BC() {
	uint8_t j = 0u;
	for (uint8_t i = 1u; i < UNK_60B181C - 1; i++) {
		if (UNK_60B1770[i] != 0 && UNK_60B1770[j] < UNK_60B1770[i]) {
			j = i;
		}
	}
	if (UNK_60B1770[j] != 0) {
		return j;
	}
	int8_t i;
	while (true) {
		for (i = 0; UNK_60B1758[i] != 0 && i < UNK_60B181C - 1; i++);
		if (i < UNK_60B181C - 1) {
			break;
		}
		for (i = 0; i < UNK_60B181C - 1; i++) {
			if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
				UNK_60B1770[i] = 1u;
			}
		}
	}
	return i;
}

void UNK_602F2B0(uint8_t arg0) {
	
	if (MidiPtr->UNK_0[arg0] == 0xFFFFu) {
		return;
	}

	uint8_t i = 0u;
	while (true) {
		if (i >= UNK_60B181C - 1) {
			return;
		}
		if (UNK_60B1728[i][1] == UNK_60B1865 && UNK_60B1728[i][0] == arg0) {
			break;
		}
		i += 1;
	}
	UNK_60B1869 = i;
	UNK_602F026(0u);
	for (uint8_t i = 0u; i < UNK_60B181C - 1; i++) {
		if (UNK_60B1758[i] != 0) {
			UNK_60B1758[i]++;
		}
		if (UNK_60B1770[i] != 0) {
			UNK_60B1770[i]++;
		}
	}
}

uint8_t UNK_602F386() {
    int8_t j = UNK_60B181C - 1;
	for (int8_t i = (int8_t)UNK_60B181C; i < UNK_60B181C; i++) {
        if (UNK_60B1770[i] != 0 && UNK_60B1770[j] < UNK_60B1770[i]) {
            j = i;
        }
    }
    if (UNK_60B1770[j] != 0) {
        return j;
    }
	int8_t i;
    while (true) {
		for (i = (int8_t)(UNK_60B181C - 1); UNK_60B1758[i] != 0 && i < UNK_60B181C; i++);
		if (i < UNK_60B181C) {
			break;
		}
		for (i = UNK_60B181C - 1; i < UNK_60B181C; i++) {
            if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
                UNK_60B1770[i] = 1u;
            }
        }
    }
	return i;
}

void UNK_602F46C() {
	// Empty.
}

void UNK_602F480() {
	if (MidiPtr->UNK_100[UNK_60B1865].UNK_0 == 0xFFFFu) {
		return;
	}
	UNK_60B1869 = UNK_602F386();
	UNK_60B1758[UNK_60B1869] = 3u;
	UNK_60B1728[UNK_60B1869][1] = UNK_60B1865;
	UNK_60B1728[UNK_60B1869][0] = 0xFFu;
	UNK_60B1862 = MidiPtr->UNK_100[UNK_60B1865].UNK_0;
	UNK_60B1865 = MidiPtr->UNK_100[UNK_60B1865].UNK_2[0];
	UNK_60B1868 = 0x20u;
	UNK_602ED40();
	for (int8_t i = UNK_60B181C - 1; i < UNK_60B181C; i++) {
		if (UNK_60B1758[i] != 0 && --UNK_60B1758[i] == 0) {
			UNK_60B1770[i] = 1;
		}
	}
}

void UNK_602F57E() {
	uint8_t i;
	
	if (MidiPtr->UNK_100[UNK_60B1865].UNK_0 == 0xFFFFu) {
		return;
	}
	i = UNK_60B181C - 1;
	while (true) {
		if (i >= UNK_60B181C) {
			return;
		}
		if (UNK_60B1728[i][1] == UNK_60B1865) {
			break;
		}
		i += 1;
	}
	UNK_60B1869 = i;
	UNK_602F026(0u);
	for (int8_t i = UNK_60B181C - 1; i < UNK_60B181C; i++) {
		if (UNK_60B1758[i] != 0) {
			UNK_60B1758[i]++;
		}
		if (UNK_60B1770[i] != 0) {
			UNK_60B1770[i]++;
		}
	}
}
