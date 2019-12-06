#define _CRT_SECURE_NO_WARNINGS

#include "tax_payment.h"

#include <stdio.h>
#include <windows.h>

int main() {
	char binary_file_name[256];

	printf("Enter binary file name: ");
	scanf("%s", binary_file_name);

	int num_of_queries;
	printf("Enter number of queries: ");
	scanf("%d", &num_of_queries);

	STARTUPINFO siCreator;
	PROCESS_INFORMATION piCreator;

	ZeroMemory(&siCreator, sizeof(STARTUPINFO));
	siCreator.cb = sizeof(STARTUPINFO);

	char lpCommandLineCreator[274];
	sprintf(lpCommandLineCreator, "Creator.exe %s %d", binary_file_name, num_of_queries);

	if (!CreateProcess(NULL, lpCommandLineCreator, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siCreator, &piCreator)) 
		return GetLastError();
	
	WaitForSingleObject(piCreator.hProcess, INFINITE);

	CloseHandle(piCreator.hThread);
	CloseHandle(piCreator.hProcess);

	FILE* binary_input = fopen(binary_file_name, "rb");

	fscanf(binary_input, "%d", &num_of_queries);
	for (int i = 0; i < num_of_queries; i++) {
		struct tax_payment payment;
		fread(&payment, sizeof(struct tax_payment), 1, binary_input);

		printf("%d %s %lf\n", payment.num, payment.name, payment.sum);
	}

	fclose(binary_input);

	char report_file_name[256];

	printf("Enter report file name: ");
	scanf("%s", report_file_name);

	double payment_threshold;

	printf("Enter payment threshold: ");
	scanf("%lf", &payment_threshold);

	char comparator;

	printf("Enter comparator: ");
	scanf(" %c", &comparator);

	STARTUPINFO siReporter;
	PROCESS_INFORMATION piReporter;

	ZeroMemory(&siReporter, sizeof(STARTUPINFO));
	siReporter.cb = sizeof(STARTUPINFO);

	char lpCommandLineReporter[540];
	sprintf(lpCommandLineReporter, "Reporter.exe %s %s %lf %c", binary_file_name, report_file_name, 
		payment_threshold, comparator);

	if (!CreateProcess(NULL, lpCommandLineReporter, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siReporter, &piReporter))
		return GetLastError();

	WaitForSingleObject(piReporter.hProcess, INFINITE);

	CloseHandle(piReporter.hThread);
	CloseHandle(piReporter.hProcess);

	FILE* report_file = fopen(report_file_name, "r");

	char line[100];
	while (fgets(line, 100, report_file) != NULL)
		printf("%s", line);

	fclose(report_file);

	return 0;
}