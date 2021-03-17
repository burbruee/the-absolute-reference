# AddBinarySDL2(
# 	${NAME}
# 	${TYPE}
# 	SOURCES ${SOURCE0} ${SOURCE1} ...
# 	HEADERS ${HEADER0} ${HEADER1} ...
# 	LIBRARIES ${LIBRARY0} ${LIBRARY1} ...
# )
#
# TYPE can be either LIBRARY or EXECUTABLE.
#
# For empty lists, provide an empty string after the list name, i.e.:
# AddBinarySDL2(
# 	program
#	EXECUTABLE
# 	SOURCES "Main.c"
# 	HEADERS ""
# 	LIBRARIES ""
# )
#
# The library list should only be a list of libraries created with previous 
# LIBRARY calls of AddBinarySDL2.
function(AddBinarySDL2 NAME TYPE)
	if((NOT ${TYPE} STREQUAL LIBRARY) AND (NOT ${TYPE} STREQUAL EXECUTABLE))
		message(FATAL_ERROR "Invalid type; must be LIBRARY or EXECUTABLE")
	endif()

	if(${ARGC} GREATER 5)
		set(START 2)
		set(LIST_NAMES SOURCES HEADERS LIBRARIES)
		list(GET LIST_NAMES -1 END)
		foreach(I RANGE 2)
			set(LENGTH 0)
			list(GET LIST_NAMES ${I} LIST_NAME)
			if (${ARGV${START}} STREQUAL ${END})
				math(EXPR START "${START} + 1")
				list(SUBLIST ARGV ${START} -1 ${END})
				break()
			elseif(${ARGV${START}} STREQUAL ${LIST_NAME})
				math(EXPR START "${START} + 1")
				foreach(J RANGE ${START} ${ARGC})
					math(EXPR NEXT "${I} + 1")
					list(GET LIST_NAMES ${NEXT} NEXT_LIST)
					if(${ARGV${J}} STREQUAL ${NEXT_LIST})
						list(SUBLIST ARGV ${START} ${LENGTH} ${LIST_NAME})
						math(EXPR START "${J}")
						break()
					endif()
					math(EXPR LENGTH "${LENGTH} + 1")
				endforeach()
			else()
				message(FATAL_ERROR "Invalid ${TYPE} ${LIST_NAME} list")
			endif()
		endforeach()
	else()
		message(FATAL_ERROR "Invalid ${TYPE} arguments")
	endif()
	
	if(${TYPE} STREQUAL LIBRARY)
		add_library(${NAME} STATIC ${SOURCES} ${HEADERS})
	elseif(${TYPE} STREQUAL EXECUTABLE)
		add_executable(${NAME} ${SOURCES} ${HEADERS})
	endif()
	source_group("Header Files" FILES ${HEADERS})
	target_include_directories("${NAME}" PUBLIC "${SRC}" "${SRC}/Platform/SDL2/PlatformSupport")
	target_link_libraries(${NAME} PUBLIC ${LIBRARIES})

	if(VCPKG_TOOLCHAIN)
		set_property(
			TARGET "${NAME}"
			PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
			${CMAKE_SOURCE_DIR}
		)

		set(VCPKG_CRT_LINKAGE dynamic)
		set(VCPKG_LIBRARY_LINKAGE dynamic)

		find_package(SDL2 REQUIRED)
		find_package(PhysFS REQUIRED)
		target_link_libraries(${NAME}
			PRIVATE
				SDL2::SDL2
				SDL2::SDL2main
				${PHYSFS_LIBRARY}
		)
	else()
		find_package(PkgConfig)
		if(PKG_CONFIG_FOUND)
			function(add_pkg_module NAME PKG)
				string(TOUPPER ${PKG} PKG_PREFIX)
				pkg_check_modules(${PKG_PREFIX} REQUIRED ${PKG})
				target_compile_options(${NAME} PRIVATE "${${PKG_PREFIX}_CFLAGS}")
				target_compile_options(${NAME} PRIVATE "${${PKG_PREFIX}_CFLAGS_OTHER}")
				target_include_directories(${NAME} SYSTEM PRIVATE "${${PKG_PREFIX}_INCLUDE_DIRS}")
				target_link_directories(${NAME} PRIVATE "${${PKG_PREFIX}_LIBRARY_DIRS}")
				target_link_libraries(${NAME} PRIVATE "${${PKG_PREFIX}_LIBRARIES}")
				target_link_options(${NAME} PRIVATE "${${PKG_PREFIX}_LDFLAGS}")
				target_link_options(${NAME} PRIVATE "${${PKG_PREFIX}_LDFLAGS_OTHER}")
			endfunction()

			add_pkg_module(${NAME} sdl2)
			find_package(PhysFS REQUIRED)
			target_link_libraries(${NAME}
				PRIVATE
					${PHYSFS_LIBRARY}
					m
			)
			target_include_directories(${NAME}
				PRIVATE
					${PHYSFS_INCLUDE_DIR}
			)
		endif()
	endif()
endfunction()

