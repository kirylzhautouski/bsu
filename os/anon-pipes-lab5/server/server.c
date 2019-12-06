#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string.h>

const int MAX_MESSAGES_COUNT = 20;
const int WAIT_INTERVAL_MS = 6000;

struct message {
	char sender[10];
	char receiver[10];
	char text[20];
};

int main() {
	int iNumberOfClients;

	printf("Enter number of clients: ");
	scanf("%d", &iNumberOfClients);
	getchar();

	HANDLE *hClientProcesses = malloc(iNumberOfClients * sizeof(HANDLE));
	
	HANDLE *hWriteInboxPipes = malloc(iNumberOfClients * sizeof(HANDLE));
	HANDLE *hReadOutboxPipes = malloc(iNumberOfClients * sizeof(HANDLE));

	HANDLE* hInboxFullSems = malloc(iNumberOfClients * sizeof(HANDLE));
	HANDLE* hInboxEmptySems = malloc(iNumberOfClients * sizeof(HANDLE));

	HANDLE* hOutboxFullSems = malloc(iNumberOfClients * sizeof(HANDLE));
	HANDLE* hOutboxEmptySems = malloc(iNumberOfClients * sizeof(HANDLE));

	char (*lpClientNames)[10] = malloc(iNumberOfClients * sizeof(char[10]));

	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	SECURITY_ATTRIBUTES sa;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(PROCESS_INFORMATION);

	for (int i = 0; i < iNumberOfClients; i++) {
		char lpClientName[11];
		char lpCommandLine[96];

		printf("Enter %d client name: ", i + 1);
		fgets(lpClientName, 11, stdin);
		lpClientName[strlen(lpClientName) - 1] = 0;
		sprintf(lpClientNames[i], "%s", lpClientName);

		HANDLE hReadInboxPipe, hReadInboxPipeInherit, hWriteOutboxPipe, hWriteOutboxPipeInherit;
		
		if (!CreatePipe(&hReadInboxPipe, &hWriteInboxPipes[i], NULL, 0))
			return GetLastError();
		
		if (!DuplicateHandle(GetCurrentProcess(), hReadInboxPipe, GetCurrentProcess(), &hReadInboxPipeInherit,
			0, TRUE, DUPLICATE_SAME_ACCESS))
			return GetLastError();

		CloseHandle(hReadInboxPipe);

		if (!CreatePipe(&hReadOutboxPipes[i], &hWriteOutboxPipe, NULL, 0))
			return GetLastError();
		
		if (!DuplicateHandle(GetCurrentProcess(), hWriteOutboxPipe, GetCurrentProcess(), &hWriteOutboxPipeInherit,
			0, TRUE, DUPLICATE_SAME_ACCESS))
			return GetLastError();

		CloseHandle(hWriteOutboxPipe);

		hInboxFullSems[i] = CreateSemaphore(&sa, 0, MAX_MESSAGES_COUNT, NULL);
		hInboxEmptySems[i] = CreateSemaphore(&sa, MAX_MESSAGES_COUNT, MAX_MESSAGES_COUNT, NULL);

		hOutboxFullSems[i] = CreateSemaphore(&sa, 0, MAX_MESSAGES_COUNT, NULL);
		hOutboxEmptySems[i] = CreateSemaphore(&sa, MAX_MESSAGES_COUNT, MAX_MESSAGES_COUNT, NULL);
		 
		sprintf(lpCommandLine, "client.exe %s %d %d %d %d %d %d", lpClientName, (int)hReadInboxPipeInherit, 
			(int)hWriteOutboxPipeInherit, (int)hInboxFullSems[i], (int)hInboxEmptySems[i], 
			(int)hOutboxFullSems[i], (int)hOutboxEmptySems[i]);
		
		if (!CreateProcess(NULL, lpCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, 
			NULL, NULL, &si, &piApp))
			return GetLastError();

		CloseHandle(piApp.hThread);

		CloseHandle(hWriteOutboxPipeInherit);
		CloseHandle(hReadInboxPipeInherit);

		hClientProcesses[i] = piApp.hProcess;
	}

	for (;;) {
		if (WaitForMultipleObjects(iNumberOfClients, hClientProcesses, TRUE, 0) == WAIT_OBJECT_0)
			break;
		
		DWORD dwWaitResult = WaitForMultipleObjects(iNumberOfClients, hOutboxFullSems, FALSE, 
			WAIT_INTERVAL_MS);

		if (dwWaitResult != WAIT_TIMEOUT) {
			int iSenderNumber = dwWaitResult - WAIT_OBJECT_0;

			struct message m;
			if (!ReadFile(hReadOutboxPipes[iSenderNumber], &m, sizeof(m), NULL, NULL))
				return GetLastError();

			ReleaseSemaphore(hOutboxEmptySems[iSenderNumber], 1, NULL);

			for (int i = 0; i < iNumberOfClients; i++) {
				if (strcmp(m.receiver, lpClientNames[i]) == 0) {
					WaitForSingleObject(hInboxEmptySems[i], INFINITE);

					if (!WriteFile(hWriteInboxPipes[i], &m, sizeof(m), NULL, NULL))
						return GetLastError();

					ReleaseSemaphore(hInboxFullSems[i], 1, NULL);
					break;
				}
			}
		}
	}

	for (int i = 0; i < iNumberOfClients; i++) {
		CloseHandle(hOutboxEmptySems[i]);
		CloseHandle(hOutboxFullSems[i]);

		CloseHandle(hInboxEmptySems[i]);
		CloseHandle(hInboxFullSems[i]);

		CloseHandle(hReadOutboxPipes[i]);
		CloseHandle(hWriteInboxPipes[i]);

		CloseHandle(hClientProcesses[i]);
	}

	free(lpClientNames);

	free(hOutboxEmptySems);
	free(hOutboxFullSems);

	free(hInboxEmptySems);
	free(hInboxFullSems);

	free(hReadOutboxPipes);
	free(hWriteInboxPipes);

	free(hClientProcesses);

	system("pause");
	return 0;
}