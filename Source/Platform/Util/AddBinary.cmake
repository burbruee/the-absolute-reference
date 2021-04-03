# AddBinary(
# 	${NAME}
# 	${TYPE}
# 	SOURCES ${SOURCE0} ${SOURCE1} ...
# 	LIBRARIES ${LIBRARY0} ${LIBRARY1} ...
# )
#
# TYPE can be either LIBRARY or EXECUTABLE.
#
# For empty lists, provide an empty string after the list name, i.e.:
# AddBinary(
# 	program
#	EXECUTABLE
# 	SOURCES "Main.c"
# 	LIBRARIES ""
# )
function(AddBinary NAME TYPE)
	if((NOT ${TYPE} STREQUAL LIBRARY) AND (NOT ${TYPE} STREQUAL EXECUTABLE))
		message(FATAL_ERROR "Invalid type; must be LIBRARY or EXECUTABLE")
	endif()

	if(${ARGC} GREATER 4)
		set(START 2)
		set(LIST_NAMES SOURCES LIBRARIES)
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
		add_library(${NAME} STATIC ${SOURCES})
	elseif(${TYPE} STREQUAL EXECUTABLE)
		add_executable(${NAME} ${SOURCES})
	endif()
	target_include_directories(${NAME} PUBLIC "${SRC}" "${SRC}/Platform/${PLATFORM_NAME}/PlatformSupport")
	target_link_libraries(${NAME} PRIVATE ${LIBRARIES})
endfunction()
