#define _CRT_SECURE_NO_WARNINGS

#include "tax_payment.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	FILE* output_file = fopen(argv[1], "wb");

	int num_of_payments = atoi(argv[2]);
	fprintf(output_file, "%d", num_of_payments);

	struct tax_payment* payments = malloc(sizeof(struct tax_payment) * num_of_payments);
	for (int i = 0; i < num_of_payments; i++) {
		printf("Enter company id, name and sum of taxes: ");
		scanf("%d %s %lf", &payments[i].num, payments[i].name, &payments[i].sum);
	}
	  
	fwrite(payments, sizeof(struct tax_payment), num_of_payments, output_file);

	free(payments);
	fclose(output_file);

	return 0;
}