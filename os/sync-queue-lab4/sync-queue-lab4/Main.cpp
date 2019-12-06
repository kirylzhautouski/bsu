#include <string>
#include <iostream>
#include <windows.h>

#include "SyncQueue.h"

#include "Consumer.h"
#include "Producer.h"
#include "ThreadParams.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	int queueSize;
	wcout << L"Введите размер очереди: ";
	wcin >> queueSize;

	SyncQueue syncQueue(queueSize);

	int producerCount, consumerCount;
	wcout << L"Введите число производителей и число потребителей через пробел: ";
	wcin >> producerCount >> consumerCount;

	ThreadParams* threadsParams = new ThreadParams[producerCount + consumerCount];

	HANDLE* hThreads = new HANDLE[producerCount + consumerCount];
	HANDLE* hEventsReady = new HANDLE[producerCount + consumerCount];

	HANDLE hEventStart = CreateEvent(NULL, TRUE, FALSE, NULL);

	CRITICAL_SECTION csIO;
	InitializeCriticalSection(&csIO);

	for (int i = 0; i < producerCount; i++) {
		hEventsReady[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hEventsReady[i] == NULL)
			return GetLastError();

		threadsParams[i].hEventReady = hEventsReady[i];
		threadsParams[i].hEventStart = hEventStart;

		threadsParams[i].syncQueue = &syncQueue;

		threadsParams[i].pcsIO = &csIO;

		DWORD IDThread;

		hThreads[i] = CreateThread(NULL, 0, Producer, &threadsParams[i], 0, &IDThread);
		if (hThreads[i] == NULL)
			return GetLastError();
	}

	for (int i = producerCount; i < producerCount + consumerCount; i++) {
		hEventsReady[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hEventsReady[i] == NULL)
			return GetLastError();

		threadsParams[i].hEventReady = hEventsReady[i];
		threadsParams[i].hEventStart = hEventStart;

		threadsParams[i].syncQueue = &syncQueue;

		threadsParams[i].pcsIO = &csIO;

		DWORD IDThread;

		hThreads[i] = CreateThread(NULL, 0, Consumer, &threadsParams[i], 0, &IDThread);
		if (hThreads[i] == NULL)
			return GetLastError();
	}

	WaitForMultipleObjects(producerCount + consumerCount, hEventsReady, TRUE, INFINITE);

	SetEvent(hEventStart);

	WaitForMultipleObjects(producerCount + consumerCount, hThreads, TRUE, INFINITE);

	DeleteCriticalSection(&csIO);

	CloseHandle(hEventStart);

	for (int i = 0; i < producerCount + consumerCount; i++) {
		CloseHandle(hEventsReady[i]);
		CloseHandle(hThreads[i]);
	}

	delete[] hEventsReady;
	delete[] threadsParams;
	delete[] hThreads;

	system("pause");

	return 0;
}