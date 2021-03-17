include(AddBinarySDL2.cmake)

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
	"${SRC}/Platform/Util/Render.c"
	"${SRC}/Platform/Util/RomData.c"

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
	"${SRC}/Platform/Util/Render.h"

	"${SRC}/Platform/SDL2/AccessDisplay.h"
)
AddBinarySDL2(
	PlatformSDL2
	LIBRARY
	SOURCES ${PLATFORM_SOURCES}
	HEADERS ${PLATFORM_HEADERS}
	LIBRARIES ""
)

AddBinarySDL2(
	taref
	EXECUTABLE
	SOURCES "Main.c"
	HEADERS ${PLATFORM_HEADERS}
	LIBRARIES PlatformSDL2
)

add_subdirectory(Tool)
