/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef _GENEPROG_BST_DATA_H_
#define _GENEPROG_BST_DATA_H_

#include "gene.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

enum GP_BSTNodeType {
	BSTNodeTypeOperand,
	BSTNodeTypeInput,
	BSTNodeTypeNumTypes //For picking random values
};

struct GP_BSTNode {
	struct GP_BSTNode *leftNode;
	struct GP_BSTNode *rightNode;
	enum GP_BSTNodeType nodeType;
	unsigned int nodeParams;
};

struct GP_BSTData {
	unsigned int num_inputs;
	unsigned int num_outputs;
	unsigned int depth;

	// There's one output node per output
	struct GP_BSTNode **output_nodes;
};

/**
 * Creates a new BST gene in the form of a plain ol' GP_Gene
 * Make sure to call GP_BST_free() after you're done using them
 * @see GP_BST_free()
 */
struct GP_Gene *GP_BST_alloc(unsigned int num_in,
			     unsigned int depth,
			     unsigned int num_out);

/**
 * Initializes a BST_Gene. Useful if you handled the malloc/calloc yourself
 * Make sure to call GP_BST_free() after you're done using it.
 * @see GP_BST_free()
 */
void GP_BST_init(struct GP_Gene *gene,
		 unsigned int num_in,
		 unsigned int depth,
		 unsigned int num_out);

/**
 * Frees a previously created BST Gene
 * @see GP_BST_alloc()
 */
void GP_BST_free(struct GP_Gene *gene);

void GP_BST_evaluate(double *in, double *out, void *data);

void GP_BST_randomize(struct GP_Gene *gene);

void GP_BST_print(struct GP_Gene *gene);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
