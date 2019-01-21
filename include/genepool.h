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
	unsigned int poolsize;

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
struct GP_GenePool *GP_GenePool_alloc(unsigned int poolsize);


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

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
