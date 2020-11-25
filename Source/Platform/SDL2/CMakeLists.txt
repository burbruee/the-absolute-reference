list(APPEND EXECUTABLE_SOURCES
	"../Util/HwData.c"
	"../Util/ini.c"
	"../Util/Render.c"
	"../Util/RomData.c"

	"EepromAccess.c"
	"PlatformUpdate.c"
)
list(APPEND EXECUTABLE_HEADERS
	"../Util/Color.h"
	"../Util/HwData.h"
	"../Util/ini.h"
	"../Util/Macros.h"
	"../Util/Render.h"

	"PlatformSupport/Config.h"
	"PlatformSupport/EepromAccess.h"
	"PlatformSupport/HwData.h"
	"PlatformSupport/PlatformUpdate.h"
	"PlatformSupport/PlatformTypes.h"
)

add_executable("${EXECUTABLE_NAME}" ${EXECUTABLE_SOURCES} ${EXECUTABLE_HEADERS})
source_group("Header Files" FILES ${EXECUTABLE_HEADERS})
target_include_directories("${EXECUTABLE_NAME}" PUBLIC "../.." "../../Generated" "PlatformSupport")

if (VCPKG_TOOLCHAIN)
	set_property(
		TARGET "${EXECUTABLE_NAME}"
		PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
	)
endif()

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
	pkg_check_modules(SDL2 REQUIRED sdl2)
	target_compile_options(${EXECUTABLE_NAME} PRIVATE "${SDL2_CFLAGS}")
	target_compile_options(${EXECUTABLE_NAME} PRIVATE "${SDL2_CFLAGS_OTHER}")
	target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE "${SDL2_INCLUDE_DIRS}")
	target_link_directories(${EXECUTABLE_NAME} PRIVATE "${SDL2_LIBRARY_DIRS}")
	target_link_libraries(${EXECUTABLE_NAME} PRIVATE "${SDL2_LIBRARIES}")
	target_link_options(${EXECUTABLE_NAME} PRIVATE "${SDL2_LDFLAGS}")
	target_link_options(${EXECUTABLE_NAME} PRIVATE "${SDL2_LDFLAGS_OTHER}")
elseif(VCPKG_TOOLCHAIN)
	set(VCPKG_CRT_LINKAGE dynamic)
	set(VCPKG_LIBRARY_LINKAGE dynamic)

	find_package(SDL2 REQUIRED)
	target_link_libraries(${EXECUTABLE_NAME}
		PUBLIC
			SDL2::SDL2
			SDL2::SDL2main
	)
endif()

install(TARGETS "${EXECUTABLE_NAME}" RUNTIME)
