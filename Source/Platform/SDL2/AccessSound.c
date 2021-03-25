#include "Platform/SDL2/AccessSound.h"
#include "Platform/Util/AccessData.h"
#include "Main/Frame.h"
#include "HwSound.h"
#include <string.h>

SDL_AudioDeviceID AudioDevice = 0;
SDL_mutex* AudioMutex = NULL;

static void UpdateStream(void* userdata, Uint8* stream, int size) {
	SDL_LockMutex(AudioMutex);

	NumVblanks = 1u;
	UpdateSound();

	memset(stream, 0, size);
	Sint16* stream16 = (Sint16*)stream;
	int length = size / sizeof(Sint16);

	for (size_t i = 0u; i < lengthof(Sound.pcmChannels); i++) {
		PcmChannelData* const channel = &Sound.pcmChannels[i];
		if (channel->playing) {
			switch (channel->bits) {
			case WAVEBITLEN_8: {
				for (int i = 0; i < length; i += 2) {
					if (channel->samplePos >= channel->endAddress) {
						channel->samplePos = channel->samplePos - channel->endAddress + channel->loopAddress;
					}

					const Sint16 sample = (Sint16)SoundRomData[channel->startAddress + (channel->samplePos >> 16)] << 8;
					stream16[i + 0] += (sample * VolumeTable[channel->totalLevel +  PanLeftTable[channel->panpot] + (channel->envelopeVolume >> 23)]) >> 17;
					stream16[i + 1] += (sample * VolumeTable[channel->totalLevel + PanRightTable[channel->panpot] + (channel->envelopeVolume >> 23)]) >> 17;

					channel->samplePos += channel->sampleStride;

					channel->envelopeVolume += channel->envelopeVolumeStride;
					if ((int32_t)(channel->envelopeVolume - channel->envelopeVolumeLimit) >= 0) {
						channel->envelopeStride++;
						PcmChannelComputeEnvelope(channel);
					}
					else if (channel->preverb && !channel->envelopePreverb && channel->envelopeStride && channel->envelopeVolume > ((6 * 8) << 23)) {
						PcmChannelComputeEnvelope(channel);
					}
				}
				break;
			}

			case WAVEBITLEN_12: {
				for (int i = 0; i < length; i += 2) {
					if (channel->samplePos >= channel->endAddress) {
						channel->samplePos = channel->samplePos - channel->endAddress + channel->loopAddress;
					}

					const Sint16 sample =
						((Sint16)SoundRomData[channel->startAddress + (channel->samplePos >> 17) * 3u + (((channel->samplePos >> 16) & 1u) << 1u)] << 8u) |
						(((Sint16)SoundRomData[channel->startAddress + (channel->samplePos >> 17) * 3u + 1u] << (((channel->samplePos >> 16) & 1u) << 2u)) & 0xF0u);
					stream16[i + 0] += (sample * VolumeTable[channel->totalLevel +  PanLeftTable[channel->panpot] + (channel->envelopeVolume >> 23)]) >> 17;
					stream16[i + 1] += (sample * VolumeTable[channel->totalLevel + PanRightTable[channel->panpot] + (channel->envelopeVolume >> 23)]) >> 17;

					channel->samplePos += channel->sampleStride;

					channel->envelopeVolume += channel->envelopeVolumeStride;
					if ((int32_t)(channel->envelopeVolume - channel->envelopeVolumeLimit) >= 0) {
						channel->envelopeStride++;
						PcmChannelComputeEnvelope(channel);
					}
					else if (channel->preverb && !channel->envelopePreverb && channel->envelopeStride && channel->envelopeVolume > ((6 * 8) << 23)) {
						PcmChannelComputeEnvelope(channel);
					}
				}
				break;
			}

			case WAVEBITLEN_16: {
				for (int i = 0; i < length; i += 2) {
					if (channel->samplePos >= channel->endAddress) {
						channel->samplePos = channel->samplePos - channel->endAddress + channel->loopAddress;
					}

					const Sint16 sample =
						((Sint16)SoundRomData[channel->startAddress + (channel->samplePos >> 16) * 2 + 0] << 8) |
						((Sint16)SoundRomData[channel->startAddress + (channel->samplePos >> 16) * 2 + 1] << 0);
					stream16[i + 0] += (sample * VolumeTable[channel->totalLevel +  PanLeftTable[channel->panpot] + (channel->envelopeVolume >> 23)]) >> 17;
					stream16[i + 1] += (sample * VolumeTable[channel->totalLevel + PanRightTable[channel->panpot] + (channel->envelopeVolume >> 23)]) >> 17;

					channel->samplePos += channel->sampleStride;

					channel->envelopeVolume += channel->envelopeVolumeStride;
					if ((int32_t)(channel->envelopeVolume - channel->envelopeVolumeLimit) >= 0) {
						channel->envelopeStride++;
						PcmChannelComputeEnvelope(channel);
					}
					else if (channel->preverb && !channel->envelopePreverb && channel->envelopeStride && channel->envelopeVolume > ((6 * 8) << 23)) {
						PcmChannelComputeEnvelope(channel);
					}
				}
				break;
			}

			default:
				for (int i = 0; i < length; i += 2) {
					if (channel->samplePos >= channel->endAddress) {
						channel->samplePos = channel->samplePos - channel->endAddress + channel->loopAddress;
					}

					stream16[i + 0] += 0;
					stream16[i + 1] += 0;

					channel->samplePos += channel->sampleStride;

					channel->envelopeVolume += channel->envelopeVolumeStride;
					if ((int32_t)(channel->envelopeVolume - channel->envelopeVolumeLimit) >= 0) {
						channel->envelopeStride++;
						PcmChannelComputeEnvelope(channel);
					}
					else if (channel->preverb && !channel->envelopePreverb && channel->envelopeStride && channel->envelopeVolume > ((6 * 8) << 23)) {
						PcmChannelComputeEnvelope(channel);
					}
				}
				break;
			}
		}
	}

	SDL_UnlockMutex(AudioMutex);
}

bool OpenSound() {
	if (!(AudioMutex = SDL_CreateMutex())) {
		fprintf(stderr, "Failed creating audio mutex: %s\n", SDL_GetError());
		return false;
	}

	SDL_AudioSpec spec;
	spec.freq = 37286;
	spec.format = AUDIO_S16SYS;   /**< Audio data format */
	spec.channels = 2u;           /**< Number of channels: 1 mono, 2 stereo */
	spec.silence = 0u;            /**< Audio buffer silence value (calculated) */
	// The number of samples set here is approximately how many samples a single video frame takes, which is how much audio should be generated after a call of UpdateSound.
	spec.samples = 605u;          /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
	spec.padding = 0u;            /**< Necessary for some compile environments */
	spec.size = 0u;	              /**< Audio buffer size in bytes (calculated) */
	spec.callback = UpdateStream; /**< Callback that feeds the audio device (NULL to use SDL_QueueAudio()). */
	spec.userdata = NULL;         /**< Userdata passed to callback (ignored for NULL callbacks). */

	AudioDevice = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
	if (AudioDevice == 0) {
		fprintf(stderr, "Failed opening audio device: %s\n", SDL_GetError());
		return false;
	}
	SDL_PauseAudioDevice(AudioDevice, 0);

	return true;
}

void CloseSound() {
	if (AudioDevice) {
		SDL_CloseAudioDevice(AudioDevice);
		AudioDevice = 0;
	}

	if (AudioMutex) {
		SDL_DestroyMutex(AudioMutex);
		AudioMutex = NULL;
	}
}
