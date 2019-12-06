#include "SyncQueue.h"

SyncQueue::SyncQueue(int nSize) : size(nSize), front(-1), rear(-1), data(new int[nSize]) {
	InitializeCriticalSection(&csQueue);

	hEmptySem = CreateSemaphore(NULL, size, size, NULL);
	hFullSem = CreateSemaphore(NULL, 0, size, NULL);
}

SyncQueue::~SyncQueue() {
	CloseHandle(hFullSem);
	CloseHandle(hEmptySem);
	
	DeleteCriticalSection(&csQueue);

	delete[] data;
}

void SyncQueue::Insert(int nElement) {
	WaitForSingleObject(hEmptySem, INFINITE);

	EnterCriticalSection(&csQueue);

	if (rear == -1) // empty
		rear = front = 0;
	else
		rear = (rear + 1) % size;

	data[rear] = nElement;

	LeaveCriticalSection(&csQueue);

	ReleaseSemaphore(hFullSem, 1, NULL);
}

int SyncQueue::Remove() {
	WaitForSingleObject(hFullSem, INFINITE);

	EnterCriticalSection(&csQueue);

	int element = data[front];
	if (front == rear)
		front = rear = -1;
	else
		front = (front + 1) % size;

	LeaveCriticalSection(&csQueue);

	ReleaseSemaphore(hEmptySem, 1, NULL);

	return element;
}