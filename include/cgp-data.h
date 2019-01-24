/* SPDX-License-Identifier: BSD-3-Clause */
/**
 * @file
 * @brief Contains the main data/functions relating to CGP genes
 */
#ifndef _GENEPROG_CGP_DATA_H_
#define _GENEPROG_CGP_DATA_H_

#include "gene.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
 * A representation of the operation for a given node.
 * Basically, whether it's addition, subtraction, etc.
 */
enum GP_CGPNodeOp {
	CGPNodeOpAdd,
	CGPNodeOpSubtract,
	CGPNodeOpMultiply,
	CGPNodeOpDivide,
	CGPNodeOpLeftOnly,
	CGPNodeOpRightOnly,
	CGPNodeOpNumOps //For picking random value
};

/**
 * The data for a CGP Gene.
 */
struct GP_CGPData {
	/**
	 * The number of inputs a gene can take in and use during evaluation.
	 */
	unsigned int num_inputs;
	/**
	 * The number of nodes between input/output.
	 * Note that there's no guarantee all/any nodes will be used to
	 * determine the output.
	 */
	unsigned int num_middle_nodes;
	/**
	 * The number of outputs the gene should generate when evaluated.
	 */
	unsigned int num_outputs;

	// NOTE: Sizes will be num_middle_nodes
	// TODO: Make these use a struct instead (like BST_Node)
	unsigned int *middle_node_left_sources;
	unsigned int *middle_node_right_sources;
	enum GP_CGPNodeOp *middle_node_ops;

	//NOTE: Size will be num_outputs
	unsigned int *output_nodes;
};


/**
 * Creates a new CGP Gene in the form of a plain ol' GP_Gene
 * Make sure to call GP_CGP_free() after you're done using them.
 * @see GP_CGP_free()
 */
struct GP_Gene *GP_CGP_alloc(unsigned int num_in,
		      unsigned int num_mid,
		      unsigned int num_out);
/**
 * Initializes a CGP_Gene. Useful if you handled the malloc/calloc yourself
 * Make sure to call GP_CGP_free() after you're done using it.
 * @see GP_CGP_free()
 */

void GP_CGP_init(struct GP_Gene *gene,
		 unsigned int num_in,
		 unsigned int num_mid,
		 unsigned int num_out);

/**
 * Frees a previously created CGP Gene
 * @see GP_CGP_alloc()
 */
void GP_CGP_free(struct GP_Gene *gene);

/**
 * Evaluates the CGP gene against the inputs, providing outputs.
 * For usage in GP_gene::evaluate
 * @param in The inputs to evaluate against
 * @param out An array to store the outputs in. This must be allocated
 * beforehand.
 * @param data The data for the CGP gene. Must be GP_CGPData.
 */
void GP_CGP_evaluate(double *in, double *out, void *data);

/**
 * Replaces a CGP gene's nodes with randomly built ones
 */
void GP_CGP_randomize(struct GP_Gene *gene);

/**
 * Mutates the CGP Gene.
 * This is a destructive operation. Make sure to clone first.
 * @see GP_CGP_clone()
 */
void GP_CGP_mutate(struct GP_Gene *gene);

/**
 * Makes a deep copy of a given CGP GP_Gene
 */
struct GP_Gene *GP_CGP_clone(struct GP_Gene *gene);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
