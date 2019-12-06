#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>

#define MESSAGE_SIZE 16

struct TaxPayment {
	int num;
	char name[10];
	double sum;
};

int clientCount = 0;
CRITICAL_SECTION csClientCount;
HANDLE hAllClientsLeaveEvent;

int* readerCounts;
CRITICAL_SECTION* csRecords;
HANDLE* hWriteSems;

struct TaxPayment* pData;

DWORD WINAPI ConnectClients(LPVOID);
DWORD WINAPI ProcessClient(LPVOID);

void PrintFile(HANDLE hFile);

int main() {
	char fileName[256];
	printf("Enter a file name: ");
	fgets(fileName, 256, stdin);
	fileName[strlen(fileName) - 1] = 0;

	HANDLE hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("CreateFile failed, %d\n", GetLastError());
		return -1;
	}

	int recordCount;

	printf("Enter number of records: ");
	scanf("%d", &recordCount);
	getchar();

	for (int i = 0; i < recordCount; i++) {
		struct TaxPayment tp;

		printf("Enter number of %d tax payment: ", i + 1);
		scanf("%d", &tp.num);
		getchar();

		char name[11];

		printf("Enter %d tax payment company name: ", i + 1);
		fgets(name, 11, stdin);
		name[strlen(name) - 1] = 0;
		sprintf(tp.name, "%s", name);

		printf("Enter sum of %d tax payment: ", i + 1);
		scanf("%lf", &tp.sum);
		getchar();

		WriteFile(hFile, &tp, sizeof(struct TaxPayment), NULL, NULL);
	}

	PrintFile(hFile);

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		printf("GetFileSize failed, %d\n", GetLastError());
		return -1;
	}

	HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hMapping == NULL) {
		printf("CreateFileMapping failed, %d\n", GetLastError());
		return -1;
	}

	pData = (struct TaxPayment*)MapViewOfFile(hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, dwFileSize);

	if (pData == NULL) {
		printf("MapViewOfFile failed, %d\n", GetLastError());
		return -1;
	}

	readerCounts = calloc(recordCount, sizeof(int));
	csRecords = malloc(sizeof(CRITICAL_SECTION) * recordCount);
	hWriteSems = malloc(sizeof(HANDLE) * recordCount);

	for (int i = 0; i < recordCount; i++) {
		InitializeCriticalSection(&csRecords[i]);

		hWriteSems[i] = CreateSemaphore(NULL, 1, 1, NULL);
		if (hWriteSems[i] == NULL) {
			printf("CreateSemaphore failed, %d\n", GetLastError());
			return -1;
		}
	}

	InitializeCriticalSection(&csClientCount);
	hAllClientsLeaveEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE hConnectClientsThread = CreateThread(NULL, 0, ConnectClients, NULL, 0, NULL);
	if (hConnectClientsThread == NULL) {
		printf("CreateThread failed, %d\n", GetLastError());
		return -1;
	}

	WaitForSingleObject(hAllClientsLeaveEvent, INFINITE);
	
	UnmapViewOfFile(pData);
	CloseHandle(hMapping);

	PrintFile(hFile);

	printf("Press any key to terminate server: ");
	getchar();
	getchar();

	TerminateThread(hConnectClientsThread, 1);

	CloseHandle(hConnectClientsThread);

	CloseHandle(hAllClientsLeaveEvent);

	DeleteCriticalSection(&csClientCount);

	for (int i = 0; i < recordCount; i++) {
		CloseHandle(hWriteSems[i]);

		DeleteCriticalSection(&csRecords[i]);
	}

	free(hWriteSems);
	free(csRecords);
	free(readerCounts);

	CloseHandle(hFile);

	system("pause");

	return 0;
}

DWORD WINAPI ConnectClients(LPVOID lpvParam) {

	BOOL fConnected = FALSE;
	HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;
	LPTSTR lpszPipename = "\\\\.\\pipe\\tax_payment_pipe";

	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;

	for (;;) {
		hPipe = CreateNamedPipe(
			lpszPipename,
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE |
			PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			0,
			0,
			0,
			&sa
		);

		if (hPipe == INVALID_HANDLE_VALUE) {
			printf("CreateNamedFile failed, %d\n", GetLastError());
			return -1;
		}

		fConnected = ConnectNamedPipe(hPipe, NULL) ?
			TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (fConnected) {
			hThread = CreateThread(
				NULL,
				0,
				ProcessClient,
				(LPVOID)hPipe,
				0,
				NULL
			);
			
			if (hThread == NULL) {
				printf("CreateThread failed, %d\n", GetLastError());
				return -1;
			}

			EnterCriticalSection(&csClientCount);
			clientCount++;
			LeaveCriticalSection(&csClientCount);
		}
	}
}

DWORD WINAPI ProcessClient(LPVOID lpvParam) {
	if (lpvParam == NULL) {
		printf("Unexpected null parameter lpvParam in function ProcessClient\n");
		
		return -1;
	}

	char message[MESSAGE_SIZE];

	BOOL fSuccess = FALSE;
	HANDLE hPipe = (HANDLE)lpvParam;

	int numOfRecord = 0;

	for (;;) {
		if (!ReadFile(hPipe, message, MESSAGE_SIZE * sizeof(char), NULL, NULL)) {
			printf("ReadFile failed, %d\n", GetLastError());
			break;
		}

		sscanf(message, "%*s %d", &numOfRecord);
		numOfRecord--;

		if (strncmp(message, "READ", 4) == 0) {
			EnterCriticalSection(&csRecords[numOfRecord]);
			
			readerCounts[numOfRecord]++;
			if (readerCounts[numOfRecord] == 1)
				WaitForSingleObject(hWriteSems[numOfRecord], INFINITE);
			
			LeaveCriticalSection(&csRecords[numOfRecord]);

			if (!WriteFile(hPipe, pData + numOfRecord, sizeof(struct TaxPayment),
				NULL, NULL)) {
				printf("WriteFile failed, %d\n", GetLastError());
				break;
			}

			if (!ReadFile(hPipe, message, MESSAGE_SIZE * sizeof(char), NULL, NULL)) {
				printf("ReadFile failed, %d\n", GetLastError());
				break;
			}

			EnterCriticalSection(&csRecords[numOfRecord]);
			
			readerCounts[numOfRecord]--;
			if (readerCounts[numOfRecord] == 0)
				ReleaseSemaphore(hWriteSems[numOfRecord], 1, NULL);
			
			LeaveCriticalSection(&csRecords[numOfRecord]);
		}
		else if (strncmp(message, "WRITE", 5) == 0) {
			WaitForSingleObject(hWriteSems[numOfRecord], INFINITE);
			
			if (!WriteFile(hPipe, pData + numOfRecord, sizeof(struct TaxPayment),
				NULL, NULL)) {
				printf("WriteFile failed, %d\n", GetLastError());
				break;
			}

			if (!ReadFile(hPipe, pData + numOfRecord, sizeof(struct TaxPayment),
				NULL, NULL)) {
				printf("ReadFile failed, %d\n", GetLastError());
				break;
			}
			
			if (!ReadFile(hPipe, message, MESSAGE_SIZE * sizeof(char), NULL, NULL)) {
				printf("ReadFile failed, %d\n", GetLastError());
				break;
			}

			ReleaseSemaphore(hWriteSems[numOfRecord], 1, NULL);
		}
		else {
			break;
		}
	}

	// decrement number of clients and set event if no clients
	EnterCriticalSection(&csClientCount);

	clientCount--;
	if (clientCount == 0)
		SetEvent(hAllClientsLeaveEvent);

	LeaveCriticalSection(&csClientCount);

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 1;
}

void PrintFile(HANDLE hFile) {
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	int i = 1;
	struct TaxPayment tp;
	DWORD numOfBytesRead;

	printf("\nbinary file data:\n");

	while (ReadFile(hFile, &tp, sizeof(struct TaxPayment), &numOfBytesRead, NULL)) {
		if (numOfBytesRead == 0)
			break;

		printf("tax payment N%d { %d, %s, %lf }\n", i, tp.num, tp.name, tp.sum);

		i++;
	}
}