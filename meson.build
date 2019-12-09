# SDL2 Linux desktop version. Don't try to actually use this, the code is incomplete.
# TODO: Verify the available version of SDL 2 is supported; checking is
# required for Vulkan support. But allow no checking for frontends that only
# use SDL 2.0.0 features, so systems with out-of-date SDL 2 libraries can at
# least run without Vulkan.
# TODO: Add build options to select supported frontends, such as OpenGL and Vulkan renderers in the same SDL2 build.
# TODO: Rewrite this top-level meson.build to support all the frontends, rather than just SDL2.
# TODO: Split build up into a meson.build per directory.
project('TheAbsoluteReference', 'c')
compiler = meson.get_compiler('c')

sdl2_dep = dependency('sdl2')
# TODO: Refactor the following deps into a single "unix_dep", and add a similar dep for Windows, etc.
opengl_dep = compiler.find_library('GL')
dl_dep = compiler.find_library('dl', required: false)
math_dep = compiler.find_library('m', required: false)

# TODO: Add remaining SDL2 sources.
platform_sources = [
	'Platform/SDL2/Main.c'
]

# The order of the sources matches the order that groups of functions are defined in
# the TAP program ROM.
#
# The order of function definition in each source file also matches TAP's
# program ROM.
# TODO: Add remaining game sources.
game_sources = [
	# Game sources. Some (DisplayObject.c for example) may have been
	# provided to Arika as example code they modified, while the rest was
	# Arika's original work. The game functions used by system functions
	# almost certainly were modified sources provided to Arika, though.
	'Game/Player.c',
	'Game/Loop.c',
	'Game/Text.c',
	'Game/DisplayObject.c',
	'Game/Grade.c',
	'Game/Ranking.c',
	'Game/ShowBlockField.c',
	'Game/Graphics.c',
	'Game/Effect.c',
	'Game/Item.c',
	'Game/Medal.c',
	'Game/Debug.c',
	'Game/RankingCode.c',

	# System support sources. These sources may not have been available to
	# Arika.
	'Game/Video.c',
	'Game/Frame.c',
	'Game/Math.c',
	'Game/Sound.c',
	'Game/EEPROM.c',
	'Game/LibC.c'
]

executable('taref',
	platform_sources,
	game_sources,
	dependencies : [sdl2_dep, opengl_dep, dl_dep, math_dep])
