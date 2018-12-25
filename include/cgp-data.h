#ifndef _GENEPROG_CGP_DATA_H_
#define _GENEPROG_CGP_DATA_H_

#include "gene.h"

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


/**
 * Creates a new CGP Gene in the form of a plain ol' GP_Gene
 * Make sure to call GP_CGP_free() after you're done using them.
 * @see GP_CGP_free()
 */
GP_Gene* GP_CGP_alloc(unsigned int num_in,
                      unsigned int num_mid,
                      unsigned int num_out);

/**
 * Frees a previously created CGP Gene
 * @see GP_CGP_alloc()
 */
void GP_CGP_free(GP_Gene* gene);

void GP_CGP_evaluate(double* in, double* out, void *data);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
