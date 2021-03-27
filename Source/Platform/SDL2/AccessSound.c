#include "Platform/SDL2/AccessSound.h"
#include "Platform/Util/AccessData.h"
#include "Platform/Util/AccessConfig.h"
#include "Main/Frame.h"
#include "HwSound.h"
#include <string.h>

SDL_AudioDeviceID AudioDevice = 0;
SDL_mutex* AudioMutex = NULL;

#define STREAMSAMPLE_LEFT() ((((sample * VolumeTable[channel->totalLevel + PanLeftTable[channel->panpot] + (channel->envelopeVolume >> 23)]) >> 17) * mixLeft) >> 16)
#define STREAMSAMPLE_RIGHT() ((((sample * VolumeTable[channel->totalLevel + PanRightTable[channel->panpot] + (channel->envelopeVolume >> 23)]) >> 17) * mixRight) >> 16)
#define STREAMSAMPLE_BOTH() (STREAMSAMPLE_LEFT() + STREAMSAMPLE_RIGHT())

#define SETCHANNELSTREAM(channel, length, sampleGet, streamSet) \
do { \
	for (int i = 0; i < (length); i++) { \
		if ((channel)->samplePos >= (channel)->endAddress) { \
			(channel)->samplePos = (channel)->samplePos - (channel)->endAddress + (channel)->loopAddress; \
		} \
		sampleGet((channel)); \
		do streamSet while (0); \
		(channel)->samplePos += (channel)->sampleStride; \
		(channel)->envelopeVolume += (channel)->envelopeVolumeStride; \
		if ((int32_t)((channel)->envelopeVolume - (channel)->envelopeVolumeLimit) >= 0) { \
			(channel)->envelopeStride++; \
			PcmChannelComputeEnvelope((channel)); \
		} \
		else if ((channel)->preverb && !(channel)->envelopePreverb && (channel)->envelopeStride && (channel)->envelopeVolume > ((6 * 8) << 23)) { \
			PcmChannelComputeEnvelope((channel)); \
		} \
	} \
} while (0)

#define SAMPLE8(channel) \
const Sint16 sample = (Sint16)SoundRomData[(channel)->startAddress + ((channel)->samplePos >> 16)] << 8

#define SAMPLE12(channel) \
const Sint16 sample = \
	((Sint16)SoundRomData[(channel)->startAddress + ((channel)->samplePos >> 17) * 3u + ((((channel)->samplePos >> 16) & 1u) << 1u)] << 8u) | \
	(((Sint16)SoundRomData[(channel)->startAddress + ((channel)->samplePos >> 17) * 3u + 1u] << ((((channel)->samplePos >> 16) & 1u) << 2u)) & 0xF0u)

#define SAMPLE16(channel) \
const Sint16 sample = \
	((Sint16)SoundRomData[(channel)->startAddress + ((channel)->samplePos >> 16) * 2 + 0] << 8) | \
	((Sint16)SoundRomData[(channel)->startAddress + ((channel)->samplePos >> 16) * 2 + 1] << 0)

#define UPDATESTREAM(userdata, stream, size, numSpeakers, setStream) \
SDL_LockMutex(AudioMutex); \
NumVblanks = 1u; \
UpdateSound(); \
memset((stream), 0, (size)); \
Sint16* stream16 = (Sint16*)(stream); \
int length = (size) / sizeof(Sint16) / (numSpeakers); \
const int32_t \
	mixLeft = MixLevelTable[Sound.pcmLeft], \
	mixRight = MixLevelTable[Sound.pcmRight]; \
for (size_t i = 0u; i < lengthof(Sound.pcmChannels); i++) { \
	PcmChannelData* const channel = &Sound.pcmChannels[i]; \
	if (channel->playing) { \
		switch (channel->bits) { \
		case WAVEBITLEN_8: { \
			SETCHANNELSTREAM(channel, length, SAMPLE8, setStream); \
			break; \
		} \
		case WAVEBITLEN_12: { \
			SETCHANNELSTREAM(channel, length, SAMPLE12, setStream); \
			break; \
		} \
		case WAVEBITLEN_16: { \
			SETCHANNELSTREAM(channel, length, SAMPLE16, setStream); \
			break; \
		} \
		default: \
			SETCHANNELSTREAM(channel, length, , {}); \
			break; \
		} \
	} \
} \
SDL_UnlockMutex(AudioMutex);


static void UpdateStreamMonoBoth(void* userdata, Uint8 * stream, int size) {
	UPDATESTREAM(userdata, stream, size, 1u,
		{
			stream16[i] += STREAMSAMPLE_BOTH();
		}
	);
}
static void UpdateStreamMonoLeft(void* userdata, Uint8* stream, int size) {
	UPDATESTREAM(userdata, stream, size, 1u,
		{
			stream16[i] += STREAMSAMPLE_LEFT();
		}
	);
}
static void UpdateStreamMonoRight(void* userdata, Uint8* stream, int size) {
	UPDATESTREAM(userdata, stream, size, 1u,
		{
			stream16[i] += STREAMSAMPLE_RIGHT();
		}
	);
}


static void UpdateStreamStereoBoth(void* userdata, Uint8* stream, int size) {
	UPDATESTREAM(userdata, stream, size, 2u,
		{
			stream16[i * 2 + 0] += STREAMSAMPLE_LEFT();
			stream16[i * 2 + 1] += STREAMSAMPLE_RIGHT();
		}
	);
}
static void UpdateStreamStereoLeft(void* userdata, Uint8* stream, int size) {
	UPDATESTREAM(userdata, stream, size, 2u,
		{
			stream16[i * 2 + 0] += STREAMSAMPLE_LEFT();
			stream16[i * 2 + 1] = 0;
		}
	);
}
static void UpdateStreamStereoRight(void* userdata, Uint8* stream, int size) {
	UPDATESTREAM(userdata, stream, size, 2u,
		{
			stream16[i * 2 + 0] = 0;
			stream16[i * 2 + 1] += STREAMSAMPLE_RIGHT();
		}
	);
}


bool OpenSound() {
	if (!(AudioMutex = SDL_CreateMutex())) {
		fprintf(stderr, "Failed creating audio mutex: %s\n", SDL_GetError());
		return false;
	}

	if (AudioMuted) {
		AudioDevice = 0;
	}
	else {
		SDL_AudioSpec spec;
		spec.freq = 37286;
		spec.format = AUDIO_S16SYS;
		// The number of samples set here is approximately how many samples a single video frame takes, which is how much audio should be generated after a call of UpdateSound.
		spec.samples = 605u;
		spec.padding = 0u;
		spec.size = 0u;
		switch (AudioMixing) {
		case MIXING_MONO:
			spec.channels = 1u;
			switch (AudioSpeakers) {
			case SPEAKERS_BOTH: spec.callback = UpdateStreamMonoBoth; break;
			case SPEAKERS_LEFT: spec.callback = UpdateStreamMonoLeft; break;
			case SPEAKERS_RIGHT: spec.callback = UpdateStreamMonoRight; break;
			}
			break;
		case MIXING_STEREO:
			spec.channels = 2u;
			switch (AudioSpeakers) {
			case SPEAKERS_BOTH: spec.callback = UpdateStreamStereoBoth; break;
			case SPEAKERS_LEFT: spec.callback = UpdateStreamStereoLeft; break;
			case SPEAKERS_RIGHT: spec.callback = UpdateStreamStereoRight; break;
			}
			break;
		}
		spec.userdata = NULL;

		AudioDevice = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
		if (AudioDevice == 0) {
			fprintf(stderr, "Failed opening audio device: %s\n", SDL_GetError());
			return false;
		}
		SDL_PauseAudioDevice(AudioDevice, 0);
	}

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
