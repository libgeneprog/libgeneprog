// SPDX-License-Identifier: BSD-3-Clause
//gcc -Wall -o testbst.out testbst.c -lgeneprog
#include "geneprog/bst-data.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	//printf("Seeding RNG\n");
	//srand(time(NULL));
	// Remove buffering so it's flushed immediately:
	setbuf(stdout, NULL);

	printf("Making BST Gene\n");
	struct GP_Gene *gene = GP_BST_alloc(3, 3, 3);

	printf("Randomizing BST Gene\n");
	GP_BST_randomize(gene);

	printf("Printing node\n");
	GP_BST_print(gene);

	double in[3] = {1, 2, 3};
	double out[3];

	printf("Evaluating gene\n");
	gene->evaluate(in, out, gene->data);

	printf("Output:\n");
	for (int i = 0; i < 3; i++)
		printf("\t%f\n", out[i]);

	printf("Destroying BST Gene\n");
	GP_BST_free(gene);

	return 1;
}
