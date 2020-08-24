- To Platform Developers -
Create a subdirectory for your platform in the "Platform" directory ("SDL2"), a 
subdirectory in there named "PlatformSupport", then in "PlatformSupport" create 
only these files:
* Config.h
* HwData.h
* PlatformTypes.h
* PlatformUpdate.h
Check existing platform implementations for info on what has to be defined in 
those headers; the PsikyoSH version is the reference all other platforms need to 
conform to. Your platform's own code goes in the platform's top directory 
("SDL2") or subdirectories other than "PlatformSupport".

The Util subdirectory contains some basic functions that are fully portable,
and intended as a way to start developing a new port, and as a reference to
help development of specialized platform-specific code, such as hardware
accelerated renderers.

A Test platform is also available using the Util functionality, that only uses
standard C; it's useful for automated use, such as testing the code against
recordings of TAP, or running with an AI at faster-than-realtime speed. The
Test platform has its own game support code, since it allows for things such as
faster startup and initialization, no-opping out the renderer and sound,
setting of the randomizer seed to match a TAP recording, optimization of an AI
against specific seeds, and creation of TAS's against specific seeds.
