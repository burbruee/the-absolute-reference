# TODO: Refactor this to be a function. Also add taking a list of libraries to link to.

add_executable("${EXECUTABLE_NAME}" ${EXECUTABLE_SOURCES} ${EXECUTABLE_HEADERS})
source_group("Header Files" FILES ${EXECUTABLE_HEADERS})
target_include_directories("${EXECUTABLE_NAME}" PUBLIC "${SRC}" "${SRC}/Platform/SDL2/PlatformSupport")
target_link_libraries(${EXECUTABLE_NAME} PUBLIC PlatformSDL2)

if(VCPKG_TOOLCHAIN)
	set_property(
		TARGET "${EXECUTABLE_NAME}"
		PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
	)

	set_property(
		DIRECTORY "${CMAKE_SOURCE_DIR}"
		PROPERTY VS_STARTUP_PROJECT ${EXECUTABLE_NAME}
	)

	set(VCPKG_CRT_LINKAGE dynamic)
	set(VCPKG_LIBRARY_LINKAGE dynamic)

	find_package(SDL2 REQUIRED)
	find_package(PhysFS REQUIRED)
	target_link_libraries(${EXECUTABLE_NAME}
		PRIVATE
			SDL2::SDL2
			SDL2::SDL2main
			${PHYSFS_LIBRARY}
	)
else()
	find_package(PkgConfig)
	if(PKG_CONFIG_FOUND)
		function(add_pkg_module EXE PKG)
			string(TOUPPER ${PKG} PKG_PREFIX)
			pkg_check_modules(${PKG_PREFIX} REQUIRED ${PKG})
			target_compile_options(${EXE} PRIVATE "${${PKG_PREFIX}_CFLAGS}")
			target_compile_options(${EXE} PRIVATE "${${PKG_PREFIX}_CFLAGS_OTHER}")
			target_include_directories(${EXE} SYSTEM PRIVATE "${${PKG_PREFIX}_INCLUDE_DIRS}")
			target_link_directories(${EXE} PRIVATE "${${PKG_PREFIX}_LIBRARY_DIRS}")
			target_link_libraries(${EXE} PRIVATE "${${PKG_PREFIX}_LIBRARIES}")
			target_link_options(${EXE} PRIVATE "${${PKG_PREFIX}_LDFLAGS}")
			target_link_options(${EXE} PRIVATE "${${PKG_PREFIX}_LDFLAGS_OTHER}")
		endfunction()

		add_pkg_module("${EXECUTABLE_NAME}" sdl2)
		find_package(PhysFS REQUIRED)
		target_link_libraries(${EXECUTABLE_NAME}
			PRIVATE
				${PHYSFS_LIBRARY}
		)
		target_include_directories(${EXECUTABLE_NAME}
			PRIVATE
				${PHYSFS_INCLUDE_DIR}
		)
	endif()
endif()
