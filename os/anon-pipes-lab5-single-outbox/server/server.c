#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string.h>

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
	
	HANDLE hReadOutboxPipe, hWriteOutboxPipe, hWriteOutboxPipeInherit;
	if (!CreatePipe(&hReadOutboxPipe, &hWriteOutboxPipe, NULL, 0)) 
		return GetLastError();

	if (!DuplicateHandle(GetCurrentProcess(), hWriteOutboxPipe, GetCurrentProcess(), &hWriteOutboxPipeInherit,
		0, TRUE, DUPLICATE_SAME_ACCESS))
		return GetLastError();

	CloseHandle(hWriteOutboxPipe);

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

		HANDLE hReadInboxPipe, hReadInboxPipeInherit;
		
		if (!CreatePipe(&hReadInboxPipe, &hWriteInboxPipes[i], NULL, 0))
			return GetLastError();
		
		if (!DuplicateHandle(GetCurrentProcess(), hReadInboxPipe, GetCurrentProcess(), &hReadInboxPipeInherit,
			0, TRUE, DUPLICATE_SAME_ACCESS))
			return GetLastError();

		CloseHandle(hReadInboxPipe);

		sprintf(lpCommandLine, "client.exe %s %d %d", lpClientName, (int)hReadInboxPipeInherit, 
			(int)hWriteOutboxPipeInherit);
		
		if (!CreateProcess(NULL, lpCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, 
			NULL, NULL, &si, &piApp))
			return GetLastError();

		CloseHandle(piApp.hThread);

		CloseHandle(hReadInboxPipeInherit);

		hClientProcesses[i] = piApp.hProcess;
	}

	CloseHandle(hWriteOutboxPipeInherit);

	struct message m;
	while (ReadFile(hReadOutboxPipe, &m, sizeof(struct message), NULL, NULL)) {
		for (int i = 0; i < iNumberOfClients; i++) {
			if (strcmp(m.receiver, lpClientNames[i]) == 0) {
				if (!WriteFile(hWriteInboxPipes[i], &m, sizeof(m), NULL, NULL))
					return GetLastError();
			
				break;
			}
		}
	}

	CloseHandle(hReadOutboxPipe);

	for (int i = 0; i < iNumberOfClients; i++) {
		CloseHandle(hWriteInboxPipes[i]);

		CloseHandle(hClientProcesses[i]);
	}

	free(lpClientNames);

	free(hWriteInboxPipes);

	free(hClientProcesses);

	system("pause");

	return 0;
}