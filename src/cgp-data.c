#include "cgp-data.h"

void GP_CGP_evaluate(double* in, double* out, void *data)
{
	GP_CGPData* cgpdata = (GP_CGPData*) data;
	for(int i = 0; i < cgpdata->num_outputs; i++)
	{
		out[i] = i;
	}
}
