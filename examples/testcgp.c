//gcc -Wall -o testcgp testcgp.c -lgeneprog
#include "geneprog/cgp-data.h"

#include <stdio.h>

int main()
{
	printf("Making CGP Gene\n");
	GP_Gene* gene = GP_CGP_alloc(3, 3, 3);

	double in[3];
	double out[3];

	printf("Evaluating gene\n");
	gene->evaluate(in, out, gene->data);

	printf("Output:\n");
	for(int i=0; i<3; i++){
		printf("\t%f\n", out[i]);
	}

	printf("Destroying CGP Gene\n");
	GP_CGP_free(gene);

	return 1;
}
