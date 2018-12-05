#include <Windows.h>
#include <string>
#include <iostream>

std::wstring replacement = L"OLD STRING";

int wmain()
{
	while (true)
	{
		std::wcout << replacement << std::endl;
		::Sleep(1000);
	}

	return 0;
}
