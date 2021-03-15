set(PLATFORM_SOURCES
	"${SRC}/Platform/Util/AccessConfig.c"
	"${SRC}/Platform/Util/AccessData.c"
	"${SRC}/Platform/Util/AccessEeprom.c"
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
	"${SRC}/Platform/Util/AccessConfig.h"
	"${SRC}/Platform/Util/AccessData.h"
	"${SRC}/Platform/Util/AccessEeprom.h"
	"${SRC}/Platform/Util/Color.h"
	"${SRC}/Platform/Util/EepromData.h"
	"${SRC}/Platform/Util/HwData.h"
	"${SRC}/Platform/Util/ini.h"
	"${SRC}/Platform/Util/Macros.h"
	"${SRC}/Platform/Util/Render.h"

	"${SRC}/Platform/SDL2/AccessDisplay.h"
)
set(LIBRARY_SOURCES ${GAME_SOURCES} ${PLATFORM_SOURCES})
set(LIBRARY_HEADERS ${GAME_SOURCES} ${PLATFORM_HEADERS})
set(LIBRARY_NAME PlatformSDL2)
include(AddLibrary.cmake)

set(EXECUTABLE_SOURCES
	"Main.c"
)
set(EXECUTABLE_HEADERS
	${GAME_HEADERS}
	${PLATFORM_HEADERS}
)
set(EXECUTABLE_NAME taref)
include(AddExecutable.cmake)

add_subdirectory(Tool)
