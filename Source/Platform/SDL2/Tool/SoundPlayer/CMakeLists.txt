set(EXECUTABLE_SOURCES
	"Main.c"
)
set(EXECUTABLE_HEADERS
	${GAME_HEADERS}
	${PLATFORM_HEADERS}
)
set(EXECUTABLE_NAME sndplay)
include(../../AddExecutable.cmake)
