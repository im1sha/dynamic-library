#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#define INVALID_RESULT -1

#include <windows.h>

__declspec(dllexport) int replaceString(const wchar_t* toReplace, const wchar_t* newString);
