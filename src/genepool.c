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
