// SPDX-License-Identifier: BSD-3-Clause
#include "genepool.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct GP_GenePool *GP_GenePool_alloc(int poolsize)
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

	int poolsize = genepool->poolsize;

	// See if we need to allocate fitnesses:
	if (genepool->fitnesses == NULL)
		genepool->fitnesses = malloc(sizeof(double) * poolsize);

	// Allocate our genes:
	genepool->genes = malloc(sizeof(struct GP_Gene *) * poolsize);

	// Go through and initialize our genes:
	for (int idx = 0; idx < poolsize; idx++)
		genepool->genes[idx] = genepool->build_gene();

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

struct GP_Gene* _GP_GenePool_tournament_select(struct GP_GenePool *genepool,
					       int tournament_size)
{
	int poolsize = genepool->poolsize;
	double best_fitness;
	int best_idx = -1;

	for (int pos = 0; pos < tournament_size; pos++) {
		// Pick a random gene:
		int pool_idx = rand() % poolsize;
		double pool_fit = genepool->fitnesses[pool_idx];
		// Is it better than our best gene?
		if (best_idx == -1 || pool_fit < best_fitness) {
			best_fitness = pool_fit;
			best_idx = pool_idx;
		}
	}

	return genepool->genes[best_idx];
}

void GP_GenePool_build_next_generation(struct GP_GenePool *genepool)
{
	// Set up a temporary pool:
	int poolsize = genepool->poolsize;
	// Allocate our genes:
	struct GP_Gene **new_pool = malloc(sizeof(struct GP_Gene *) * poolsize);

	// TODO: Make this a configuration parameter
	int TOURNAMENT_SIZE = 4;

	// Go through each pool slot:
	for (int idx = 0; idx < poolsize; idx++) {
		struct GP_Gene* best_gene;
		best_gene = _GP_GenePool_tournament_select(genepool,
							   TOURNAMENT_SIZE);
		assert(best_gene != NULL);

		// We have the best gene from the tournament:
		// Make a new gene:
		assert(best_gene->clone != NULL);

		struct GP_Gene* new_gene = best_gene->clone(best_gene);

		new_pool[idx] = new_gene;

		// See if we should mutate:
		if (((double)rand()) / RAND_MAX < 0.9) {
			assert(new_pool[idx]->mutate != NULL);
			new_pool[idx]->mutate(new_pool[idx]);
		}
	}

	// We now have a new pool:
	for (int idx=0; idx < poolsize; idx++)
		genepool->genes[idx]->free(genepool->genes[idx]);

	genepool->genes = new_pool;

}


char *GP_GenePool_as_debug_json(struct GP_GenePool *genepool)
{
	char *genebuffer = NULL;

	for (int idx = 0; idx < genepool->poolsize; idx++) {
		struct GP_Gene *gene = (genepool->genes)[idx];
		char *temp = NULL;
		// Should we add a comma?
		if(idx == 0){
			temp = malloc(snprintf(NULL, 0, "%s",
			     gene->as_debug_json(gene)) + 1);
			sprintf(temp, "%s",
	      gene->as_debug_json(gene));
		} else {
			temp = malloc(snprintf(NULL, 0, "%s,%s",
			     genebuffer,
			     gene->as_debug_json(gene)) + 1);
			sprintf(temp, "%s,%s",
	      genebuffer,
	      gene->as_debug_json(gene));
		}
		if(genebuffer != NULL)
			free(genebuffer);
		genebuffer = temp;
	}

	char *buffer = malloc(snprintf(NULL, 0,
		"{"
		"'address':'%p',"
		"'poolsize':'%i',"
		"'genes':'%s',"
		"}",
		genepool, // address
		genepool->poolsize, // Poolsize
		genebuffer // genes
				 ) + 1);
	sprintf(buffer,
		"{"
		"'address':'%p',"
		"'poolsize':'%i',"
		"'genes':'%s',"
		"}",
		genepool, // address
		genepool->poolsize, // Poolsize
		genebuffer // genes
		);

	free(genebuffer);
	return buffer;

}

