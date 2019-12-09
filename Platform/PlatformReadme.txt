- To Platform Developers -
Create a subdirectory for each platform, define main in
Platform/PlatformName/Main.c, and call the MainLoop function, of Game/Loop.h,
in main. The game code is written to, hopefully, not require any changes for
each port, assuming the target platform is sufficiently capable. Refer to the
PsikyoSH port for a reference on what needs to be initialized for a platform,
depending on what the platform intends to support. It might make sense to
create a thread just for executing the Loop function, if you want a separate
renderer thread; the SDL2 port demonstrates that style of implementation.

The Util subdirectory contains some basic functions that are fully portable,
and intended as a way to start developing a new platform, and as a reference to
help development of specialized platform-specific code, such as hardware
accelerated renderers.

A Test platform is also available using the Util functionality, that only uses
standard C; it's useful for automated use, such as testing the code against
recordings of TAP, or running with an AI at faster-than-realtime speed. The
Test platform has its own game support code, since it allows for things such as
faster startup and initialization, no-opping out the renderer and sound,
setting of the randomizer seed to match a TAP recording, optimization of an AI
against specific seeds, and creation of TASs against specific seeds.

Some types used by the game code have to be declared by the platforms in
Platform/PlatformName/PlatformTypes.h. For example, the Color type has to be
defined as some lvalue type, with some macros defined to access color
components; uint32_t, or some other unsigned 32-bit integer type, is probably
what you should use, if your platform supports 24-bit color.
