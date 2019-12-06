#ifndef POLYNOM_H_
#define POLYNOM_H_

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

// an*x^n + an-1*x^(n-1) + ... + a1*x + a0, n - degree
struct polynom_entry {
	int *coefficients; // an, an-1, ..., a1, a0
	unsigned degree;
	int x;
	int result;
};

void enter_polynom_from_console(struct polynom_entry* pe);

void release_polynom(struct polynom_entry* pe);

DWORD WINAPI polynom(LPVOID lpPolynomEntry);

#endif // POLYNOM_H_