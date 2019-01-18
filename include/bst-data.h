/* SPDX-License-Identifier: BSD-3-Clause */
/**
 * @file
 * @brief Contains the main data/functions relating to BST genes
 */
#ifndef _GENEPROG_BST_DATA_H_
#define _GENEPROG_BST_DATA_H_

#include "gene.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/**
 * A representation of the node's function in a BS tree.
 * Basically, whether it's an operand or an input.
 */
enum GP_BSTNodeType {
	BSTNodeTypeOperand,
	BSTNodeTypeInput,
	BSTNodeTypeNumTypes //For picking random values
};

/**
 * A single node in a BS tree.
 */
struct GP_BSTNode {
	/**
	 * The left child of this BST node. May be NULL if this is a leaf node
	 */
	struct GP_BSTNode *leftNode;
	/**
	 * The right child of this BST node. May be NULL if this is a leaf node
	 */
	struct GP_BSTNode *rightNode;
	/**
	 * The type of node this is (whether it's an operand or input)
	 */
	enum GP_BSTNodeType nodeType;
	/**
	 * The parameters for the type of node.
	 * For example:
	 * If it's an input node, it determines which input is used.
	 * If it's an operand node, it determines which operation is used.
	 */
	unsigned int nodeParams;
};

/**
 * The data for a BST Gene.
 * Note that a BST Gene will potentially have multiple trees
 * (one for each output)
 */
struct GP_BSTData {
	/**
	 * The number of inputs a tree can take in and use during evaluation
	 */
	unsigned int num_inputs;
	/**
	 * The number of outputs the gene should generate.
	 * Determines the number of trees.
	 */
	unsigned int num_outputs;
	/**
	 * The max depth allowed for any given tree.
	 */
	unsigned int depth;

	/**
	 * The root nodes of the trees.
	 * There will be one node per output, so the size will be equal to
	 * num_outputs
	 */
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
