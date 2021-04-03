set(PLATFORM_SOURCES
	${GAME_SOURCES}

	"${SRC}/Platform/Util/AccessConfig.c"
	"${SRC}/Platform/Util/AccessData.c"
	"${SRC}/Platform/Util/AccessEeprom.c"
	"${SRC}/Platform/Util/AccessPaths.c"
	"${SRC}/Platform/Util/Eeprom.c"
	"${SRC}/Platform/Util/EepromData.c"
	"${SRC}/Platform/Util/HwData.c"
	"${SRC}/Platform/Util/ini.c"
	"${SRC}/Platform/Util/RomData.c"

	"${SRC}/Platform/Util/Render/Render.c"

	"${SRC}/Platform/SDL2/AccessAudio.c"
	"${SRC}/Platform/SDL2/AccessDisplay.c"
	"${SRC}/Platform/SDL2/HwSound.c"
	"${SRC}/Platform/SDL2/PlatformUpdate.c"
)
set(PLATFORM_HEADERS
	${GAME_HEADERS}

	"${SRC}/Platform/Util/AccessConfig.h"
	"${SRC}/Platform/Util/AccessData.h"
	"${SRC}/Platform/Util/AccessEeprom.h"
	"${SRC}/Platform/Util/AccessPaths.h"
	"${SRC}/Platform/Util/Color.h"
	"${SRC}/Platform/Util/EepromData.h"
	"${SRC}/Platform/Util/HwData.h"
	"${SRC}/Platform/Util/ini.h"
	"${SRC}/Platform/Util/Macros.h"

	"${SRC}/Platform/Util/Render/Render.h"

	"${SRC}/Platform/SDL2/AccessAudio.h"
	"${SRC}/Platform/SDL2/AccessDisplay.h"
)

find_package(SDL2 REQUIRED)
find_package(PhysFS REQUIRED)

include(${SRC}/Platform/Util/AddBinary.cmake)

AddBinary(
	PlatformSDL2
	LIBRARY
	SOURCES ${PLATFORM_SOURCES} ${PLATFORM_HEADERS}
	LIBRARIES
		SDL2::SDL2
		${PHYSFS_LIBRARY}
)

AddBinary(
	taref
	EXECUTABLE
	SOURCES "Game.c" ${PLATFORM_HEADERS}
	LIBRARIES
		PlatformSDL2
		SDL2::SDL2
		SDL2::SDL2main
		${PHYSFS_LIBRARY}
)

AddBinary(
	sndplay
	EXECUTABLE
	SOURCES "SoundPlayer.c" ${PLATFORM_HEADERS}
	LIBRARIES
		PlatformSDL2
		SDL2::SDL2
		SDL2::SDL2main
		${PHYSFS_LIBRARY}
)
