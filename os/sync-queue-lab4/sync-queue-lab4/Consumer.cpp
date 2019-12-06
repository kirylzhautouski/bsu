#include <iostream>
#include <windows.h>

#include "Consumer.h"
#include "ThreadParams.h"

using namespace std;

DWORD WINAPI Consumer(LPVOID lpParameters) {
	ThreadParams* threadParams = (ThreadParams*)lpParameters;

	SyncQueue* syncQueue = threadParams->syncQueue;

	CRITICAL_SECTION* pcsIO = threadParams->pcsIO;

	int numberCount;

	EnterCriticalSection(pcsIO);
	wcout << L"¬ведите количество потребл€емых чисел: ";
	wcin >> numberCount;
	LeaveCriticalSection(pcsIO);

	SetEvent(threadParams->hEventReady);

	WaitForSingleObject(threadParams->hEventStart, INFINITE);

	for (int i = 0; i < numberCount; i++) {
		int number = syncQueue->Remove();

		EnterCriticalSection(pcsIO);
		wcout << L"\t”потреблено число: " << number << endl;
		LeaveCriticalSection(pcsIO);

		Sleep(15);
	}

	return 0;
}