// This is a custom configuration file for Raylib.
// Raylib will automatically include this file if it's in the include path.

// We force the definitions that prevent Windows API name collisions.
// Because Raylib includes this file FIRST, these definitions will be active
// before any part of Raylib or its dependencies (like windows.h) are processed.
#define NOGDI
#define WIN32_LEAN_AND_MEAN