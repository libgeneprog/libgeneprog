/* SPDX-License-Identifier: BSD-3-Clause */
/**
 * @file
 * @brief Contains the main definition of a generic gene
 */
#ifndef _GENEPROG_GENE_H_
#define _GENEPROG_GENE_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
 * The types of genes that are available for GP_Gene
 */
enum GP_GeneType {
	// Hopefully we won't need Unknown that often...
	GeneTypeUnknown,
	GeneTypeBST,
	GeneTypeCGP,
	GeneTypeNumTypes //For picking random values
};

/**
 * A generic gene. Can be any number of types (BST, CGP, etc).
 */
struct GP_Gene {
	/**
	 * The underlying data of the gene. Will be specific to the gene type
	 * (BST, CGP, etc).
	 */
	void *data;

	/**
	 * The type of gene this represents.
	 * Used for verifying functions are being called for the proper genes.
	 */
	enum GP_GeneType geneType;

	/**
	 * The evaluation function for the gene. Will be specific to the gene
	 * type (BST, CGP, etc).
	 * Common usage:
	 * @code
	 * gene->evaluate(inputs, outputs, gene->data);
	 * @endcode
	 * @param in The array of inputs
	 * @param out The array to store the outputs in. Must be allocated ahead
	 * of time
	 * @param data The gene's data. Must match the evaluation function
	 */
	void (*evaluate)(double *in, double *out, void *data);

	/**
	 * The clone method for the gene.
	 * Should do a deep copy of the given gene.
	 * @code
	 * GP_Gene *clone = gene->clone(gene);
	 * @endcode
	 * @param gene The gene to do a deep copy of.
	 */
	struct GP_Gene *(*clone)(struct GP_Gene *gene);

	/**
	 * The mutate method for the gene.
	 * Randomly changes elements of the gene.
	 * This will be a destructive operation, so make sure to clone the gene
	 * first.
	 * @code
	 * gene->mutate(gene);
	 * @endcode
	 * @see clone()
	 * @param gene The gene to mutate
	 */
	void (*mutate)(struct GP_Gene *gene);

	/**
	 * Free method for the gene.
	 * Will take care of properly free'ing up the memory used in gene->data.
	 * @code
	 * gene->free(gene);
	 * @endcode
	 * @param gene The gene to free
	 */
	void (*free)(struct GP_Gene *gene);

	/**
	 * Gets a JSON representation of the gene. Provides enough info to
	 * debug.
	 */
	char *(*as_debug_json)(struct GP_Gene *gene);

};


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
