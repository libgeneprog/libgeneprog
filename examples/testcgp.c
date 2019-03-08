// SPDX-License-Identifier: BSD-3-Clause
//gcc -Wall -o testcgp.out testcgp.c -lgeneprog
#include "geneprog/cgp-data.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	//printf("Seeding RNG\n");
	//srand(time(NULL));

	printf("Making CGP Gene\n");
	struct GP_Gene *gene = GP_CGP_alloc(3, 3, 3);

	printf("Randomizing CGP Gene\n");
	GP_CGP_randomize(gene);

	printf("Printing CGP Gene\n");
	printf("%s\n",GP_CGP_as_debug_json(gene));

	double in[3] = {1, 2, 3};
	double out[3];

	printf("Evaluating gene\n");
	gene->evaluate(in, out, gene->data);

	printf("Output:\n");
	for (int i = 0; i < 3; i++)
		printf("\t%f\n", out[i]);

	printf("Destroying CGP Gene\n");
	GP_CGP_free(gene);

	return 1;
}
