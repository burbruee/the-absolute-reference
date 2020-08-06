#pragma once
// TODO: Have the build system generate this.

#define USESTDC

#ifdef _WIN32
// From what I've read, Microsoft seems to guarantee little endianness on all
// Windows versions.
#define BIGEND 0
#else
#endif

// Don't define this as "PLUS"; I don't want recordings that might have bugs or
// cheats in them passed off as recordings of the real TAP. Even the reference,
// as I provided it, may have bugs not in TAP.
// -Brandon McGriff
// TODO: Define this per the platform; "TEST" for the pure-C test version,
// "SDL2" for SDL2, "REF" for PS5V2, etc. "REF" for now, since it's not defined
// per platform yet, and to keep this code differentiated from TAP.
#define VERSION_NAME "REF"

// TODO: Define this based on how long the version name is. For names four
// characters or shorter, it can be zero. Only use it at the version name title
// screen.
#define VERSION_OFFSET 0