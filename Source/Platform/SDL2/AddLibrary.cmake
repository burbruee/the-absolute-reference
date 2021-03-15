# TODO: Refactor this as a function.
add_library("${LIBRARY_NAME}" STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
source_group("Header Files" FILES ${LIBRARY_HEADERS})
target_include_directories("${LIBRARY_NAME}" PUBLIC "${SRC}" "${SRC}/Platform/SDL2/PlatformSupport")

if(VCPKG_TOOLCHAIN)
	set_property(
		TARGET "${LIBRARY_NAME}"
		PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
	)

	set(VCPKG_CRT_LINKAGE dynamic)
	set(VCPKG_LIBRARY_LINKAGE dynamic)

	find_package(SDL2 REQUIRED)
	find_package(PhysFS REQUIRED)
	target_link_libraries(${LIBRARY_NAME}
		PRIVATE
			SDL2::SDL2
			SDL2::SDL2main
			${PHYSFS_LIBRARY}
	)
else()
	find_package(PkgConfig)
	if(PKG_CONFIG_FOUND)
		function(add_pkg_module LIB PKG)
			string(TOUPPER ${PKG} PKG_PREFIX)
			pkg_check_modules(${PKG_PREFIX} REQUIRED ${PKG})
			target_compile_options(${LIB} PRIVATE "${${PKG_PREFIX}_CFLAGS}")
			target_compile_options(${LIB} PRIVATE "${${PKG_PREFIX}_CFLAGS_OTHER}")
			target_include_directories(${LIB} SYSTEM PRIVATE "${${PKG_PREFIX}_INCLUDE_DIRS}")
			target_link_directories(${LIB} PRIVATE "${${PKG_PREFIX}_LIBRARY_DIRS}")
			target_link_libraries(${LIB} PRIVATE "${${PKG_PREFIX}_LIBRARIES}")
			target_link_options(${LIB} PRIVATE "${${PKG_PREFIX}_LDFLAGS}")
			target_link_options(${LIB} PRIVATE "${${PKG_PREFIX}_LDFLAGS_OTHER}")
		endfunction()

		add_pkg_module("${LIBRARY_NAME}" sdl2)
		find_package(PhysFS REQUIRED)
		target_link_libraries(${LIBRARY_NAME}
			PRIVATE
				${PHYSFS_LIBRARY}
		)
		target_include_directories(${LIBRARY_NAME}
			PRIVATE
				${PHYSFS_INCLUDE_DIR}
		)
	endif()
endif()
