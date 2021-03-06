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

	// Set the type:
	gene->geneType = GeneTypeBST;

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
	gene->free = GP_BST_free;
	gene->mutate = GP_BST_mutate;
	gene->as_debug_json = GP_BST_as_debug_json;
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
	assert(gene->geneType == GeneTypeBST);
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
	assert(gene->geneType == GeneTypeBST);
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
	assert(gene->geneType == GeneTypeBST);
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

/**
 * Calculates the size (num children nodes + self) for the given node
 */
int _GP_BST_node_size(struct GP_BSTNode *node)
{
	int size = 0;

	if (node != NULL) {
		int lhs = _GP_BST_node_size(node->leftNode);
		int rhs = _GP_BST_node_size(node->rightNode);

		// Make sure to count self (hence the +1)
		size = lhs + rhs + 1;
	}
	return size;
}

void _GP_BST_mutate_node(struct GP_BSTNode *node, int idx)
{
	// idx determines which node we're after:
	// Get *our* size:
	int node_size = _GP_BST_node_size(node);
	// If idx is the last one, it's us:
	if (idx >= node_size - 1) {
		// It's us
		// 1) Free left/right:
		_GP_BST_free_node(node->leftNode);
		_GP_BST_free_node(node->rightNode);
		// 2) Pick an op
		node->nodeType = (enum GP_BSTNodeType)(rand() %
						       BSTNodeTypeNumTypes);
		node->nodeParams = rand();

		// 3) See if we need a new left/right:
		if (node->nodeType == BSTNodeTypeInput) {
			node->leftNode = NULL;
			node->rightNode = NULL;
		} else {
			// TODO: Pick better numbers for depth:
			// Ideally we respect max_depth
			node->leftNode = _GP_BST_random_node(3);
			node->rightNode = _GP_BST_random_node(3);
		}

	} else {
		// Not us.
		// So it's either left or right
		int left_size = _GP_BST_node_size(node->leftNode);

		if (idx < left_size) {
			// It's in the left
			_GP_BST_mutate_node(node->leftNode, idx);
		} else {
			// It's in the right
			int new_idx = idx - left_size;

			_GP_BST_mutate_node(node->rightNode, new_idx);
		}
	}
}

void GP_BST_mutate(struct GP_Gene *gene)
{
	assert(gene->geneType == GeneTypeBST);
	// Alright, mutation time. Pull out the data:
	struct GP_BSTData *bstdata = (struct GP_BSTData *) gene->data;
	// Now, we need to pick *which* tree we're going to mutate:
	unsigned int num_out = bstdata->num_outputs;
	int tree_idx = rand() % num_out;
	struct GP_BSTNode *root_node = bstdata->output_nodes[tree_idx];
	// We have found what tree we're going to mutate
	// Get the size of that tree:
	int tree_size = _GP_BST_node_size(root_node);
	// And figure out which node idx we're going to mutate:
	int node_idx = rand() % tree_size;
	// And pass it to mutate_node:
	_GP_BST_mutate_node(root_node, node_idx);
}

/**
 * Clones a given node and all it's children
 */
struct GP_BSTNode *_GP_BST_clone_node(struct GP_BSTNode *source_node)
{
	struct GP_BSTNode *cloned_node = NULL;

	if (source_node != NULL) {
		// Make a new node:
		cloned_node =
			(struct GP_BSTNode *)malloc(sizeof(struct GP_BSTNode));
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
	assert(source_gene->geneType == GeneTypeBST);
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
	depth = source_data->depth;

	// Make a new gene to hold everything:
	cloned_gene = GP_BST_alloc(num_in, depth, num_out);

	// Set up our output nodes
	cloned_data = (struct GP_BSTData *) cloned_gene->data;
	cloned_data->output_nodes = malloc(sizeof(struct GP_BSTNode *) *
					   num_out);
	for (int idx = 0; idx < num_out; idx++) {
		source_node = source_data->output_nodes[idx];
		cloned_data->output_nodes[idx] =
			_GP_BST_clone_node(source_node);
	}

	return cloned_gene;
}

const char *_GP_BST_node_type_name(enum GP_BSTNodeType nodeType)
{
	switch(nodeType){
		case BSTNodeTypeOperand:  return "operand";
		case BSTNodeTypeInput:    return "input";
		case BSTNodeTypeNumTypes: return "numtypes";
	}
}

char *GP_BST_node_debug_json(struct GP_BSTNode *node)
{
	char *buffer;
	if(node == NULL) {
		// Have to do it as malloc/sprintf so that later frees work
		buffer = malloc(snprintf(NULL,0,"null")+1);
		sprintf(buffer, "null");
	} else {
		char *left = GP_BST_node_debug_json(node->leftNode);
		char *right= GP_BST_node_debug_json(node->rightNode);
		assert(left != NULL);
		assert(right != NULL);
		const char *nodeType = _GP_BST_node_type_name(node->nodeType);
		buffer = malloc(
			snprintf(NULL, 0,
				"{"
				"\"left\":%s,"
				"\"right\":%s,"
				"\"nodeType\":\"%s\","
				"\"nodeParams\":%u"
				"}",
				left,
				right,
				nodeType,
				node->nodeParams
			) + 1);
		sprintf(buffer,
			"{"
			"\"left\":%s,"
			"\"right\":%s,"
			"\"nodeType\":\"%s\","
			"\"nodeParams\":%u"
			"}",
			left,
			right,
			nodeType,
			node->nodeParams
		);
		free(left);
		free(right);
	}
	return buffer;
}

char *GP_BST_as_debug_json(struct GP_Gene *gene)
{
	assert(gene->geneType == GeneTypeBST);
	struct GP_BSTData *bstdata = (struct GP_BSTData *) gene->data;

	char *nodesJson;
	nodesJson = malloc(snprintf(NULL, 0, "")+1);
	sprintf(nodesJson, "");
	for (int i = 0; i < bstdata->num_outputs; i++) {
		char *temp;
		char *nodeJson = GP_BST_node_debug_json(bstdata->output_nodes[i]);
		char separator;
		if(i == 0)
			separator = '[';
		else
			separator = ',';
		temp = malloc(
			snprintf(NULL, 0,
			"%s"
			"%c"
			"%s",
			nodesJson,
			separator,
			nodeJson
			) + 1);
		sprintf(temp,
			"%s"
			"%c"
			"%s",
			nodesJson,
			separator,
			nodeJson
			);
		free(nodeJson);
		free(nodesJson);
		nodesJson = temp;
	}


	char *buffer = malloc(
		snprintf(NULL, 0,
			"{"
			"\"type\":\"BSTGene\","
			"\"address\":\"%p\","
			"\"nodes\":%s]"
			"}",
			gene,
			nodesJson
		) + 1);
	sprintf(buffer,
		"{"
		"\"type\":\"BSTGene\","
		"\"address\":\"%p\","
		"\"nodes\":%s]"
		"}",
		gene,
		nodesJson
		);
	free(nodesJson);
	return buffer;

}
