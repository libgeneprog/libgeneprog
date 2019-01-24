// SPDX-License-Identifier: BSD-3-Clause
#include "genepool.h"

#include <stdlib.h>
#include <assert.h>

struct GP_GenePool *GP_GenePool_alloc(unsigned int poolsize)
{
	assert(poolsize > 0);

	// Make the genepool:
	struct GP_GenePool *genepool = malloc(sizeof(struct GP_GenePool));

	genepool->poolsize = poolsize;
	genepool->genes = NULL;
	genepool->fitnesses = NULL;
	genepool->build_gene = NULL;

	return genepool;
}

void GP_GenePool_genesis_with_builder(struct GP_GenePool *genepool,
				      struct GP_Gene *(*builder)())
{
	// Set the builder function:
	genepool->build_gene = builder;
	// Call genesis:
	GP_GenePool_genesis(genepool);
}

void GP_GenePool_genesis(struct GP_GenePool *genepool)
{
	// Make sure the builder function is set:
	assert(genepool->build_gene != NULL);

	unsigned int poolsize = genepool->poolsize;

	// See if we need to allocate fitnesses:
	if (genepool->fitnesses == NULL)
		genepool->fitnesses = malloc(sizeof(unsigned int) * poolsize);

	// Allocate our genes:
	genepool->genes = malloc(sizeof(struct GP_Gene *) * poolsize);

	// Go through and initialize our genes:
	for (int idx = 0; idx < poolsize; idx++)
		(genepool->genes)[idx] = genepool->build_gene();

}

void GP_GenePool_evaluate(struct GP_GenePool *genepool,
			  unsigned int num_rows,
			  unsigned int num_outputs,
			  double **inputs,
			  double **expected_outputs,
			  double (*fitness)(double**, double**))
{
	// Make sure we have a pool first:
	assert(genepool->genes != NULL);

	// Build a structure to hold our outputs:
	double **actual_outputs = malloc(sizeof(double *) * num_rows);
	for (int idx = 0; idx < num_rows; idx++)
		actual_outputs[idx] = malloc(sizeof(double) * num_outputs);

	// Go through the pool and evaluate:
	for (int pool_idx = 0; pool_idx < genepool->poolsize; pool_idx++) {
		for (int row_idx = 0; row_idx < num_rows; row_idx++) {
			genepool->genes[pool_idx]->evaluate(
				inputs[row_idx],
				actual_outputs[row_idx],
				genepool->genes[pool_idx]->data);
		}
		genepool->fitnesses[pool_idx] = fitness(actual_outputs,
							expected_outputs);
	}

}
