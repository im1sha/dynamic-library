#include <windows.h>
#include <stdio.h>

extern "C"
{
	__declspec(dllimport) int __stdcall getMin(int x, int y);

	__declspec(dllimport) int __stdcall getMax(int x, int y);

	__declspec(dllimport) char* __stdcall getCurrentProcessName();
}

int wmain()
{
	int x, y;
	char* processName = getCurrentProcessName();

	if (processName == nullptr)
	{
		::printf_s("Error getting current process name\n");
	}
	else
	{
		::printf_s("Current process name: %s\n", processName);
		delete[] processName;
	}

	::printf_s("Enter x: ");
	::scanf_s("%d", &x);
	::printf_s("Enter y: ");
	::scanf_s("%d", &y);

	::printf_s("Minimum of x and y: %d\nMaximum of x and y: %d\n", getMin(x, y), getMax(x, y));
	::system("pause");
}
