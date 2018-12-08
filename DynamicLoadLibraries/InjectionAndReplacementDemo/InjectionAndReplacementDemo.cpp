#include <windows.h>
#include <string>
#include <iostream>

std::wstring str = L"OLD STRING";

int wmain()
{
	const DWORD sleepTime = 2000;

	while (true)
	{
		::Sleep(sleepTime);
		std::wcout << str << std::endl;		
	}

	return 0;
}
