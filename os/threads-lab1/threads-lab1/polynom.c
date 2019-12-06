
#include "polynom.h"

#include <stdio.h>
#include <math.h>

void enter_polynom_from_console(struct polynom_entry* pe) {
	printf("Enter degree: ");
	scanf("%u", &pe->degree);

	pe->coefficients = (int*)malloc((pe->degree + 1) * sizeof(int));

	printf("Enter %u coefficients: ", pe->degree + 1);
	for (unsigned i = 0; i <= pe->degree; i++)
		scanf("%u", pe->coefficients + i);
}

void release_polynom(struct polynom_entry* pe) {
	free(pe->coefficients);
}

DWORD WINAPI polynom(LPVOID lpPolynomEntry) {
	struct polynom_entry* pe = (struct polynom_entry*)lpPolynomEntry;
	
	pe->result = 0;
	for (unsigned i = 0; i <= pe->degree; i++) {
		pe->result += pe->coefficients[pe->degree - i] * (int)pow(pe->x, i);
		Sleep(15);
	}

	printf("p(%d) = %d\n", pe->x, pe->result);
	return 0;
}