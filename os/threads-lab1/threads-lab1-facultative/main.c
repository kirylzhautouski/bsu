#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <math.h>
#include <stdio.h>

// an*x^n + an-1*x^(n-1) + ... + a1*x + a0, n - degree
struct polynom_entry {
	int *coefficients; // an, an-1, ..., a1, a0
	unsigned degree;
	float x;
	double result;
};

struct addend {
	int coefficient;
	unsigned degree;
	float x;
	double result;
};

void print_polynom(struct polynom_entry* pe) {
	printf("%u %f %lf\n", pe->degree, pe->x, pe->result);

	for (unsigned i = 0; i <= pe->degree; i++) {
		printf("%d ", pe->coefficients[i]);
	}

	printf("\n");
}

void enter_polynom_from_console(struct polynom_entry* pe) {
	printf("Enter degree: ");
	scanf("%u", &pe->degree);

	pe->coefficients = (int*)malloc((pe->degree + 1) * sizeof(int));
	for (unsigned i = 0; i <= pe->degree; i++) {
		printf("Enter %u coefficient: ", i + 1);
		scanf("%u", pe->coefficients + i);
	}
}

DWORD WINAPI count_addend(LPVOID lpAddend) {
	struct addend* a = (struct addend*)lpAddend;
	a->result = a->coefficient * pow(a->x, a->degree);

	return 0;
}

DWORD WINAPI polynom(LPVOID lpPolynomEntry) {
	struct polynom_entry* pe = (struct polynom_entry*)lpPolynomEntry;
	pe->result = 0;

	HANDLE *hThreads = (HANDLE*)malloc((pe->degree + 1) * sizeof(HANDLE));
	DWORD *dwThreads = (DWORD*)malloc((pe->degree + 1) * sizeof(DWORD));
	struct addend* addends = (struct addend*)malloc((pe->degree + 1) * sizeof(struct addend));

	for (unsigned i = 0; i <= pe->degree; i++) {
		addends[i].coefficient = pe->coefficients[pe->degree - i];
		addends[i].degree = i;
		addends[i].x = pe->x;
		hThreads[i] = CreateThread(NULL, 0, count_addend, addends + i, 0, dwThreads + i);
		Sleep(15);
	}

	for (unsigned i = 0; i <= pe->degree; i++) {
		CloseHandle(hThreads[i]);

		pe->result += addends[i].result;
	}

	printf("p(%lf) = %lf\n", pe->x, pe->result);

	free(addends);
	free(dwThreads);
	free(hThreads);

	return 0;
}

int main() {
	struct polynom_entry numerator_polinomial, denominator_polinomial;
	float x;

	printf("Enter numerator polinomial\n");
	enter_polynom_from_console(&numerator_polinomial);
	printf("Enter denominator polinomial\n");
	enter_polynom_from_console(&denominator_polinomial);

	printf("Enter variable x: ");
	scanf("%f", &x);

	numerator_polinomial.x = x;
	denominator_polinomial.x = x;

	HANDLE hThread[2];
	DWORD dwThread[2];

	hThread[0] = CreateThread(NULL, 0, polynom, &numerator_polinomial, 0, &dwThread[0]);
	if (hThread[0] == NULL)
		return GetLastError();

	hThread[1] = CreateThread(NULL, 0, polynom, &denominator_polinomial, 0, &dwThread[1]);
	if (hThread[1] == NULL)
		return GetLastError();

	if (WaitForMultipleObjects(2, hThread, TRUE, INFINITE) == WAIT_FAILED)
		return GetLastError();

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	printf("f(%f) = %lf\n", x, numerator_polinomial.result / denominator_polinomial.result);

	free(numerator_polinomial.coefficients);
	free(denominator_polinomial.coefficients);

	return 0;
}


