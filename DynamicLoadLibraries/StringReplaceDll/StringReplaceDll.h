#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

__declspec(dllexport) int replaceString(const wchar_t* toReplace, const wchar_t* newString);
