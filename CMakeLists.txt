cmake_minimum_required(VERSION 3.10)

# TODO: Add a CMake option for specifying the platform, choosing a sane default
# based on the platform CMake was run on if the option wasn't specified.
set(PLATFORM_NAME "SDL2")

project("TheAbsoluteReference" VERSION 0.0.0 LANGUAGES C)
set(EXECUTABLE_NAME "taref")
add_subdirectory("Source/Platform/${PLATFORM_NAME}")
