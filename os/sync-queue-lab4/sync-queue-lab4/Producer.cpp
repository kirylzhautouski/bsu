#include <iostream>
#include <windows.h>

#include "Producer.h"
#include "ThreadParams.h"

using namespace std;

DWORD WINAPI Producer(LPVOID lpParameters) {
	ThreadParams* threadParams = (ThreadParams*)lpParameters;
	
	SyncQueue* syncQueue = threadParams->syncQueue;

	CRITICAL_SECTION* pcsIO = threadParams->pcsIO;

	int numberCount, number;

	EnterCriticalSection(pcsIO);
	wcout << L"Введите количество производимых чисел и число через пробел: ";
	wcin >> numberCount >> number;
	LeaveCriticalSection(pcsIO);

	SetEvent(threadParams->hEventReady);

	WaitForSingleObject(threadParams->hEventStart, INFINITE);

	for (int i = 0; i < numberCount; i++) {
		syncQueue->Insert(number);

		EnterCriticalSection(pcsIO);
		wcout << L"Произведено число: " << number << endl;
		LeaveCriticalSection(pcsIO);

		Sleep(15);
	}

	return 0;
}