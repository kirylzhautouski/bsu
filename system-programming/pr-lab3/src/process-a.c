#define _UNICODE

#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <TlHelp32.h>

#define INVALID_PROCESS_ID 0

const int MAX_STRING_LENGTH = 255;

HANDLE hMutex;

LPTSTR slotName = TEXT("\\\\.\\mailslot\\process_b_mailslot");
LPCSTR mutexName = "mutex";

DWORD GetProcessIdByExe(const wchar_t szExe[]);
BOOL WriteSlot(HANDLE hSlot, LPVOID lpszMessage);

int _tmain()
{
	_tprintf(TEXT("Enter string: "));
	TCHAR s[MAX_STRING_LENGTH];

	_fgetts(s, MAX_STRING_LENGTH, stdin);
	
	_tprintf(TEXT("%s"), s);
	DWORD processBId = GetProcessIdByExe(L"process-b.exe");
	
	if (processBId == (INVALID_PROCESS_ID))
	{
		_tprintf(TEXT("Can't find process"));
		return 1;
	}

	HANDLE hProcessB = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, 
		processBId);

	if (hProcessB == (INVALID_HANDLE_VALUE))
	{
		_tprintf(TEXT("Can't open process"));
		return 2;
	}

	LPVOID address = VirtualAllocEx(hProcessB, NULL, sizeof(TCHAR) * MAX_STRING_LENGTH, MEM_COMMIT | MEM_RESERVE, 
		PAGE_EXECUTE_READWRITE);

	if (address == NULL)
	{
		_tprintf(TEXT("Can't allocate memory"));
		CloseHandle(hProcessB);

		return 3;
	}

	BOOL writeResult = WriteProcessMemory(hProcessB, address, s, sizeof(TCHAR) * MAX_STRING_LENGTH, NULL);

	if (writeResult == FALSE)
	{
		_tprintf(TEXT("Can't write string"));
		CloseHandle(hProcessB);

		return 4;
	}

	// send to B
	HANDLE hFile = CreateFile(slotName, GENERIC_WRITE, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES) NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL); 

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("CreateFile failed"));
		CloseHandle(hProcessB);

		return 5;
	}

	WriteSlot(hFile, address);

	CloseHandle(hFile);
	
	hMutex = CreateMutexA(NULL, FALSE, mutexName);
	if (hMutex == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("CreateMutexA failed"));
		CloseHandle(hProcessB);

		return 6;
	}

	DWORD waitResult = WaitForSingleObject(hMutex, INFINITE);

	if (waitResult == WAIT_OBJECT_0)
	{
		BOOL freeResult = VirtualFreeEx(hProcessB, address, 0, MEM_RELEASE);

		if (freeResult == FALSE)
		{
			_tprintf(TEXT("Can't release memory"));
			CloseHandle(hProcessB);
			CloseHandle(hMutex);

			return 7;
		}
	}

	CloseHandle(hMutex);
	CloseHandle(hProcessB);
	return 0;
}

DWORD GetProcessIdByExe(const wchar_t szExe[])
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return (INVALID_PROCESS_ID);
	
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return (INVALID_PROCESS_ID);
	}

	do
	{
		if (!wcscmp(pe32.szExeFile, szExe))
			return pe32.th32ProcessID;
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return (INVALID_PROCESS_ID);
}

BOOL WriteSlot(HANDLE hSlot, LPVOID address)
{
	DWORD cbWritten;
	BOOL writeResult = WriteFile(hSlot, &address, sizeof(address),
		&cbWritten, (LPOVERLAPPED)NULL);

	return writeResult;
}