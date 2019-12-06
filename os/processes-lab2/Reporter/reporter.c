#define _CRT_SECURE_NO_WARNINGS

#include "tax_payment.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const struct tax_payment* lhs, const struct tax_payment* rhs) {
	if (lhs->sum > rhs->sum)
		return 1;
	else if (lhs->sum < rhs->sum)
		return -1;

	return 0;
}

void write_to_textfile(const struct tax_payment* p, FILE* f) {
	fprintf(f, "%d %s %.3lf\n", p->num, p->name, p->sum);
}

int main(int argc, char** argv) {
	char binary_input_file_name[256];
	char report_file_name[256];

	sscanf(argv[1], "%s", binary_input_file_name);
	sscanf(argv[2], "%s", report_file_name);

	double payment_threshold = strtod(argv[3], NULL);
	char comparator = argv[4][0];

	FILE* report_output = fopen(report_file_name, "w");
	FILE* binary_input = fopen(binary_input_file_name, "rb");

	char report_headline[263];
	sprintf(report_headline, "%s report\n", binary_input_file_name);
	fwrite(report_headline, sizeof(char), strlen(report_headline), report_output);

	int num_of_payments;
	fscanf(binary_input, "%d", &num_of_payments);

	struct tax_payment* payments = malloc(sizeof(struct tax_payment) * num_of_payments);
	fread(payments, sizeof(struct tax_payment), num_of_payments, binary_input);

	qsort(payments, num_of_payments, sizeof(struct tax_payment), compare);

	if (comparator == '<') {
		for (int i = 0; i < num_of_payments; i++) {
			if (payments[i].sum < payment_threshold)
				write_to_textfile(payments + i, report_output);
			else
				break;
		}
	}
	else if (comparator == '>') {
		for (int i = 0; i < num_of_payments; i++) {
			if (payments[i].sum > payment_threshold)
				write_to_textfile(payments + i, report_output);
		}
	}

	free(payments);

	fclose(binary_input);
	fclose(report_output);

	return 0;
}