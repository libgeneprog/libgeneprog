// SPDX-License-Identifier: BSD-3-Clause
//gcc -Wall -o testgenepool.out testgenepool.c -lgeneprog
#include "geneprog/genepool.h"
#include "geneprog/gene.h"
#include "geneprog/cgp-data.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct GP_Gene *_gp_cgp_gene_builder(void)
{
	printf("Building gene\n");
	// In, depth, out
	struct GP_Gene *gene = GP_CGP_alloc(1, 50, 1);
	GP_CGP_randomize(gene);
	printf("%s\n", gene->as_debug_json(gene));
	return gene;
}

double _gp_fitness(double **actual, double **expected)
{
	int num_rows = 10;
	double sum = 0;
	for (int idx = 0; idx < num_rows; idx++) {
		sum += fabs(actual[idx][0] - expected[idx][0]);
	}
	return sum;
}

int main(void)
{
	printf("Making genepool\n");
	struct GP_GenePool *genepool = GP_GenePool_alloc(50);
	printf("Populating genepool\n");
	GP_GenePool_genesis_with_builder(genepool, _gp_cgp_gene_builder);

	printf("Genepool:\n");
	printf("%s\n", GP_GenePool_as_debug_json(genepool));

	printf("Building evaluation data:\n");
	int num_rows = 10;
	double **in = malloc(sizeof(double *) * num_rows);
	double **expected_out = malloc(sizeof(double *) * num_rows);
	for (int idx = 0; idx < num_rows; idx++) {
		in[idx] = malloc(sizeof(double) * 1);
		expected_out[idx] = malloc(sizeof(double) * 1);
		in[idx][0] = idx / (10.0 / 2) - 1.0;
	}

	printf("Evaluating:\n");
	GP_GenePool_evaluate(genepool,
		       num_rows,
		       1,
		       in,
		       expected_out,
		       _gp_fitness);

	printf("Building next generation:\n");
	GP_GenePool_build_next_generation(genepool);

	printf("Running a couple generations:\n");
	for (int gen = 1; gen < 100; gen++) {
		printf("\tGen%i\n", gen);
		GP_GenePool_evaluate(genepool,
			       num_rows,
			       1,
			       in,
			       expected_out,
			       _gp_fitness);
		GP_GenePool_build_next_generation(genepool);
	}

}
