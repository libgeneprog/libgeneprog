#include "cgp-data.h"

#include <stdlib.h>
#include <assert.h>

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
	data->middle_nodes = NULL;
	data->output_nodes = NULL;
	gene->data = data;
	// Set up functions:
	gene->evaluate = GP_CGP_evaluate;
}

void GP_CGP_free(GP_Gene* gene)
{
	GP_CGPData* cgpdata = (GP_CGPData*)(gene->data);
	//TODO: Free the nodes
	// Free the data:
	free(gene->data);
	// Free the gene itself:
	free(gene);
}

void GP_CGP_evaluate(double* in, double* out, void *data)
{
	GP_CGPData* cgpdata = (GP_CGPData*) data;
	for(int i = 0; i < cgpdata->num_outputs; i++)
	{
		out[i] = i;
	}
}

