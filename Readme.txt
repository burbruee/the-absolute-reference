- Installation -
If you install the necessary files correctly (the ROMs, taref.ini, and eeprom)
then you can put the game's EXE/binary anywhere on your system, so long as it
can access the libraries it needs (so you'd need SDL2.dll and physfs.dll in the
same folder as taref.exe on Windows).

Windows:
Put tgm2.zip and tgm2p.zip here:
C:\Users\<your-username-here>\AppData\Roaming\nightmareci\taref\roms\tgm2.zip
C:\Users\<your-username-here>\AppData\Roaming\nightmareci\taref\roms\tgm2p.zip

You can leave taref.ini in the same folder as taref.exe. It's recommended
to copy it here though, as when in-game configuration is supported, the INI has
to be here to be able to change it:
C:\Users\<your-username-here>\AppData\Roaming\nightmareci\taref\taref.ini

If you don't provide an INI, a default one only having keyboard controls will
be created.

You can copy over your MAME "eeprom", probably in nvram\tgm2p\eeprom, to here,
to use your old save data.
C:\Users\<your-username-here>\AppData\Roaming\nightmareci\taref\nvram\tgm2p\eeprom

You don't have to copy in an "eeprom" file; a default one will be created if you
don't provide one, just like in MAME.

Other platforms:
Run the game from command line, and it will tell you the directories where
files need to go. In the event some ROMs are missing in tgm2.zip or tgm2p.zip,
it will tell you which are missing.

- Building and Running Requirements -
* A C99 compiler.
* Libraries required of the frontend(s) you wish to compile support for.
  The only frontend currently supported, SDL2, requires SDL 2.0 and PhysicsFS.
* Until free assets are created, both of TAP's MAME ROM sets (tgm2 and tgm2p)
  are required.

- Notes -
In the source code, I call a four-line clear a "skill clear", based on the SK
medal. To me, it seems to be a good alternative to the trademarked name.

The reverse-engineered game code (everything outside Source/Platform, except 
Source/Platform/PsikyoSH and what PsikyoSH uses) is intended to be a fully 
playable reference of TAP, minimally changed to fit my taste in coding style, 
and to ensure full portability. Bugs in the original game that have well-defined 
behavior are kept, but bugs with undefined behavior are fixed; all known bugs 
are noted in the code. Unused code/data is included.

- Contributing -
For the reverse-engineered game code, only bug fixes will be accepted, and it
must be fully portable C99 code; more specifically, it has to use the subset of
C99 that works with both more fully compliant compilers (GCC, Clang) and less
compliant ones (MSVC). Outside of the game code, contributions don't have to
follow such strict requirements, especially support for more platforms.

- License -
MIT. See License.txt. This software is unaffiliated with Arika, Psikyo, and The
Tetris Company.
