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

	struct message m;
	sprintf(m.sender, "%s", argv[1]);

	printf("Client %s\n\n", m.sender);

	for (;;) {
		char operation;
		
		printf("Enter 's' to send a message and read messages, 'r' to read messages, 'q' to exit: ");
		scanf("%c", &operation);
		getchar();

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

			DWORD dwBytesWritten;
			if (!WriteFile(hWriteOutboxPipe, &m, sizeof(m), &dwBytesWritten, NULL))
				return GetLastError();
		}
		
		DWORD dwBytesAvail;

		do {
			if (!PeekNamedPipe(hReadInboxPipe, NULL, 0, NULL, &dwBytesAvail, NULL))
				return GetLastError();

			if (dwBytesAvail > 0) {
				struct message inboxMessage;
				DWORD dwBytesRead;

				if (!ReadFile(hReadInboxPipe, &inboxMessage, sizeof(inboxMessage), &dwBytesRead, NULL))
					return GetLastError();

				printf("\nMessage from: %s\n%s\n\n", inboxMessage.sender, inboxMessage.text);
			}
			else
				break;

		} while (TRUE);
	}

	CloseHandle(hWriteOutboxPipe);
	CloseHandle(hReadInboxPipe);

	system("pause");

	return 0;
}