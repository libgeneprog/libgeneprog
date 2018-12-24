#ifndef _GENEPROG_CGP_DATA_H_
#define _GENEPROG_CGP_DATA_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct _GP_CGPData
{
	unsigned int num_inputs;
	unsigned int num_middle_nodes;
	unsigned int num_outputs;

	//NOTE: Size will be 3 * num_middle_nodes
	unsigned int* middle_nodes;
	//NOTE: Size will be num_outputs
	unsigned int* output_nodes;
} GP_CGPData;

void GP_CGP_evaluate(double* in, double* out, void *data);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
