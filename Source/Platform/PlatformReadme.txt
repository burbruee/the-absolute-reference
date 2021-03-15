- To Platform Developers -
Create a subdirectory for your platform in the "Platform" directory ("SDL2"), a 
subdirectory in there named "PlatformSupport", then in "PlatformSupport" create 
only these files:
* Config.h
* HwData.h
* HwSound.h
* PlatformTypes.h
* PlatformUpdate.h
Check existing platform implementations for info on what has to be defined in 
those headers; the PsikyoSH version is the reference all other platforms need to 
conform to. Your platform's own code goes in the platform's top directory 
("SDL2") or subdirectories other than "PlatformSupport".

The Util subdirectory contains some basic functions and data that are fully
portable, and intended as a way to start developing a new port, and as a
reference to help development of specialized platform-specific code, such as
hardware accelerated renderers.