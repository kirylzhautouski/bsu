#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>

#define MESSAGE_LENGTH 20

const char* event_name_format = "ev-sender%d-ready";
const char* mutex_name = "mtx-binary-file";
const char* empty_semaphore_name = "sem-empty";
const char* full_semaphore_name = "sem-full";

const int number_of_messages = 3;

int main() {
	char binary_file_name[256];

	printf("Enter name of binary file: ");
	scanf("%s", binary_file_name);
	
	FILE* binary_file = fopen(binary_file_name, "w+b");

	int number_of_senders;
	printf("Enter number of senders: ");
	scanf("%d%*c", &number_of_senders);

	STARTUPINFO siCreator;
	PROCESS_INFORMATION piCreator;

	ZeroMemory(&siCreator, sizeof(STARTUPINFO));
	siCreator.cb = sizeof(STARTUPINFO);

	HANDLE *hEvents = malloc(number_of_senders * sizeof(HANDLE));

	HANDLE hMutex = CreateMutex(NULL, FALSE, mutex_name);
	if (hMutex == NULL)
		return GetLastError();

	HANDLE hEmptySemaphore = CreateSemaphore(NULL, number_of_messages, number_of_messages, 
		empty_semaphore_name);
	if (hEmptySemaphore == NULL)
		return GetLastError();

	HANDLE hFullSemaphore = CreateSemaphore(NULL, 0, number_of_messages, full_semaphore_name);
	if (hFullSemaphore == NULL)
		return GetLastError();

	char lpEventName[20];
	char lpCommandLineCreator[272];

	for (int i = 0; i < number_of_senders; i++) {
		sprintf(lpEventName, event_name_format, i);

		hEvents[i] = CreateEvent(NULL, FALSE, FALSE, lpEventName);
		if (hEvents[i] == NULL)
			return GetLastError();
		
		sprintf(lpCommandLineCreator, "sender.exe %s %d", binary_file_name, i);
		if (!CreateProcess(NULL, lpCommandLineCreator, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
			NULL, NULL, &siCreator, &piCreator))
			return GetLastError();
	
		CloseHandle(piCreator.hThread);
		CloseHandle(piCreator.hProcess);
	}
	
	printf("Waiting for signals\n");

	WaitForMultipleObjects(number_of_senders, hEvents, TRUE, INFINITE);

	printf("Received all signals\n");

	for (int i = 0; i < number_of_senders; i++)
		CloseHandle(hEvents[i]);

	free(hEvents);

	char operation;
	char message[MESSAGE_LENGTH];

	int front = 0, rear = 0;
	fwrite(&rear, sizeof(int), 1, binary_file);
	fflush(binary_file);

	for (;;) {
		printf("Input 'r' to read message from file or 'q' to quit: ");
		scanf("%c%*c", &operation);

		if (operation == 'r') {
			// full.wait(); уменьшить кол-во продуктов на складе 
			WaitForSingleObject(hFullSemaphore, INFINITE); 
			
			// mutex.acquire();  вход в критическую секцию 
			WaitForSingleObject(hMutex, INFINITE);

			// getProduct();  взять продукт со склада
			fseek(binary_file, sizeof(int) + sizeof(char) * MESSAGE_LENGTH * front, SEEK_SET); 
			fread(message, sizeof(char), MESSAGE_LENGTH, binary_file);
			
			front++;
			front %= number_of_messages;

			// mutex.release(); выход из критической секции 
			ReleaseMutex(hMutex);

			// empty.signal();  увеличить кол-во своб. ячеек на склад
			ReleaseSemaphore(hEmptySemaphore, 1, NULL);

			// consumeProduct(); потребить продукт
			printf("\nMessage: %s\n\n", message);
		}
		else if (operation == 'q')
			break;
	}

	CloseHandle(hFullSemaphore);
	CloseHandle(hEmptySemaphore);
	CloseHandle(hMutex);

	fclose(binary_file);

	return 0;
}