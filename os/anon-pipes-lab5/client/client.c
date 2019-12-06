#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>

struct message {
	char sender[10];
	char receiver[10];
	char text[20];
};

int main(int argc, char* argv[]) {
	const char* lpcClientName = argv[1];

	HANDLE hReadInboxPipe = (HANDLE)atoi(argv[2]);
	HANDLE hWriteOutboxPipe = (HANDLE)atoi(argv[3]);

	HANDLE hInboxFullSem = (HANDLE)atoi(argv[4]);
	HANDLE hInboxEmptySem = (HANDLE)atoi(argv[5]);

	HANDLE hOutboxFullSem = (HANDLE)atoi(argv[6]);
	HANDLE hOutboxEmptySem = (HANDLE)atoi(argv[7]);

	struct message m;
	sprintf(m.sender, "%s", argv[1]);

	printf("Client %hs\n\n", m.sender);

	for (;;) {
		char operation;
		
		printf("Enter 's' to send a message and read messages, 'r' to read messages, 'q' to exit: ");
		scanf("%c", &operation);
		getwchar();

		if (operation == 'q')
			break;

		if (operation == 's') {
			char receiver[11];

			printf("Enter receiver name: ");
			fgets(receiver, 11, stdin);
			receiver[strlen(receiver) - 1] = 0;

			sprintf(m.receiver, "%s", receiver);

			char lpcMessageText[21];

			printf("Enter message: ");
			fgets(lpcMessageText, 21, stdin);
			lpcMessageText[strlen(lpcMessageText) - 1] = 0;

			sprintf(m.text, "%s", lpcMessageText);

			WaitForSingleObject(hOutboxEmptySem, INFINITE);

			DWORD dwBytesWritten;
			if (!WriteFile(hWriteOutboxPipe, &m, sizeof(m), &dwBytesWritten, NULL)) {
				printf("%d", GetLastError());

				return GetLastError();
			}

			if (!ReleaseSemaphore(hOutboxFullSem, 1, NULL)) {
				printf("%d", GetLastError());

				return GetLastError();
			}
		}
		
		while (WaitForSingleObject(hInboxFullSem, 0) == WAIT_OBJECT_0) {
			struct message inboxMessage;
			DWORD dwBytesRead;
			
			if (!ReadFile(hReadInboxPipe, &inboxMessage, sizeof(inboxMessage), &dwBytesRead, NULL)) {
				printf("%d", GetLastError());

				return GetLastError();
			}

			ReleaseSemaphore(hInboxEmptySem, 1, NULL);

			printf("\n%s\n%s\n\n", inboxMessage.sender, inboxMessage.text);
		}
	}

	CloseHandle(hOutboxEmptySem);
	CloseHandle(hOutboxFullSem);

	CloseHandle(hInboxEmptySem);
	CloseHandle(hInboxFullSem);

	CloseHandle(hWriteOutboxPipe);
	CloseHandle(hReadInboxPipe);

	system("pause");
}