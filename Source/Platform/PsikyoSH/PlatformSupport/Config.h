#pragma once

#define BIGEND 1

// Don't define this as "PLUS"; I don't want recordings that might have bugs or
// cheats in them passed off as recordings of the real TAP. Even the reference,
// as I provided it, may have bugs not in TAP.
// -Brandon McGriff
#define VERSION_NAME "REF"

// Define this based on how long the version name is. For names four characters 
// or shorter, it can be zero. Only use it at the version name title screen.
#define VERSION_OFFSET 0
