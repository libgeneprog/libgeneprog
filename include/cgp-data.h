/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef _GENEPROG_CGP_DATA_H_
#define _GENEPROG_CGP_DATA_H_

#include "gene.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

enum GP_CGPNodeOp {
	CGPNodeOpAdd,
	CGPNodeOpSubtract,
	CGPNodeOpMultiply,
	CGPNodeOpDivide,
	CGPNodeOpLeftOnly,
	CGPNodeOpRightOnly,
	CGPNodeOpNumOps //For picking random value
};

struct GP_CGPData {
	unsigned int num_inputs;
	unsigned int num_middle_nodes;
	unsigned int num_outputs;

	//NOTE: Sizes will be num_middle_nodes
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

void GP_CGP_evaluate(double *in, double *out, void *data);

void GP_CGP_randomize(struct GP_Gene *gene);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
