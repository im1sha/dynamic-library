#pragma once

#include <windows.h>
#include <psapi.h>

extern "C"
{
	__declspec(dllexport) int __stdcall getMin(int x, int y);

	__declspec(dllexport) int __stdcall getMax(int x, int y);

	__declspec(dllexport) char* __stdcall getCurrentProcessName();
}
