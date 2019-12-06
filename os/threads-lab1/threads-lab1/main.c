
#include "polynom.h"

#include <windows.h>
#include <stdio.h>

int main() {
	struct polynom_entry numerator_polynomial, denominator_polynomial;
	int x;
	
	printf("Enter numerator polynomial\n");
	enter_polynom_from_console(&numerator_polynomial);
	printf("Enter denominator polynomial\n");
	enter_polynom_from_console(&denominator_polynomial);

	printf("Enter variable x: ");
	scanf("%d", &x);
	
	numerator_polynomial.x = x;
	denominator_polynomial.x = x;

	HANDLE hThread[2];
	DWORD dwThread[2];

	hThread[0] = CreateThread(NULL, 0, polynom, &numerator_polynomial, 0, &dwThread[0]);
	if (hThread[0] == NULL)
		return GetLastError();

	hThread[1] = CreateThread(NULL, 0, polynom, &denominator_polynomial, 0, &dwThread[1]);
	if (hThread[1] == NULL) 
		return GetLastError();

	if (WaitForMultipleObjects(2, hThread, TRUE, INFINITE) == WAIT_FAILED) 
		return GetLastError();

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	printf("f(%d) = %lf\n", x, (double)numerator_polynomial.result / denominator_polynomial.result);

	release_polynom(&numerator_polynomial);
	release_polynom(&denominator_polynomial);

	return 0;
}


