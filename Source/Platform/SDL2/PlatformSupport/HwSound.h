#pragma once

// TODO: Move things here that aren't platform-specific into "Sound/HwSound.h", put the platform-specific parts into "PlatformSupport/PlatformSound.h", and include PlatformSound.h in HwSound.h.

#include <stdint.h>

// The number of wave table sound channels.
#define SND_NUMPCMCHANNELS 24

typedef enum SoundRegisterRead {
	SNDREGRD_STATUS,
	SNDREGRD_FM1 = 1,
	SNDREGRD_FM3 = 3,
	SNDREGRD_PCM = 5
} SoundRegisterRead;

typedef enum SoundRegisterWrite {
	SNDREGWR_SELECTFMREGA = 0,
	SNDREGWR_WRITEFMREGA  = 1,

	SNDREGWR_SELECTFMREGB = 2,
	SNDREGWR_WRITEFMREGB  = 3,

	SNDREGWR_SELECTPCMREG = 4,
	SNDREGWR_WRITEPCMREG  = 5
} SoundRegisterWrite;

// After init, a sound device ID is returned on the first status register read.
typedef enum SoundDeviceId {
	SNDDEVID_OPL2 = 6,
	SNDDEVID_OPL3 = 0,
	SNDDEVID_OPL4 = 2
} SoundDeviceId;

// Flags indicating sound hardware status. Status flags are returned from status register reads.
typedef enum SoundStatusFlag {
	SNDSTATUS_IRQ     = 1 << 7,
	SNDSTATUS_FT1     = 1 << 6,
	SNDSTATUS_FT2     = 1 << 5,
	// Bits 4 to 2 are unused.
	SNDSTATUS_LOADING = 1 << 1,
	SNDSTATUS_BUSY    = 1 << 0
} SoundStatusFlag;

typedef enum SoundRegisterPcm {
	SNDREGPCM_LSITEST1   = 0x00,
	SNDREGPCM_LSITEST2   = 0x01,

	SNDREGPCM_MEMMODE    = 0x02,
	SNDREGPCM_MEMTYPE    = 0x02,
	SNDREGPCM_WAVETBLHDR = 0x02,
	SNDREGPCM_DEVICEID   = 0x02, // When read, 0x20 is returned for an OPL4. The value returned does not correspond to SoundDeviceId values.

	SNDREGPCM_MEMADDR1   = 0x03,
	SNDREGPCM_MEMADDR2   = 0x04,
	SNDREGPCM_MEMADDR3   = 0x05,
	SNDREGPCM_MEMDATA    = 0x06,
	SNDREGPCM_UNUSED7    = 0x07,

	// For each of these, there are 24 registers, one for each wave table channel, starting with the register numbers specified here.
	SNDREGPCM_CHANNELREGS1     = 0x08,
	SNDREGPCM_WAVETBLNUMN0TON7 = 0x08,

	SNDREGPCM_CHANNELREGS2     = 0x20,
	SNDREGPCM_WAVETBLNUMN8     = 0x20,
	SNDREGPCM_FNUMF0TOF6       = 0x20,

	SNDREGPCM_CHANNELREGS3     = 0x38,
	SNDREGPCM_FNUMF7TOF9       = 0x38,
	SNDREGPCM_OCTAVE           = 0x38,
	SNDREGPCM_REVERB           = 0x38,

	SNDREGPCM_CHANNELREGS4     = 0x50,
	SNDREGPCM_TOTALLEVEL       = 0x50,
	SNDREGPCM_LEVELDIRECT      = 0x50,

	SNDREGPCM_CHANNELREGS5     = 0x68,
	SNDREGPCM_KEYSETTING       = 0x68,
	SNDREGPCM_DAMP             = 0x68,
	SNDREGPCM_LFORESET         = 0x68,
	SNDREGPCM_OUTPUTCHANNEL    = 0x68,
	SNDREGPCM_PANPOT           = 0x68,

	SNDREGPCM_CHANNELREGS6     = 0x80,
	SNDREGPCM_LFOFREQ          = 0x80,
	SNDREGPCM_VIBRATODEPTH     = 0x80,

	SNDREGPCM_CHANNELREGS7     = 0x98,
	SNDREGPCM_ATTACKRATE       = 0x98,
	SNDREGPCM_DECAY1RATE       = 0x98,

	SNDREGPCM_CHANNELREGS8     = 0xB0,
	SNDREGPCM_DECAYLEVEL       = 0xB0,
	SNDREGPCM_DECAY2RATE       = 0xB0,

	SNDREGPCM_CHANNELREGS9     = 0xC8,
	SNDREGPCM_RELEASERATE      = 0xC8,
	SNDREGPCM_RATECORRECTION   = 0xC8,

	SNDREGPCM_TREMOLODEPTH     = 0xE0,

	// These control the stereo mixing of the FM and PCM audio.
	SNDREGPCM_MIXCONTROLFM  = 0xF8,
	SNDREGPCM_MIXCONTROLPCM = 0xF9
} SoundRegisterPcm;

typedef enum SoundRegisterFm {
	SNDREGFM_LSITEST1      = 0x00,
	SNDREGFM_LSITEST2      = 0x01,
	SNDREGFM_TIMER1        = 0x02,
	SNDREGFM_TIMER2        = 0x03,
	SNDREGFM_TIMERCTRL     = 0x04,
	SNDREGFM_FLAGCTRL      = 0x04,
	SNDREGFM_OPMODE        = 0x04,
	SNDREGFM_EXPANSION     = 0x05, // This must be set to a value corresponding to the OPL feature set you want to use before doing anything with the sound hardware.
	SNDREGFM_KEYBOARDSPLIT = 0x08,

	SNDREGFM_TREMOLOEFFECT = 0x20,

	SNDREGFM_VIBRATOEFFECT = 0x20,

	SNDREGFM_PERCUSSIVESND = 0x20,
	SNDREGFM_RATEKEYSCALE  = 0x20,
	SNDREGFM_MULTIPLIER    = 0x20,

	SNDREGFM_TOTALLEVEL    = 0x40,
	SNDREGFM_LEVELKEYSCALE = 0x40,

	SNDREGFM_ATTACKRATE    = 0x60,
	SNDREGFM_DECAYRATE     = 0x60,

	SNDREGFM_RELEASERATE   = 0x80,
	SNDREGFM_SUSTAINLEVEL  = 0x80,

	SNDREGFM_FNUMF0TOF7    = 0xA0,
	SNDREGFM_FNUMF9TOF8    = 0xB0,

	SNDREGFM_BLOCK         = 0xB0,
	SNDREGFM_KEYSETTING    = 0xB0,

	SNDREGFM_EFFECTRHYTHM  = 0xBD,

	SNDREGFM_ALGORITHM     = 0xC0,
	SNDREGFM_FEEDBACK      = 0xC0,
	SNDREGFM_OUTPUTCHANNEL = 0xC0,

	SNDREGFM_WAVE          = 0xE0
} SoundRegisterFm;

#define SNDFM_FLAGCTRL(rst, mt1, mt2) ((!!(rst) << 7) | (!!(mt1) << 6) | (!!(mt2) << 5))
#define SNDFM_KEYBOARDSPLIT(nst) (!!(nst) << 6)
#define SNDFM_TOTALLEVEL(level) ((level) & 0x3Fu)

typedef enum SoundExpansionFlag {
	SNDEXP_NEW2 = 1 << 1,
	SNDEXP_NEW  = 1 << 0,

	SNDEXP_OPL3 = SNDEXP_NEW,
	SNDEXP_OPL4 = SNDEXP_OPL3 | SNDEXP_NEW2
} SoundExpansionFlag;

void SoundControlStartInit();
// Does the first part of sound hardware init. Call first, before any further init. Be sure to set the expansion FM register immediately after calling this.
#define SNDCTRL_STARTINIT() SoundControlStartInit()

#define SNDCTRL_STARTWAIT()

void SoundControlStatusWait(const SoundStatusFlag flags);
// Wait until the status flags are clear.
#define SNDCTRL_STATUSWAIT(flags) SoundControlStatusWait((SoundStatusFlag)(flags))

uint8_t SoundControlRead(const uint8_t i);
#define SNDCTRL_READ(i) SoundControlRead((uint8_t)(i))

void SoundControlWrite(const uint8_t i, const uint8_t value);
#define SNDCTRL_WRITE(i, value) SoundControlWrite((uint8_t)(i), (uint8_t)(value))
