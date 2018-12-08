#include <windows.h>
#include "../StringReplaceDll/StringReplaceDll.h"

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		replaceString(L"OLD STRING", L"-->NEW ONE");
	}

	return TRUE;
}
