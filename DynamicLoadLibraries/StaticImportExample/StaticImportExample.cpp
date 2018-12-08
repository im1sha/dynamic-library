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
	int x = 0;
	int y = 0;

	char* processName = getCurrentProcessName();

	if (processName == nullptr)
	{
		::printf("Error getting current process name\n");
	}
	else
	{
		::printf("Current process name: %s\n", processName);
		delete[] processName;
	}

	::printf("Enter x: ");
	::scanf_s("%d", &x);

	::printf("Enter y: ");
	::scanf_s("%d", &y);

	::printf("Minimum of x and y: %d\nMaximum of x and y: %d\n", getMin(x, y), getMax(x, y));
	::system("pause");
}
