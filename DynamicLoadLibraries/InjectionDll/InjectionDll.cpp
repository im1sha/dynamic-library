#include <Windows.h>
#include "../StringReplaceDll/StringReplaceDll.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		replaceString(L"OLD STRING", L"-->NEW ONE");
	}

	return TRUE;
}
