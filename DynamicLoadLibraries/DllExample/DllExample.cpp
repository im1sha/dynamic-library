#include "DllExample.h"

__declspec(dllexport) int __stdcall getMin(int x, int y)
{
	return x < y ? x : y;
}

__declspec(dllexport) int __stdcall getMax(int x, int y)
{
	return x < y ? y : x;
}

__declspec(dllexport) char* __stdcall getCurrentProcessName()
{
	const WORD nameLength = MAX_PATH + 1;
	HANDLE currentProcess = ::GetCurrentProcess();
	char* processName = new char[nameLength] {};
	if (::GetModuleBaseName(currentProcess, nullptr, processName, nameLength) == 0)
	{
		delete[] processName;
		return nullptr;
	}
	else
	{
		return processName;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}
