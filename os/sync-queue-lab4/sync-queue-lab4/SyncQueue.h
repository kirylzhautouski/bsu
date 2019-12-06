#pragma once

#include <windows.h>

class SyncQueue
{
public:
	SyncQueue(int nSize);
	~SyncQueue();
	void Insert(int nElement);
	int Remove();
private:
	int size;
	
	int front;
	int rear;

	int* data;

	CRITICAL_SECTION csQueue;

	HANDLE hEmptySem;
	HANDLE hFullSem;

};

