// SPDX-License-Identifier: BSD-3-Clause
#include "cgp-data.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

//TODO: Need a logging method of some sort...

struct GP_Gene *GP_CGP_alloc(unsigned int num_in,
			     unsigned int num_mid,
			     unsigned int num_out)
{
	// Make sure we're not doing anything wrong:
	assert(num_in > 0);
	assert(num_mid > 0);
	assert(num_out > 0);

	struct GP_Gene *gene = malloc(sizeof(struct GP_Gene));

	GP_CGP_init(gene, num_in, num_mid, num_out);

	return gene;
}

void GP_CGP_init(struct GP_Gene *gene,
		 unsigned int num_in,
		 unsigned int num_mid,
		 unsigned int num_out)
{
	// Make sure we're not doing anything wrong:
	assert(num_in > 0);
	assert(num_mid > 0);
	assert(num_out > 0);

	// Set up data:
	struct GP_CGPData *data = malloc(sizeof(struct GP_CGPData));

	data->num_inputs = num_in;
	data->num_middle_nodes = num_mid;
	data->num_outputs = num_out;
	//TODO: Build the middle/output nodes
	// For now though, set to null:
	data->middle_node_left_sources = NULL;
	data->middle_node_right_sources = NULL;
	data->middle_node_ops = NULL;
	data->output_nodes = NULL;
	gene->data = data;
	// Set up functions:
	gene->evaluate = GP_CGP_evaluate;
	gene->clone = GP_CGP_clone;
	gene->free = GP_CGP_free;
	gene->mutate = GP_CGP_mutate;
	gene->as_debug_json = GP_CGP_as_debug_json;
}

void GP_CGP_free(struct GP_Gene *gene)
{
	struct GP_CGPData *cgpdata = (struct GP_CGPData *)(gene->data);
	// Free the nodes:
	if (cgpdata->middle_node_left_sources != NULL)
		free(cgpdata->middle_node_left_sources);
	if (cgpdata->middle_node_right_sources != NULL)
		free(cgpdata->middle_node_right_sources);
	if (cgpdata->middle_node_ops != NULL)
		free(cgpdata->middle_node_ops);
	if (cgpdata->output_nodes != NULL)
		free(cgpdata->output_nodes);
	// Free the data:
	free(gene->data);
	// Free the gene itself:
	free(gene);
}

double _GP_CGP_evaluate_node(struct GP_CGPData *data,
			     double *in,
			     unsigned int node_idx)
{
	//Is the node_idx an input node?
	if (node_idx < data->num_inputs)
		return in[node_idx];

	// Alright, not an input node. Must be a middle node then
	unsigned int middle_node_idx, left_source, right_source;
	double left, right;
	enum GP_CGPNodeOp op;

	middle_node_idx  = node_idx - data->num_inputs;
	left_source  = data->middle_node_left_sources[middle_node_idx];
	right_source = data->middle_node_right_sources[middle_node_idx];
	op = data->middle_node_ops[middle_node_idx];
	left = _GP_CGP_evaluate_node(data, in, left_source);
	right = _GP_CGP_evaluate_node(data, in, right_source);
	if (op == CGPNodeOpAdd) {
		return left + right;
	} else if (op == CGPNodeOpSubtract) {
		return left - right;
	} else if (op == CGPNodeOpMultiply) {
		return left * right;
	} else if (op == CGPNodeOpDivide) {
		// TODO: Figure out a better way to handle N / 0
		// Ideally, end user should have say in how this is handled
		// (ideally via a configuration.)
		if (right == 0)
			return left;
		return left / right;
	} else if (op == CGPNodeOpLeftOnly) {
		return left;
	} else if (op == CGPNodeOpRightOnly) {
		return right;
	}
	// TODO: Fail spectacularly (not supposed to make it here)
	return right;
}

void GP_CGP_evaluate(double *in, double *out, void *data)
{
	struct GP_CGPData *cgpdata = (struct GP_CGPData *) data;
	// We *must* have nodes to continue:
	assert(cgpdata->middle_node_left_sources != NULL);
	assert(cgpdata->middle_node_right_sources != NULL);
	assert(cgpdata->middle_node_ops != NULL);
	assert(cgpdata->output_nodes != NULL);

	unsigned int out_source;
	double result;

	for (int i = 0; i < cgpdata->num_outputs; i++) {
		out_source = cgpdata->output_nodes[i];
		result = _GP_CGP_evaluate_node(cgpdata, in, out_source);
		out[i] = result;
	}
}

void GP_CGP_randomize(struct GP_Gene *gene)
{
	struct GP_CGPData *cgpdata = (struct GP_CGPData *) gene->data;
	unsigned int num_in = cgpdata->num_inputs;
	unsigned int num_mid = cgpdata->num_middle_nodes;
	unsigned int num_out = cgpdata->num_outputs;

	// See if we need to alloc our nodes:
	if (cgpdata->middle_node_left_sources == NULL)
		cgpdata->middle_node_left_sources =
			malloc(sizeof(unsigned int) * num_mid);
	if (cgpdata->middle_node_right_sources == NULL)
		cgpdata->middle_node_right_sources =
			malloc(sizeof(unsigned int) * num_mid);
	if (cgpdata->middle_node_ops == NULL)
		cgpdata->middle_node_ops =
			malloc(sizeof(enum GP_CGPNodeOp) * num_mid);
	if (cgpdata->output_nodes == NULL)
		cgpdata->output_nodes = malloc(sizeof(unsigned int) * num_out);

	// Go through and set up our middle nodes:
	unsigned int source_left, source_right;
	enum GP_CGPNodeOp op;

	for (int i = 0; i < num_mid; i++) {
		source_left = rand() % (num_in + i);
		source_right = rand() % (num_in + i);
		cgpdata->middle_node_left_sources[i] = source_left;
		cgpdata->middle_node_right_sources[i] = source_right;
		op = (enum GP_CGPNodeOp) (rand() % CGPNodeOpNumOps);
		cgpdata->middle_node_ops[i] = op;
	}

	// Go through and set up our output nodes:
	unsigned int source;

	for (int i = 0; i < num_out; i++) {
		source = rand() % (num_in + num_mid);
		cgpdata->output_nodes[i] = source;
	}
}

void GP_CGP_mutate(struct GP_Gene *gene)
{
	// Make sure we have a gene:
	assert(gene != NULL);
	// And data:
	assert(gene->data != NULL);
	struct GP_CGPData *cgpdata = (struct GP_CGPData *) gene->data;
	unsigned int num_in = cgpdata->num_inputs;
	unsigned int num_mid = cgpdata->num_middle_nodes;
	unsigned int num_out = cgpdata->num_outputs;

	// TODO: Make this a constant/configuration:
	double MUTATION_RATE = 0.2;

	// Go through and set mutate middle nodes:
	unsigned int source_left, source_right;
	enum GP_CGPNodeOp op;

	for (int i = 0; i < num_mid; i++) {
		source_left = rand() % (num_in + i);
		source_right = rand() % (num_in + i);
		if ( ((double)rand()) / RAND_MAX < MUTATION_RATE)
			cgpdata->middle_node_left_sources[i] = source_left;
		if ( ((double)rand()) / RAND_MAX < MUTATION_RATE)
			cgpdata->middle_node_right_sources[i] = source_right;
			op = (enum GP_CGPNodeOp) (rand() % CGPNodeOpNumOps);
		if ( ((double)rand()) / RAND_MAX < MUTATION_RATE)
			cgpdata->middle_node_ops[i] = op;
	}

	// Go through and set up our output nodes:
	unsigned int source;

	for (int i = 0; i < num_out; i++) {
		source = rand() % (num_in + num_mid);
		if ( ((double)rand()) / RAND_MAX < MUTATION_RATE)
			cgpdata->output_nodes[i] = source;
	}


}

struct GP_Gene *GP_CGP_clone(struct GP_Gene *source_gene)
{
	assert(source_gene != NULL);
	struct GP_Gene *cloned_gene;
	struct GP_CGPData *source_data;
	struct GP_CGPData *cloned_data;
	unsigned int num_in, num_mid, num_out;

	// Get the data:
	source_data = (struct GP_CGPData *)source_gene->data;
	assert(source_data != NULL);
	num_in = source_data->num_inputs;
	num_mid = source_data->num_middle_nodes;
	num_out = source_data->num_outputs;

	// Make our new gene:
	cloned_gene = GP_CGP_alloc(num_in, num_mid, num_out);
	cloned_data = (struct GP_CGPData *)cloned_gene->data;
	// Allocate all the data
	cloned_data->middle_node_left_sources =
		malloc(sizeof(unsigned int) * num_mid);
	cloned_data->middle_node_right_sources =
		malloc(sizeof(unsigned int) * num_mid);
	cloned_data->middle_node_ops =
		malloc(sizeof(enum GP_CGPNodeOp) * num_mid);
	cloned_data->output_nodes = malloc(sizeof(unsigned int) * num_out);

	// Copy the data over:
	for (int idx = 0; idx < num_mid; idx++) {
		cloned_data->middle_node_left_sources[idx] =
			source_data->middle_node_left_sources[idx];
		cloned_data->middle_node_right_sources[idx] =
			source_data->middle_node_right_sources[idx];
		cloned_data->middle_node_ops[idx] =
			source_data->middle_node_ops[idx];
	}
	for (int idx = 0; idx < num_out; idx++) {
		cloned_data->output_nodes[idx] =
			source_data->output_nodes[idx];
	}

	return cloned_gene;
}

char *GP_CGP_as_debug_json(struct GP_Gene *gene)
{
	//char *buffer;
	char *buffer = malloc(
		snprintf(NULL, 0,
		"{"
		"'type':'CGPGene',"
		"'address':'%p',"
		"'evaluate_function_address':'%p',"
		"'clone_function_address':'%p',"
		"'mutate_function_address':'%p',"
		"'as_debug_json_address':'%p'"
		"}",
		gene, // Address
		gene->evaluate, // Evaluate function address
		gene->clone, // Clone function address
		gene->mutate, // Mutate function address
		gene->as_debug_json // as_debug_json function address
	     ) + 1);
	sprintf(buffer,
		"{"
		"'type':'CGPGene',"
		"'address':'%p',"
		"'evaluate_function_address':'%p',"
		"'clone_function_address':'%p',"
		"'mutate_function_address':'%p',"
		"'as_debug_json_address':'%p'"
		"}",
		gene, // Address
		gene->evaluate, // Evaluate function address
		gene->clone, // Clone function address
		gene->mutate, // Mutate function address
		gene->as_debug_json // as_debug_json function address
		);
	return buffer;
}
