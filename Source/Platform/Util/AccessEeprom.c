#include "Platform/Util/AccessEeprom.h"
#include "Platform/Util/EepromData.h"
#include "physfs.h"
#include <stdio.h>
#include <stdlib.h>

static const char* const DefaultEepromFilename = "roms/tgm2p/tgm2p.default.nv";
#define EEPROM_DIR "nvram/tgm2p/"
static const char* const EepromFilename = EEPROM_DIR "eeprom";

bool OpenEeprom() {
	PHYSFS_Stat eepromStat;
	if (!PHYSFS_stat(EepromFilename, &eepromStat)) {
		if (!PHYSFS_stat(DefaultEepromFilename, &eepromStat)) {
			fprintf(stderr, "Failed to find the default \"%s\" EEP-ROM save data in the \"tgm2p\" ROM set\n\n", DefaultEepromFilename);
			return false;
		}
		else if (eepromStat.filetype != PHYSFS_FILETYPE_REGULAR) {
			fprintf(stderr, "Failed opening EEP-ROM save data, the filesystem entity \"%s\" is not a regular file\n\n", DefaultEepromFilename);
			return false;
		}
		else if (eepromStat.filesize < 0) {
			fprintf(stderr, "File size of EEP-ROM save data \"%s\" is invalid\n\n", DefaultEepromFilename);
			return false;
		}
		else {
			// No valid EEP-ROM save data available, attempt to read the default EEP-ROM provided by the "tgm2p" ROM set.
			PHYSFS_File* const eepromFile = PHYSFS_openRead(DefaultEepromFilename);
			if (PHYSFS_readBytes(eepromFile, EepromData, sizeof(EepromData)) != sizeof(EepromData)) {
				fprintf(stderr, "Error while reading \"%s\"\n\n", DefaultEepromFilename);
				return false;
			}
			printf("Opened default EEP-ROM save data \"%s\".\n\n", DefaultEepromFilename);
		}
	}
	else if (eepromStat.filetype != PHYSFS_FILETYPE_REGULAR) {
		fprintf(stderr, "Failed opening EEP-ROM save data, the filesystem entity \"%s\" is not a regular file\n", EepromFilename);
		return false;
	}
	else if (eepromStat.filesize < 0) {
		fprintf(stderr, "File size of EEP-ROM save data \"%s\" is invalid\n", EepromFilename);
		return false;
	}
	else {
		// Valid EEP-ROM save data available in the filesystem.
		PHYSFS_File* const eepromFile = PHYSFS_openRead(EepromFilename);
		if (PHYSFS_readBytes(eepromFile, EepromData, sizeof(EepromData)) != sizeof(EepromData)) {
			fprintf(stderr, "Error while reading \"%s\"\n\n", EepromFilename);
			PHYSFS_close(eepromFile);
			return false;
		}
		PHYSFS_close(eepromFile);
		printf("Opened EEP-ROM save data \"%s\".\n\n", EepromFilename);
	}

	printf("Finished reading EEP-ROM save data.\n\n");

	return true;
}

void SaveEeprom() {
	PHYSFS_File* eepromFile = NULL;
	if (!PHYSFS_mkdir(EEPROM_DIR) || (eepromFile = PHYSFS_openWrite(EepromFilename)) == NULL || PHYSFS_writeBytes(eepromFile, EepromData, sizeof(EepromData)) != sizeof(EepromData)) {
		fprintf(stderr, "Failed writing EEP-ROM save data to file \"%s\"\n\n", EepromFilename);
	}
	PHYSFS_close(eepromFile);
	printf("Finished saving EEP-ROM save data\n\n");
}
