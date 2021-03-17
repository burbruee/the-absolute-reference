#include "Platform/Util/AccessPaths.h"
#include "physfs.h"
#include <stdio.h>

bool OpenPaths(const char* const argv0) {
	if (!PHYSFS_init(argv0)) {
		fprintf(stderr, "Error with PHYSFS_init: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	if (!PHYSFS_setSaneConfig("nightmareci", "taref", "ZIP", 0, 0)) {
		fprintf(stderr, "Error setting sane PhysicsFS config: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	printf("Search path directories:\n");
	for (char** searchPath = PHYSFS_getSearchPath(); searchPath != NULL && *searchPath != NULL; searchPath++) {
		printf("%s\n", *searchPath);
	}
	printf("\n");
	
	return true;
}

void ClosePaths() {
	if (!PHYSFS_deinit()) {
		fprintf(stderr, "Failed PhysicsFS deinit: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}
