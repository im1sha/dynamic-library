#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <windows.h>
#include <stdio.h>

int wmain()
{
	DWORD processId;
	::printf_s("Enter process ID: ");
	::scanf_s("%d", &processId);

	HANDLE injectedProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (injectedProcess == nullptr)
	{
		::printf("Error opening process with ID %d\n", processId);
	}
	else
	{		
		const wchar_t* dllName = L"InjectionDll.dll";
		int dllNameLength = ::lstrlen(dllName);
		void* injectionLibConatiner = ::VirtualAllocEx(injectedProcess, nullptr,
			(dllNameLength + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);

		if (injectionLibConatiner == nullptr)
		{
			::printf("Error creating memory page in injected process\n");
		}
		else
		{
			if (::WriteProcessMemory(injectedProcess, injectionLibConatiner,
				dllName, (dllNameLength + 1) * sizeof(wchar_t), nullptr))
			{
				if (::CreateRemoteThread(injectedProcess, nullptr, 0,
					(LPTHREAD_START_ROUTINE)::LoadLibrary, 
					injectionLibConatiner, 0, nullptr) == nullptr)
				{
					::printf("Error creating remote thread\n");
				}
			}
			else
			{
				::printf("Error writing in remote process memory page\n");
			}
		}
		::CloseHandle(injectedProcess);
	}

	::system("pause");
	return 0;
}
