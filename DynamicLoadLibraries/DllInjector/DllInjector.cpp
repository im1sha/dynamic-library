#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#define INVALID_RESULT -1
#define INVALID_HANDLE 0


#include <windows.h>
#include <stdio.h>
#include <vector>
#include <tlhelp32.h>

DWORD getProcessByName(const wchar_t* searchProc)
{
	DWORD result = INVALID_HANDLE;

	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);
	::Process32First(handle, &ProcEntry);

	do
	{
		if (::wcscmp(ProcEntry.szExeFile, searchProc) == 0) 
		{
			result = ProcEntry.th32ProcessID;
			break;
		}
	} while (::Process32Next(handle, &ProcEntry));

	::CloseHandle(handle);

	return result;
}

int wmain()
{
	const wchar_t* dllName = L"InjectionDll.dll";
	const wchar_t* exeToEnjectName = L"InjectionAndReplacementDemo.exe";

	DWORD processId = INVALID_HANDLE;
	int iterationsLeft = 10;
	const DWORD timeout = 500;

	while (processId == INVALID_HANDLE)
	{
		iterationsLeft--;
		if (iterationsLeft == 0)
		{
			printf("Cannot find such the process\n");
			return INVALID_RESULT;
		}

		processId  = getProcessByName(exeToEnjectName);
		
		if (processId == INVALID_HANDLE)
		{
			::Sleep(timeout);
		}
		else
		{
			break;
		}		
	}

	HANDLE injectedProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	if (injectedProcess == nullptr)
	{
		::printf("Error opening process with ID %d\n", processId);
	}
	else
	{		
		int dllNameLength = ::lstrlen(dllName);
		void* injectionLibConatiner = ::VirtualAllocEx(injectedProcess, nullptr,
			(dllNameLength + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

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

	::printf("Succeeded\n");
	::system("pause");
	return 0;
}
