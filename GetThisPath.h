
// from "http://stackoverflow.com/questions/2647429/c-windows-path-to-the-folder-where-the-executable-is-located"

#pragma once
#include <windows.h>

/// dest is expected to be MAX_PATH in length.
/// returns dest
///     TCHAR dest[MAX_PATH];
///     GetThisPath(dest, MAX_PATH);
TCHAR* GetThisPath(TCHAR* dest, size_t destSize);

BOOL SetWorkingDirectory2AppPath();		// added by Claudiu Ciutacu