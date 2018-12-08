#include <windows.h>
#include <stdio.h>

typedef int (__stdcall MinPtr) (int x, int y);

typedef int (__stdcall MaxPtr) (int x, int y);

typedef char* (__stdcall GetCurrentProcessNamePtr) ();

char* getFullFunctionName(const char* functionName, int argumentsSize)
{
	const int decorationCharsLength = 2; // ( _ , @ )
	
	int nameLength = ::lstrlen(functionName) + decorationCharsLength;

	int sizeCounter = argumentsSize;
	do
	{
		sizeCounter /= 10; 
		nameLength++;
	} while (sizeCounter > 0); // count total chars required by "%d" of "_%s@%d"

	char* result = new char[nameLength + 1];
	if (::sprintf_s(result, nameLength + 1, "_%s@%d", functionName, argumentsSize) == -1)
	{
		delete[] result;
		return nullptr;
	}
	
	return result;	
}

int main()
{
	const char* dllPath = "DllExample.dll";

	HMODULE loadedDll = ::LoadLibrary(dllPath);

	if (loadedDll == nullptr)
	{
		::printf("Error loading library %s\n", dllPath);
	}
	else
	{
		const char* minDecoratedString = getFullFunctionName("getMin", (int)(2 * sizeof(int)));
		const char* maxDecoratedString = getFullFunctionName("getMax", (int)(2 * sizeof(int)));
		const char* getCurrentProcessNameDecoratedString = 
			getFullFunctionName("getCurrentProcessName", 0);

		MinPtr * pointerToMin = (MinPtr *) ::GetProcAddress(loadedDll, minDecoratedString);
		MaxPtr * pointerToMax = (MaxPtr *) ::GetProcAddress(loadedDll, maxDecoratedString);
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

			::printf("Enter x: ");
			::scanf_s("%d", &x);

			::printf("Enter y: ");
			::scanf_s("%d", &y);

			::printf("Minimum of x and y: %d\nMaximum of x and y: %d\n", 
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
				::printf("Error getting current process name\n");
			}
			else
			{
				::printf("Current process name: %s\n", processName);
				delete[] processName;
			}
		}

		::FreeLibrary(loadedDll);
	}
	::system("pause");
}
