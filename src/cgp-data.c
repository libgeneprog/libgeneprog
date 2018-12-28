#include "cgp-data.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

//TODO: Need a logging method of some sort...

GP_Gene* GP_CGP_alloc(unsigned int num_in,
                      unsigned int num_mid,
                      unsigned int num_out)
{
	// Make sure we're not doing anything wrong:
	assert(num_in > 0);
	assert(num_mid > 0);
	assert(num_out > 0);

	GP_Gene* gene = malloc(sizeof(GP_Gene));

	GP_CGP_init(gene, num_in, num_mid, num_out);

	return gene;
}

void GP_CGP_init(GP_Gene *gene,
                 unsigned int num_in,
                 unsigned int num_mid,
                 unsigned int num_out)
{
	// Make sure we're not doing anything wrong:
	assert(num_in > 0);
	assert(num_mid > 0);
	assert(num_out > 0);

	// Set up data:
	GP_CGPData* data = malloc(sizeof(GP_CGPData));
	data->num_inputs = num_in;
	data->num_middle_nodes= num_mid;
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
}

void GP_CGP_free(GP_Gene* gene)
{
	GP_CGPData* cgpdata = (GP_CGPData*)(gene->data);
	// Free the nodes:
	if(cgpdata->middle_node_left_sources != NULL)
		free(cgpdata->middle_node_left_sources);
	if(cgpdata->middle_node_right_sources != NULL)
		free(cgpdata->middle_node_right_sources);
	if(cgpdata->middle_node_ops != NULL)
		free(cgpdata->middle_node_ops);
	if(cgpdata->output_nodes != NULL)
		free(cgpdata->output_nodes);
	// Free the data:
	free(gene->data);
	// Free the gene itself:
	free(gene);
}

double _GP_CGP_evaluate_node(GP_CGPData* data, double* in, unsigned int node_idx)
{
	//Is the node_idx an input node?
	if(node_idx < data->num_inputs)
		return in[node_idx];
	// Alright, not an input node. Must be a middle node then
	unsigned int middle_node_idx = node_idx - data->num_inputs;
	unsigned int left_source = data->middle_node_left_sources[middle_node_idx];
	unsigned int right_source = data->middle_node_right_sources[middle_node_idx];
	enum GP_CGPNodeOp op = data->middle_node_ops[middle_node_idx];
	double left = _GP_CGP_evaluate_node(data, in, left_source);
	double right = _GP_CGP_evaluate_node(data, in, right_source);
	if(op == CGPNodeOpAdd){
		return left + right;
	}
	else if(op == CGPNodeOpSubtract){
		return left - right;
	}
	else if(op == CGPNodeOpMultiply){
		return left * right;
	}
	else if(op == CGPNodeOpDivide){
		// TODO: Figure out a better way to handle N / 0
		// Ideally, end user should have say in how this is handled via a configuration.
		if(right == 0)
			return left;
		return left / right;
	}
	else if(op == CGPNodeOpLeftOnly){
		return left;
	}
	else if(op == CGPNodeOpRightOnly){
		return right;
	}
	// TODO: Fail spectacularly (not supposed to make it here)
	return right;
}

void GP_CGP_evaluate(double* in, double* out, void *data)
{
	GP_CGPData* cgpdata = (GP_CGPData*) data;
	// We *must* have nodes to continue:
	assert(cgpdata->middle_node_left_sources != NULL);
	assert(cgpdata->middle_node_right_sources != NULL);
	assert(cgpdata->middle_node_ops != NULL);
	assert(cgpdata->output_nodes != NULL);
	for(int i = 0; i < cgpdata->num_outputs; i++)
	{
		unsigned int out_source = cgpdata->output_nodes[i];
		double result = _GP_CGP_evaluate_node(cgpdata, in, out_source);
		out[i] = result;
	}
}

void GP_CGP_randomize(GP_Gene* gene)
{
	GP_CGPData* cgpdata = (GP_CGPData*) gene->data;
	unsigned int num_in = cgpdata->num_inputs;
	unsigned int num_mid = cgpdata->num_middle_nodes;
	unsigned int num_out= cgpdata->num_outputs;
	// See if we need to alloc our nodes:
	if(cgpdata->middle_node_left_sources == NULL)
		cgpdata->middle_node_left_sources = malloc(sizeof(unsigned int) * num_mid);
	if(cgpdata->middle_node_right_sources == NULL)
		cgpdata->middle_node_right_sources = malloc(sizeof(unsigned int) * num_mid);
	if(cgpdata->middle_node_ops == NULL)
		cgpdata->middle_node_ops = malloc(sizeof(enum GP_CGPNodeOp) * num_mid);
	if(cgpdata->output_nodes == NULL)
		cgpdata->output_nodes= malloc(sizeof(unsigned int) * num_out);

	// Go through and set up our middle nodes:
	for(int i=0; i<num_mid; i++){
		unsigned int source_left = rand() % (num_in + i);
		unsigned int source_right = rand() % (num_in + i);
		cgpdata->middle_node_left_sources[i] = source_left;
		cgpdata->middle_node_right_sources[i] = source_right;
		enum GP_CGPNodeOp op = (enum GP_CGPNodeOp) (rand() % CGPNodeOpNumOps);
		cgpdata->middle_node_ops[i] = op;
	}

	// Go through and set up our output nodes:
	for(int i=0; i<num_out; i++){
		unsigned int source = rand() % (num_in + num_mid);
		cgpdata->output_nodes[i] = source;
	}

}

