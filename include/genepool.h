/* SPDX-License-Identifier: BSD-3-Clause */
/**
 * @file
 * @brief Contains the definition of a genepool
 */
#ifndef _GENEPROG_GENEPOOL_H_
#define _GENEPROG_GENEPOOL_H_

#include "gene.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
 * A regular genepool
 */
struct GP_GenePool {
	/**
	 * The genes that make up the genepool
	 */
	struct GP_Gene **genes;

	/**
	 * The number of genes in the pool
	 */
	int poolsize;

	/**
	 * The fitnesses for a given generation
	 */
	double *fitnesses;

	/**
	 * The function used to generate a gene.
	 * This *must* be provided before genesis is called.
	 */
	struct GP_Gene *(*build_gene)();
};


/**
 * Builds a genepool
 */
struct GP_GenePool *GP_GenePool_alloc(int poolsize);


/**
 * Sets up the genepool using the provided gene builder function.
 */
void GP_GenePool_genesis_with_builder(struct GP_GenePool *genepool,
				      struct GP_Gene *(*builder)());

/**
 * Sets up the genepool.
 * build_gene must be set before this is called.
 * Use GP_GenePool_genesis_with_builder to provide this.
 */
void GP_GenePool_genesis(struct GP_GenePool *genepool);

/**
 * Evaluates a genepool using the provided fitness function
 */
void GP_GenePool_evaluate(struct GP_GenePool *genepool,
			  unsigned int num_rows,
			  unsigned int num_outputs,
			  double **inputs,
			  double **expected_outputs,
			  double (*fitness)(double**, double**));

/**
 * Builds the next generation based on the fitnesses in a given genepool
 */
void GP_GenePool_build_next_generation(struct GP_GenePool *genepool);

/**
 * Returns a debuggable JSON representation of the genepool.
 */
char *GP_GenePool_as_debug_json(struct GP_GenePool *genepool);

/**
 * Returns the fittest gene in the genepool.
 */
struct GP_Gene *GP_GenePool_fittest_gene(struct GP_GenePool *genepool);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
