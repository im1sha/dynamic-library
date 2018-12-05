#include <windows.h>
#include <stdio.h>

typedef int (__stdcall MinPtr) (int x, int y);

typedef int (__stdcall MaxPtr) (int x, int y);

typedef char* (__stdcall GetCurrentProcessNamePtr) ();

char* getFullFunctionName(const char* functionName, int argumentsSize)
{
	int nameLength = ::lstrlen(functionName) + 2;
	int sizeCounter = argumentsSize;
	do
	{
		sizeCounter /= 10;
		nameLength++;
	} while (sizeCounter > 0);

	char* result = new char[nameLength + 1];
	if (::sprintf_s(result, nameLength + 1, "_%s@%d", functionName, argumentsSize) == -1)
	{
		delete[] result;
		return nullptr;
	}
	else
	{
		return result;
	}
}

int main()
{
	const char* dllPath = "DllExample.dll";
	HMODULE loadedDll = ::LoadLibrary(dllPath);

	if (loadedDll == NULL)
	{
		::printf("Error loading library %s\n", dllPath);
	}
	else
	{
		const char* minDecoratedString = getFullFunctionName("getMin", (int)(2 * sizeof(int)));
		const char* maxDecoratedString = getFullFunctionName("getMax", (int)(2 * sizeof(int)));
		const char* getCurrentProcessNameDecoratedString = 
			getFullFunctionName("getCurrentProcessName", 0);

		MinPtr * pointerToMin = (MinPtr *)::GetProcAddress(loadedDll, minDecoratedString);
		MaxPtr * pointerToMax = (MaxPtr *)::GetProcAddress(loadedDll, maxDecoratedString);
		GetCurrentProcessNamePtr *pointerToGetCurrentProcessName = (GetCurrentProcessNamePtr *)
			::GetProcAddress(loadedDll, getCurrentProcessNameDecoratedString);

		delete[] minDecoratedString;
		delete[] maxDecoratedString;
		delete[] getCurrentProcessNameDecoratedString;

		if ((pointerToMin == nullptr) || (pointerToMax == nullptr))
		{
			::printf("Error loading Min or Max functions\n");
		}
		else
		{
			int x, y;

			::printf_s("Enter x: ");
			::scanf_s("%d", &x);

			::printf_s("Enter y: ");
			::scanf_s("%d", &y);

			::printf_s("Minimum of x and y: %d\nMaximum of x and y: %d\n", 
				pointerToMin(x, y), pointerToMax(x, y));
		}

		if (pointerToGetCurrentProcessName == nullptr)
		{
			::printf("Error GetCurrentProcessName function\n");
		}
		else
		{
			const char* processName = pointerToGetCurrentProcessName();

			if (processName == nullptr)
			{
				::printf_s("Error getting current process name\n");
			}
			else
			{
				::printf_s("Current process name: %s\n", processName);
				delete[] processName;
			}
		}

		::FreeLibrary(loadedDll);
	}
	::system("pause");
}
