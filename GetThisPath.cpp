

#include <Shlwapi.h>
#include "GetThisPath.h"
//
#pragma comment(lib, "shlwapi.lib")


TCHAR* GetThisPath(TCHAR* dest, size_t destSize)
{
	if (!dest) return NULL;
	if (MAX_PATH > destSize) return NULL;

	DWORD length = GetModuleFileName( NULL, dest, destSize );
	PathRemoveFileSpec(dest);
	return dest;
}


BOOL SetWorkingDirectory2AppPath()	// 
{
	LPTSTR szDest = (LPTSTR)GlobalAlloc(GPTR, MAX_PATH);
	GetThisPath(szDest, MAX_PATH);
	return SetCurrentDirectory(szDest);
	GlobalFree(szDest);

}
