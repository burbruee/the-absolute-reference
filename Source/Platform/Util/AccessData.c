#include "Platform/Util/AccessData.h"
#include "Main/AttractLoop.h"
#include "Main/GameLoop.h"
#include "Main/DemoReplayInput.h"
#include "Video/Pal.h"
#include "Sound/Sound.h"
#include "physfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

ROMDATA Color Pal1[NUMPALCOLORS_4BPP];
ROMDATA Color PalSmallText[NUMPALCOLORS_4BPP];

// NOTE: For constant data loaded into RAM, take the RAM address and subtract 0x5FFF860 to get the address in program ROM.

uint8_t* TileData;
static const char* const TileRomFileNames[NUMTILEROMS] = {
    "roms/tgm2/81ts_3l.u6",
    "roms/tgm2/82ts_3h.u14",
    "roms/tgm2/83ts_4l.u7",
    "roms/tgm2/84ts_4h.u15",
    "roms/tgm2/85ts_5l.u8",
    "roms/tgm2/86ts_5h.u16",
    "roms/tgm2/87ts_6l.u1",
    "roms/tgm2/88ts_6h.u2",
    "roms/tgm2/89ts_7l.u19",
    "roms/tgm2/90ts_7h.u20",
    "roms/tgm2/91ts_8l.u28",
    "roms/tgm2/92ts_8h.u29",
    "roms/tgm2/93ts_9l.u41",
    "roms/tgm2/94ts_9h.u42",
    "roms/tgm2/95ts_10l.u58",
    "roms/tgm2/96ts_10h.u59"
};

#define PROGRAMROM_SIZE 0x80000
#define NUMPROGRAMROMS 2

static const char* const ProgramRomFileNames[NUMPROGRAMROMS] = {
	"roms/tgm2p/1b.u22",
	"roms/tgm2p/2b.u21"
};

#define ROMOFFSET_DEMOREPLAYINPUTTWIN    0x33F87u
#define ROMOFFSET_DEMOREPLAYINPUTDOUBLES 0x3495Fu
#define ROMOFFSET_DEMOREPLAYINPUTVERSUS  0x35337u

#define ROMCOLOR(colorData, offset) COLOR((colorData)[(offset) + 0], (colorData)[(offset) + 1], (colorData)[(offset) + 2], (colorData)[(offset) + 3])
#define ROMOFFSET_PAL1             0x3141Cu
#define ROMOFFSET_PALSMALLTEXT     0x3145Cu
#define ROMOFFSET_UNK_6032884      0x33024u
#define ROMOFFSET_UNK_60328C4      0x33064u
#define ROMOFFSET_PALCYCLETEXTPAL0 0x33EE8u
#define ROMOFFSET_UNK_6033790      0x33F30u

#define ROMOFFSET_PAL 0x60088u

// By matching the offset array's order to the original's pointer array, a simple loop can be used for initialization. -Brandon McGriff
#define ROMOFFSET_BGMAPSECTION2 0x68B8Cu
#define ROMOFFSET_BGMAPSECTION5 0x72194u
#define ROMOFFSET_BGMAPSECTION8 0x76C9Cu
#define ROMOFFSET_BGMAPSECTION3 0x7B7A4u
#define ROMOFFSET_BGMAPSECTION1 0x802ACu
#define ROMOFFSET_BGMAPSECTION0 0x84DB4u
#define ROMOFFSET_BGMAPSECTION4 0x898BCu
#define ROMOFFSET_BGMAPSECTION6 0x92EC4u
#define ROMOFFSET_BGMAPSECTION7 0x979CCu
#define ROMOFFSET_BGMAPSECTION9 0x9C4D4u
#define ROMOFFSET_BGMAPVERSUS   0xA0FDCu
static const size_t BgMapRomOffsets[] = {
	ROMOFFSET_BGMAPSECTION2,
	ROMOFFSET_BGMAPSECTION5,
	ROMOFFSET_BGMAPSECTION8,
	ROMOFFSET_BGMAPSECTION3,
	ROMOFFSET_BGMAPSECTION1,
	ROMOFFSET_BGMAPSECTION0,
	ROMOFFSET_BGMAPSECTION4,
	ROMOFFSET_BGMAPSECTION6,
	ROMOFFSET_BGMAPSECTION7,
	ROMOFFSET_BGMAPSECTION9,
	ROMOFFSET_BGMAPVERSUS
};

#define ROMOFFSET_OBJECTDATA 0xA5D08u

uint8_t* SoundRomData;
static const char* SoundRomFileName = "roms/tgm2/97ts_snd.u52";

static uint8_t* MidiNotes;
#define ROMOFFSET_MIDI 0x40088u
#define ROMOFFSET_MIDINOTES 0x41788u
#define MIDINOTES_LENGTH 0x12BCAu

static char* locationToNativeFormat(const char* const locationInPortableFormat) {
	assert(locationInPortableFormat);
	if (!locationInPortableFormat) {
		return NULL;
	}

	const char* const sep = PHYSFS_getDirSeparator();
	const size_t sepLen = strlen(sep);

	size_t locationSize = 0u;
	for (const char* c = locationInPortableFormat; *c != '\0'; c++) {
		if (*c != '/') {
			locationSize++;
		}
		else {
			locationSize += sepLen;
		}
	}
	locationSize++;

	char* location = malloc(locationSize);
	assert(location);
	if (!location) {
		return NULL;
	}

	location[0] = '\0';
	char cat[2] = { '\0', '\0' };
	for (const char* c = locationInPortableFormat; *c != '\0'; c++) {
		if (*c != '/') {
			cat[0] = *c;
			strcat(location, cat);
		}
		else {
			strcat(location, sep);
		}
	}
	assert(strlen(location) + 1u == locationSize);

	return location;
}

static bool Mount(const char* locationNoExtension) {
	if (!locationNoExtension) {
		return false;
	}

	const char* const dirSeparator = PHYSFS_getDirSeparator();
	const size_t dirSeparatorLen = strlen(dirSeparator);
	char* location = NULL;
	PHYSFS_Stat stat;
	if (PHYSFS_stat(locationNoExtension, &stat) && stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
		const char* realDir = PHYSFS_getRealDir(locationNoExtension);
		if (!realDir) {
			return false;
		}

		char* locationCat = locationToNativeFormat(locationNoExtension);
		assert(locationCat);
		if (!locationCat) {
			return false;
		}

		const size_t locationSize = (size_t)snprintf(NULL, 0u, "%s%s", realDir, locationCat) + 1u;
		location = malloc(locationSize);
		assert(location);
		if (!location) {
			free(locationCat);
			return false;
		}

		if (snprintf(location, locationSize, "%s%s", realDir, locationCat) != locationSize - 1u) {
			free(locationCat);
			free(location);
			return false;
		}
		free(locationCat);
	}
	else {
		const size_t locationWithExtensionSize = (size_t)snprintf(NULL, 0u, "%s%s", locationNoExtension, ".zip") + 1u;
		char* locationWithExtension = malloc(locationWithExtensionSize);
		if (snprintf(locationWithExtension, locationWithExtensionSize, "%s%s", locationNoExtension, ".zip") != locationWithExtensionSize - 1u) {
			free(locationWithExtension);
			return false;
		}

		if (!PHYSFS_stat(locationWithExtension, &stat) || stat.filetype != PHYSFS_FILETYPE_REGULAR) {
			free(locationWithExtension);
			fprintf(stderr, "Failed finding location of \"%s\" to mount.\n\n", locationNoExtension);
			return false;
		}

		const char* realDir = PHYSFS_getRealDir(locationWithExtension);
		if (!realDir) {
			return false;
		}

		char* locationCat = locationToNativeFormat(locationWithExtension);
		free(locationWithExtension);
		assert(locationCat);
		if (!locationCat) {
			return false;
		}

		const size_t locationSize = (size_t)snprintf(NULL, 0u, "%s%s", realDir, locationCat) + 1u;
		location = malloc(locationSize);
		assert(location);
		if (!location) {
			free(locationCat);
			return false;
		}

		if (snprintf(location, locationSize, "%s%s", realDir, locationCat) != locationSize - 1u) {
			free(locationCat);
			free(location);
			return false;
		}
		free(locationCat);
	}

	if (!PHYSFS_mount(location, locationNoExtension, 1)) {
		fprintf(stderr, "Failed mounting \"%s\" at \"%s\".\n\n", location, locationNoExtension);
		free(location);
		return false;
	}
	printf("Mounted \"%s\" at \"%s\".\n\n", location, locationNoExtension);
	free(location);

	return true;
}

bool MountRoms() {
	if (!Mount("roms/tgm2")) {
		fprintf(stderr, "Failed to mount location \"roms/tgm2\"\n");
		return false;
	}
	printf("Files in mount location \"roms/tgm2\":\n");
	for (char** files = PHYSFS_enumerateFiles("roms/tgm2"); files != NULL && *files != NULL; files++) {
		printf("roms/tgm2/%s\n", *files);
	}
	printf("\n");

	if (!Mount("roms/tgm2p")) {
		fprintf(stderr, "Failed to mount location \"roms/tgm2p\"\n");
		return false;
	}
	printf("Files in mount location \"roms/tgm2p\":\n");
	for (char** files = PHYSFS_enumerateFiles("roms/tgm2p"); files != NULL && *files != NULL; files++) {
		printf("roms/tgm2p/%s\n", *files);
	}
	printf("\n");

	return true;
}

const uint8_t* OpenProgramData() {
	uint8_t* const programData = malloc(PROGRAMROM_SIZE * NUMPROGRAMROMS);
	if (!programData) {
		fprintf(stderr, "Failed allocating program data\n");
		return NULL;
	}
	uint8_t* const programTemp = malloc(PROGRAMROM_SIZE * NUMPROGRAMROMS);
	if (!programTemp) {
		fprintf(stderr, "Failed allocating program data temporary storage\n");
		free(programData);
		return NULL;
	}

	PHYSFS_File* const programHighFile = PHYSFS_openRead(ProgramRomFileNames[1]);
	if (!programHighFile || PHYSFS_readBytes(programHighFile, &programTemp[PROGRAMROM_SIZE * 0], PROGRAMROM_SIZE) != PROGRAMROM_SIZE) {
		fprintf(stderr, "Failed opening program ROM \"%s\"", ProgramRomFileNames[1]);
		if (programHighFile) {
			PHYSFS_close(programHighFile);
		}
		free(programTemp);
		free(programData);
		return NULL;
	}
	PHYSFS_File* const programLowFile = PHYSFS_openRead(ProgramRomFileNames[0]);
	if (!programLowFile || PHYSFS_readBytes(programLowFile, &programTemp[PROGRAMROM_SIZE * 1], PROGRAMROM_SIZE) != PROGRAMROM_SIZE) {
		fprintf(stderr, "Failed opening program ROM \"%s\"", ProgramRomFileNames[0]);
		if (programLowFile) {
			PHYSFS_close(programLowFile);
		}
		if (programHighFile) {
			PHYSFS_close(programHighFile);
		}
		free(programTemp);
		free(programData);
		return NULL;
	}
	PHYSFS_close(programLowFile);
	PHYSFS_close(programHighFile);

	for (size_t j = 0u; j < NUMPROGRAMROMS; j++) {
		uint8_t* programPtr = programData + j * 2;
		uint8_t* programSrc = &programTemp[PROGRAMROM_SIZE * j];
		for (size_t i = 0u; i < PROGRAMROM_SIZE * NUMPROGRAMROMS; i += 4u, programSrc += 2, programPtr += 2) {
			*programPtr++ = programSrc[1];
			*programPtr++ = programSrc[0];
		}
	}
	free(programTemp);

	return programData;
}

void CloseProgramData(const uint8_t* const programData) {
	free((void*)programData);
}

bool OpenTiles() {
	TileData = malloc(TILEDATA_SIZE);
	if (!TileData) {
		fprintf(stderr, "Failed allocating memory for tile data\n");
		return false;
	}
	uint8_t* const tileDataTemp = malloc(TILEDATA_SIZE);
	if (!tileDataTemp) {
		fprintf(stderr, "Failed allocating memory for tile data temporary storage\n");
		return false;
	}
	for (size_t i = 0u; i < NUMTILEROMS; i++) {
		PHYSFS_File* const tileFile = PHYSFS_openRead(TileRomFileNames[i]);
		if (!tileFile || PHYSFS_readBytes(tileFile, &tileDataTemp[i * TILEROM_SIZE], TILEROM_SIZE) != TILEROM_SIZE) {
			if (tileFile) {
				PHYSFS_close(tileFile);
			}
			fprintf(stderr, "Failed opening tile ROM file \"%s\"\n", TileRomFileNames[i]);
			return false;
		}
		else {
			PHYSFS_close(tileFile);
		}
	}
	uint8_t* tilePtr = TileData;
	for (size_t i = 0u; i < NUMTILEROMS; i += 2) {
		for (const uint8_t* tileLow = &tileDataTemp[(i + 0) * TILEROM_SIZE], * tileEnd = tileLow + TILEROM_SIZE; tileLow < tileEnd;) {
			*tilePtr++ = *tileLow++;
			*tilePtr++ = *tileLow++;
			tilePtr += 2;
		}
		tilePtr -= TILEROM_SIZE * 2;
		for (const uint8_t* tileHigh = &tileDataTemp[(i + 1) * TILEROM_SIZE], * tileEnd = tileHigh + TILEROM_SIZE; tileHigh < tileEnd;) {
			tilePtr += 2;
			*tilePtr++ = *tileHigh++;
			*tilePtr++ = *tileHigh++;
		}
	}
	free(tileDataTemp);

	return true;
}

void CloseTiles() {
	free(TileData);
}

void LoadReplays(const uint8_t* const programData) {
	for (size_t i = 0u; i < DEMOREPLAY_LENGTH * NUMPLAYERS; i++) {
		DemoReplayInputTwin[i] = programData[ROMOFFSET_DEMOREPLAYINPUTTWIN + i];
		DemoReplayInputDoubles[i] = programData[ROMOFFSET_DEMOREPLAYINPUTDOUBLES + i];
		DemoReplayInputVersus[i] = programData[ROMOFFSET_DEMOREPLAYINPUTVERSUS + i];
	}
}

void LoadPals(const uint8_t* const programData) {
	memcpy(Pal.header, &programData[ROMOFFSET_PAL], sizeof(Pal.header));
	for (size_t i = 0u; i < lengthof(Pal.data); i++) {
		Pal.data[i] = ROMCOLOR(&programData[ROMOFFSET_PAL + sizeof(Pal.header)], i * 4u);
	}

	for (size_t i = 0u; i < NUMPALCOLORS_4BPP; i++) {
		Pal1[i] = ROMCOLOR(&programData[ROMOFFSET_PAL1], i * 4u);
		PalSmallText[i] = ROMCOLOR(&programData[ROMOFFSET_PALSMALLTEXT], i * 4u);
		UNK_6032884[i] = ROMCOLOR(&programData[ROMOFFSET_UNK_6032884], i * 4u);
		UNK_60328C4[i] = ROMCOLOR(&programData[ROMOFFSET_UNK_60328C4], i * 4u);
		PalCycleTextPal0[i] = ROMCOLOR(&programData[ROMOFFSET_PALCYCLETEXTPAL0], i * 4u);
		UNK_6033790[i] = ROMCOLOR(&programData[ROMOFFSET_UNK_6033790], i * 4u);
	}
}

void LoadBgMaps(const uint8_t* const programData) {
	for (size_t i = 0u; i < lengthof(BgMapTable); i++) {
		BgMapTable[i]->header.tileInfo =
			((uint32_t)programData[BgMapRomOffsets[i] + 0] << 24) |
			((uint32_t)programData[BgMapRomOffsets[i] + 1] << 16) |
			((uint32_t)programData[BgMapRomOffsets[i] + 2] <<  8) |
			((uint32_t)programData[BgMapRomOffsets[i] + 3] <<  0);
		BgMapTable[i]->header.UNK_4 =
			((uint16_t)programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + 0] << 8) |
			((uint16_t)programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + 1] << 0);
		BgMapTable[i]->header.UNK_6 =
			((uint16_t)programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + sizeoffield(BgMapHeader, UNK_4) + 0] << 8) |
			((uint16_t)programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + sizeoffield(BgMapHeader, UNK_4) + 1] << 0);
		assert(BgMapTable[i]->header.UNK_4 == 0x78u && BgMapTable[i]->header.UNK_6 == 0x50u);

		const uint8_t* programPtr = &programData[BgMapRomOffsets[i] + sizeoffield(BgMapHeader, tileInfo) + sizeoffield(BgMapHeader, UNK_4) + sizeoffield(BgMapHeader, UNK_6)];
		if (BgMapTable[i]->header.tileInfo & BGMAPTILEINFO_PERTILEPAL) {
			assert(((BgMap32*)BgMapTable[i])->header.UNK_4 == 0x78u && ((BgMap32*)BgMapTable[i])->header.UNK_6 == 0x50u);
			uint32_t* name = ((BgMap32*)BgMapTable[i])->names;
			for (size_t i = 0u; i < lengthoffield(BgMap32, names); i++, name++, programPtr += sizeof(uint32_t)) {
				*name =
					((uint32_t)programPtr[0] << 24) |
					((uint32_t)programPtr[1] << 16) |
					((uint32_t)programPtr[2] <<  8) |
					((uint32_t)programPtr[3] <<  0);
			}
		}
		else {
			assert(((BgMap16*)BgMapTable[i])->header.UNK_4 == 0x78u && ((BgMap16*)BgMapTable[i])->header.UNK_6 == 0x50u);
			uint16_t* name = ((BgMap16*)BgMapTable[i])->names;
			for (size_t i = 0u; i < lengthoffield(BgMap16, names); i++, name++, programPtr += sizeof(uint16_t)) {
				*name =
					((uint16_t)programPtr[0] << 8) |
					((uint16_t)programPtr[1] << 0);
			}
		}
	}
}

void LoadObjects(const uint8_t* const programData) {
	for (size_t i = 0u; i < lengthof(Objects.data); i++) {
		for (size_t j = 0u; j < 6u; j++) {
			Objects.data[i][j] =
				((uint16_t)programData[ROMOFFSET_OBJECTDATA + i * sizeof(ObjectData) + j * sizeof(uint16_t) + 0u] << 8) |
						   programData[ROMOFFSET_OBJECTDATA + i * sizeof(ObjectData) + j * sizeof(uint16_t) + 1u];
		}
	}
}

bool OpenSound(const uint8_t* const programData) {
	SoundRomData = malloc(SNDROM_SIZE);
	if (!SoundRomData) {
		fprintf(stderr, "Failed allocating memory for the sound ROM\n\n");
		return false;
	}
	PHYSFS_File* const soundFile = PHYSFS_openRead(SoundRomFileName);
	if (!soundFile || PHYSFS_readBytes(soundFile, SoundRomData, SNDROM_SIZE) != SNDROM_SIZE) {
		if (soundFile) {
			PHYSFS_close(soundFile);
		}
		free(SoundRomData);
		fprintf(stderr, "Failed opening sound ROM file \"%s\"\n\n", SoundRomFileName);
		return false;
	}
	else {
		PHYSFS_close(soundFile);
	}

	const uint8_t* midiData = &programData[ROMOFFSET_MIDI];

	for (size_t i = 0u; i < lengthoffield(MidiData, musicWaves); i++) {
		Midi.musicWaves[i] = ((uint16_t)midiData[sizeof(uint16_t) * i + 0u] << 8) | midiData[sizeof(uint16_t) * i + 1u];
	}
	midiData += lengthoffield(MidiData, musicWaves) * sizeof(uint16_t);

	for (size_t i = 0u; i < lengthoffield(MidiData, UNK_100); i++) {
		STRUCT_MidiData_100* const data = &Midi.UNK_100[i];
		data->wave =
			((uint16_t)midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 2) * i + 0u] << 8) |
					   midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 2) * i + 1u];
		data->nextOctaveFrequency = midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 2) * i + 2u];
	}
	midiData += lengthoffield(MidiData, UNK_100) * (sizeof(uint16_t) + sizeof(uint8_t) * 2);

	for (size_t i = 0u; i < lengthoffield(MidiData, soundEffectWaves); i++) {
		SoundEffectWaveData* const data = &Midi.soundEffectWaves[i];
		data->wave =
			((uint16_t)midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 4) * i + 0u] << 8) |
					   midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 4) * i + 1u];
		data->totalLevel = midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 4) * i + 2u];
		data->UNK_3 = midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 4) * i + 3u];
		data->UNK_4 = midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 4) * i + 4u];
		data->octaveFrequency = midiData[(sizeof(uint16_t) + sizeof(uint8_t) * 4) * i + 5u];
	}
	midiData += lengthoffield(MidiData, soundEffectWaves) * (sizeof(uint16_t) + sizeof(uint8_t) * 4);

	MidiNotes = malloc(MIDINOTES_LENGTH);
	if (!MidiNotes) {
		fprintf(stderr, "Failed allocating MIDI notes buffer\n");
		return false;
	}
	memcpy(MidiNotes, &programData[ROMOFFSET_MIDINOTES], MIDINOTES_LENGTH);
	for (size_t i = 0u; i < lengthoffield(MidiData, barNotes); i++) {
		for (size_t j = 0u; j < lengthoffield(MidiData, barNotes[0]); j++) {
			uint32_t offset =
				((uint32_t)midiData[sizeof(uint32_t) * 16u * i + sizeof(uint32_t) * j + 0u] << 24) |
				((uint32_t)midiData[sizeof(uint32_t) * 16u * i + sizeof(uint32_t) * j + 1u] << 16) |
				((uint32_t)midiData[sizeof(uint32_t) * 16u * i + sizeof(uint32_t) * j + 2u] <<  8) |
				((uint32_t)midiData[sizeof(uint32_t) * 16u * i + sizeof(uint32_t) * j + 3u] <<  0);
			if (offset == 0u) {
				Midi.barNotes[i][j] = NULL;
			}
			else {
				Midi.barNotes[i][j] = &MidiNotes[offset - ROMOFFSET_MIDINOTES];
			}
		}
	}

	return true;
}

void CloseSound() {
	free(MidiNotes);
	free(SoundRomData);
}

bool OpenData() {
	if (!MountRoms()) {
		return false;
	}

	const uint8_t* const programData = OpenProgramData();
	if (!programData) {
		return false;
	}

	if (!OpenTiles()) {
		CloseProgramData(programData);
		return false;
	}
	LoadReplays(programData);
	LoadPals(programData);
	LoadBgMaps(programData);
	LoadObjects(programData);
	if (!OpenSound(programData)) {
		CloseTiles();
		CloseProgramData(programData);
		return false;
	}

	CloseProgramData(programData);

	printf("Successfully loaded data from ROMs.\n\n");
	return true;
}

void CloseData() {
	CloseSound();
	CloseTiles();
}
