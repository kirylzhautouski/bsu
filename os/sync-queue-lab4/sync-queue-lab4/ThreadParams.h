#include <windows.h>

#include "SyncQueue.h"

struct ThreadParams {
	HANDLE hEventReady;
	HANDLE hEventStart;
	
	CRITICAL_SECTION* pcsIO;

	SyncQueue* syncQueue;
};