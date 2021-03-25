#include "Platform/Util/AccessData.h"
#include "Platform/Util/AccessPaths.h"
#include "Platform/SDL2/AccessSound.h"
#include "Platform/SDL2/PlatformSupport/PlatformUpdate.h"
#include "Main/Frame.h"
#include "Sound/Sound.h"
#include "HwSound.h"
#include "SDL.h"
#include "physfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>

bool PlayWaves(SDL_AudioDeviceID deviceId, SDL_AudioSpec* spec, const uint8_t* const soundRom) {
	for (size_t waveNum = 0u; waveNum < 384u; waveNum++) {
		const uint8_t* const headerData = &soundRom[waveNum * 12u];
		WaveTableHeader header;
		header.bitLength = (headerData[0] >> 6u) & 3u;

		header.startAddress = ((headerData[0] & 0x3Fu) << 16u) | (headerData[1] << 8u) | headerData[2];
		header.loopAddress = (headerData[3] << 8u) | (headerData[4] << 0u);
		header.endAddress = (((headerData[5] << 8u) | (headerData[6] << 0u)) - 1) ^ 0xFFFFu;

		if (header.endAddress == 0u) {
			continue;
		}

		header.lfo = (headerData[7] >> 3u) & 7u;
		header.vib = headerData[7] & 7u;
		header.ar = (headerData[8] >> 4) & 0xFu;
		header.d1r = headerData[8] & 0xFu;
		header.dl = (headerData[9] >> 4) & 0xFu;
		header.d2r = headerData[9] & 0xFu;
		header.rateCorrection = (headerData[10] >> 4) & 0xFu;
		header.rr = headerData[10] & 0xFu;
		header.am = headerData[11] & 7u;

		printf("wave number: %zu\n", waveNum);
		printf("bit length: ");
		switch (header.bitLength) {
		case WAVEBITLEN_8: printf("8 bits\n"); break;
		case WAVEBITLEN_12: printf("12 bits\n"); break;
		case WAVEBITLEN_16: printf("16 bits\n"); break;
		default: printf("invalid\n"); break;
		}

		printf("start address: 0x%08"PRIX32"\n", header.startAddress);
		printf("loop address: 0x%04"PRIX16"\n", header.loopAddress);
		printf("end address: 0x%04"PRIX16"\n", header.endAddress);

		printf("LFO: %"PRIu8"\n", header.lfo);
		printf("VIB: %"PRIu8"\n", header.vib);
		printf("AR: %"PRIu8"\n", header.ar);
		printf("D1R: %"PRIu8"\n", header.d1r);
		printf("DL: %"PRIu8"\n", header.dl);
		printf("D2R: %"PRIu8"\n", header.d2r);
		printf("rate correction: %"PRIu8"\n", header.rateCorrection);
		printf("RR: %"PRIu8"\n", header.rr);
		printf("AM: %"PRIu8"\n", header.am);

		printf("\n");

		int16_t* const buffer = malloc(sizeof(int16_t) * header.endAddress);
		assert(buffer != NULL);
		switch (header.bitLength) {
		case WAVEBITLEN_8:
			for (size_t i = 0u; i < header.endAddress; i++) {
				buffer[i] = soundRom[header.startAddress + i] << 8;
			}
			break;

		case WAVEBITLEN_12:
			for (size_t i = 0u; i < header.endAddress; i++) {
				buffer[i] =
					(soundRom[header.startAddress + (i >> 1u) * 3u + ((i & 1u) << 1u)] << 8u) |
					((soundRom[header.startAddress + (i >> 1u) * 3u + 1u] << ((i & 1u) << 2u)) & 0xF0u);
			}
			break;

		case WAVEBITLEN_16:
			for (size_t i = 0u; i < header.endAddress; i++) {
				buffer[i] =
					(soundRom[header.startAddress + (i << 1u) + 0u] << 8u) |
					(soundRom[header.startAddress + (i << 1u) + 1u] << 0u);
			}
			break;

		default:
			memset(buffer, 0, sizeof(int16_t) * header.endAddress);
			break;
		}

		if (SDL_QueueAudio(deviceId, buffer, header.endAddress * sizeof(int16_t)) < 0) {
			fprintf(stderr, "Failed to queue audio: %s\n", SDL_GetError());
			SDL_CloseAudioDevice(deviceId);
			free(buffer);
			SDL_Quit();
			CloseSound();
			return false;
		}
		free(buffer);
		SDL_Delay((Uint32)(1000.0 * ((double)(header.endAddress * sizeof(int16_t)) / spec->channels / spec->freq / (SDL_AUDIO_BITSIZE(spec->format) / 8))));
	}
	return true;
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_AUDIO);

	if (!OpenPaths(argv[0])) {
		return EXIT_FAILURE;
	}

	if (!MountRoms()) {
		return EXIT_FAILURE;
	}

	const uint8_t* const programData = OpenProgramData();
	if (!programData) {
		return false;
	}

	if (!OpenSoundData(programData)) {
		CloseProgramData(programData);
		return EXIT_FAILURE;
	}

	CloseProgramData(programData);

#if 1
	SDL_AudioSpec spec;
	spec.freq = 44100 / 2;
	spec.format = AUDIO_S16SYS; /**< Audio data format */
	spec.channels = 1u;         /**< Number of channels: 1 mono, 2 stereo */
	spec.silence = 0u;          /**< Audio buffer silence value (calculated) */
	spec.samples = 0u;          /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
	spec.padding = 0u;          /**< Necessary for some compile environments */
	spec.size = 0u;	            /**< Audio buffer size in bytes (calculated) */
	spec.callback = NULL;       /**< Callback that feeds the audio device (NULL to use SDL_QueueAudio()). */
	spec.userdata = NULL;       /**< Userdata passed to callback (ignored for NULL callbacks). */

	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
	if (deviceId == 0) {
		fprintf(stderr, "Failed opening audio device: %s\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}
	SDL_PauseAudioDevice(deviceId, 0);
	if (!PlayWaves(deviceId, &spec, SoundRomData)) {
		return EXIT_FAILURE;
	}

	SDL_CloseAudioDevice(deviceId);
#else
	SoundStart();
	SoundReset();
	InitSound();
	if (!OpenSound()) {
		return EXIT_FAILURE;
	}

	SDL_LockMutex(AudioMutex);

	StopMusic();
	DisableQuiet();
	UNK_602EB4C();
	UNK_602EC5C();
	UNK_602E6B8();
	FadeSoundEffects();
	StopMusic();
	SetPcmVolumeRight(5);

	uint32_t frames = 0u;
	const Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	const Uint64 gameFrameDuration = (Uint64)(FRAME_DURATION * performanceFrequency);
	PlatformCurrentTime = SDL_GetPerformanceCounter();
	PlatformTimeAccumulator = 0u;

	PlayMusic(MUSIC_SELECTMODE);
	SDL_UnlockMutex(AudioMutex);

	printf("Playing sound...\n");
	while (frames < TIME(2, 0, 0)) {
		while (PlatformTimeAccumulator < gameFrameDuration) {
			SDL_Delay(1u);

			Uint64 newTime = SDL_GetPerformanceCounter();
			Uint64 delay = newTime - PlatformCurrentTime;
			if (delay > performanceFrequency / 4u) {
				delay = performanceFrequency / 4u;
			}
			PlatformCurrentTime = newTime;

			PlatformTimeAccumulator += delay;
		}
		while (PlatformTimeAccumulator > gameFrameDuration) {
			PlatformTimeAccumulator -= gameFrameDuration;
			frames++;
		}
	}

	printf("Shutting down.\n");
	CloseSound();
#endif

	SDL_Quit();
	CloseSoundData();
	ClosePaths();
	return EXIT_SUCCESS;
}
