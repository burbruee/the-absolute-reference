- Requirements -
* A C99 compiler.
* Libraries required of the frontend(s) you wish to compile support for. Some
  frontends have optional features that can be compiled in (such as SDL2 having
  software, OpenGL, and Vulkan renderers).
* Until free assets are created, both of TAP's MAME ROM sets (tgm2 and tgm2p)
  are required. For baremetal platforms, tools are provided that can extract
  and incorporate the assets into the build. The assets (object, tile, palette,
  and sound data) are Arika's property, so don't distribute them, nor builds
  incorporating them.

- Notes -
I call a four-line clear a "skill clear", based on the SK medal. To me, it
seems to be a good alternative to the trademarked name.

The reverse-engineered game code (everything outside Source/Platform, except 
Source/Platform/PsikyoSH and what PsikyoSH uses) is intended to be a fully 
playable reference of TAP, minimally changed to fit my taste in coding style, 
and to ensure full portability. Bugs in the original game that have well-defined 
behavior are kept, but bugs with undefined behavior are fixed; all known bugs 
are noted in the code. Unused code/data is included.

- Contributing -
For the reverse-engineered game code, only bug fixes will be accepted, and it
must be fully portable C99 code. Outside of the game code, contributions don't
have to follow such strict requirements, especially support for more platforms.

- License -
MIT. See License.txt. This software is unaffiliated with Arika, Psikyo, and The
Tetris Company.
