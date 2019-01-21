// SPDX-License-Identifier: BSD-3-Clause
#include "bst-data.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct GP_Gene *GP_BST_alloc(unsigned int num_in,
			     unsigned int depth,
			     unsigned int num_out)
{
	// Make sure we're not doing anything wrong:
	assert(num_in > 0);
	assert(num_out > 0);

	struct GP_Gene *gene = malloc(sizeof(struct GP_Gene));

	GP_BST_init(gene, num_in, depth, num_out);

	return gene;
}

void GP_BST_init(struct GP_Gene *gene,
		 unsigned int num_in,
		 unsigned int depth,
		 unsigned int num_out)
{
	// Make sure we're not doing anything wrong
	assert(num_in > 0);
	assert(num_out > 0);

	// Set up data:
	struct GP_BSTData *data = malloc(sizeof(struct GP_BSTData));

	data->num_inputs = num_in;
	data->depth = depth;
	data->num_outputs = num_out;

	// For now, set to null:
	data->output_nodes = NULL;

	gene->data = data;

	// Set up functions:
	gene->evaluate = GP_BST_evaluate;
	gene->clone = GP_BST_clone;
}

void _GP_BST_free_node(struct GP_BSTNode *node)
{
	if (node != NULL) {
		_GP_BST_free_node(node->leftNode);
		_GP_BST_free_node(node->rightNode);
		free(node);
	}
}

void GP_BST_free(struct GP_Gene *gene)
{
	struct GP_BSTData *bstdata = (struct GP_BSTData *)(gene->data);
	// Free each of our nodes:
	if (bstdata->output_nodes != NULL) {
		for (int i = 0; i < bstdata->num_outputs; i++)
			_GP_BST_free_node(bstdata->output_nodes[i]);
	}
	// Free the array:
	free(bstdata->output_nodes);
	// Free the gene itself:
	free(gene);
}

double _GP_BST_evaluate_node(struct GP_BSTNode *node,
			     double *in,
			     unsigned int num_in)
{
	// See what node type we're dealing with:
	enum GP_BSTNodeType nodeType = node->nodeType;

	if (nodeType == BSTNodeTypeOperand) {
		// What's our operand?
		unsigned int operand = node->nodeParams;
		// What's left/right?
		double left = _GP_BST_evaluate_node(node->leftNode,
						    in, num_in);
		double right = _GP_BST_evaluate_node(node->leftNode,
						     in, num_in);

		operand = operand % 3;
		if (operand == 0)
			return left + right;
		else if (operand == 1)
			return left - right;
		else if (operand == 2)
			return left * right;

	} else if (nodeType == BSTNodeTypeInput) {
		// Figure out which input we're using
		unsigned int inputIdx = node->nodeParams % num_in;
		return in[inputIdx];
	}


	return 0;
}

void GP_BST_evaluate(double *in, double *out, void *data)
{
	struct GP_BSTData *bstdata = (struct GP_BSTData *) data;
	// We *must* have nodes to continue:
	assert(bstdata->output_nodes != NULL);

	double result;

	// Go through and evaluate each of the trees:
	for (int i = 0; i < bstdata->num_outputs; i++) {
		result = _GP_BST_evaluate_node(bstdata->output_nodes[i],
					       in,
					       bstdata->num_inputs);
		out[i] = result;
	}
}

struct GP_BSTNode *_GP_BST_random_node(int depth)
{
	struct GP_BSTNode *node;

	node = (struct GP_BSTNode *)malloc(sizeof(struct GP_BSTNode));
	// See if we're at the bottom:
	if (depth <= 1)
		// We have to return a leaf node
		// This means a input node:
		node->nodeType = BSTNodeTypeInput;
	else
		// Pick a node type:
		node->nodeType = (enum GP_BSTNodeType)(rand() %
						       BSTNodeTypeNumTypes);

	node->nodeParams = rand();

	if (node->nodeType == BSTNodeTypeInput) {
		node->leftNode = NULL;
		node->rightNode = NULL;
	} else {
		node->leftNode = _GP_BST_random_node(depth-1);
		node->rightNode = _GP_BST_random_node(depth-1);
	}

	return node;
}

void GP_BST_randomize(struct GP_Gene *gene)
{
	struct GP_BSTData *bstdata = (struct GP_BSTData *) gene->data;
	unsigned int num_in = bstdata->num_inputs;
	unsigned int depth = bstdata->depth;
	unsigned int num_out = bstdata->num_outputs;

	if (bstdata->output_nodes == NULL)
		bstdata->output_nodes = malloc(sizeof(struct GP_BSTNode *) *
					       num_out);

	// Go through and set up our nodes:
	for (int i = 0; i < num_out; i++)
		bstdata->output_nodes[i] = _GP_BST_random_node(depth);

}

void _GP_BST_print_node(struct GP_BSTNode *node, int depth)
{
	for (int i = 0; i < depth; i++)
		printf("\t");
	printf("Node type: %u\n", node->nodeType);
	for (int i = 0; i < depth; i++)
		printf("\t");
	printf("Node params: %u\n", node->nodeParams);
	for (int i = 0; i < depth; i++)
		printf("\t");
	printf("Left Node:\n");
	if (node->leftNode == NULL) {
		for (int i = 0; i < depth; i++)
			printf("\t");
		printf("NULL\n");
	} else {
		_GP_BST_print_node(node->leftNode, depth+1);
	}
	for (int i = 0; i < depth; i++)
		printf("\t");
	printf("Right Node:\n");
	if (node->rightNode == NULL) {
		for (int i = 0; i < depth; i++)
			printf("\t");
		printf("NULL\n");
	} else {
		_GP_BST_print_node(node->rightNode, depth+1);
	}
}

void GP_BST_print(struct GP_Gene *gene)
{
	printf("Pulling out data:\n");
	struct GP_BSTData *bstdata = (struct GP_BSTData *) gene->data;

	printf("GP_BSTData:\n");
	printf("num_in: %u\n", bstdata->num_inputs);
	printf("num_out: %u\n", bstdata->num_outputs);
	printf("depth: %u\n", bstdata->depth);
	printf("nodes:\n");
	for (int i = 0; i < bstdata->num_outputs; i++) {
		printf("node %u\n", i);
		_GP_BST_print_node(bstdata->output_nodes[i], 1);
	}
}

struct GP_BSTNode *_GP_BST_clone_node(struct GP_BSTNode *source_node)
{
	struct GP_BSTNode *cloned_node = NULL;

	if(source_node != NULL) {
		// Make a new node:
		cloned_node = (struct GP_BSTNode *)malloc(sizeof(struct GP_BSTNode));
		cloned_node->nodeType = source_node->nodeType;
		cloned_node->nodeParams = source_node->nodeParams;
		cloned_node->leftNode =
			_GP_BST_clone_node(source_node->leftNode);
		cloned_node->rightNode =
			_GP_BST_clone_node(source_node->rightNode);
	}

	return cloned_node;
}

struct GP_Gene *GP_BST_clone(struct GP_Gene *source_gene)
{
	struct GP_Gene *cloned_gene;
	struct GP_BSTData *source_data;
	struct GP_BSTData *cloned_data;
	struct GP_BSTNode *source_node;
	unsigned int num_in, num_out, depth;

	// Get the data:
	source_data = (struct GP_BSTData *) source_gene->data;
	// Get the params:
	num_in = source_data->num_inputs;
	num_out = source_data->num_outputs;
	depth= source_data->depth;

	// Make a new gene to hold everything:
	cloned_gene = GP_BST_alloc(num_in, depth, num_out);

	// Set up our output nodes
	cloned_data = (struct GP_BSTData *) cloned_gene->data;
	cloned_data->output_nodes = malloc(sizeof(struct GP_BSTNode *) *
					   num_out);
	for (int idx = 0; idx < num_out; idx++) {
		source_node = source_data->output_nodes[idx];
		cloned_data->output_nodes[idx] = _GP_BST_clone_node(source_node);
	}

	return cloned_gene;
}
