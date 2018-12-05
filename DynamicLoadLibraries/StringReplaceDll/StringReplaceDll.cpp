#include "StringReplaceDll.h"

__declspec(dllexport) int replaceString(const wchar_t* toReplace, const wchar_t* newString)
{
	int replacementLength = ::lstrlen(toReplace);
	if ((replacementLength == 0) || (replacementLength != ::lstrlen(newString)))
	{
		return -1;
	}

	HANDLE currentProcess = ::GetCurrentProcess();
	SYSTEM_INFO systemInfo;
	::GetSystemInfo(&systemInfo);

	DWORD pageSizeInWchars = systemInfo.dwPageSize / sizeof(wchar_t);

	wchar_t *pageContent = (wchar_t *)::calloc(pageSizeInWchars, sizeof(wchar_t));

	DWORD lastPageSymbol = pageSizeInWchars - (DWORD)replacementLength;

	MEMORY_BASIC_INFORMATION pageInfo;
	bool isStringFound;
	bool shouldWritePage;
	int totalReplaced = 0;

	for (void* pageAddress = systemInfo.lpMinimumApplicationAddress; 
		pageAddress < systemInfo.lpMaximumApplicationAddress; 
		pageAddress = (LPVOID)((ULONG_PTR)pageAddress 
			+ (ULONG_PTR)systemInfo.dwPageSize))
	{
		::VirtualQuery(pageAddress, &pageInfo, sizeof(MEMORY_BASIC_INFORMATION));

		if ((pageInfo.State == MEM_COMMIT) &&
			(pageInfo.Protect & PAGE_READWRITE))
		{
			if (::ReadProcessMemory(currentProcess, pageAddress, pageContent,
				systemInfo.dwPageSize, nullptr))
			{
				shouldWritePage = false;

				for (DWORD j = 0; j < lastPageSymbol; j++)
				{
					isStringFound = true;
					for (int i = 0; (i < replacementLength) && isStringFound; i++)
					{
						isStringFound = (pageContent[j + i] 
							== toReplace[i]);
					}

					if (isStringFound)
					{
						for (int i = 0; i < replacementLength; i++)
						{
							pageContent[j + i] = newString[i];
						}
						totalReplaced++;
						shouldWritePage = true;
					}
				}

				if (shouldWritePage)
				{
					::WriteProcessMemory(currentProcess, pageAddress, pageContent,
						systemInfo.dwPageSize, nullptr);
				}
			}
		}
	}

	::free(pageContent);
	return totalReplaced;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}
