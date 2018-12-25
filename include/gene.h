#ifndef _GENEPROG_GENE_H_
#define _GENEPROG_GENE_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct _GP_Gene
{
	void* data;
	void (*evaluate)(double* in, double* out, void* data);
} GP_Gene;


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
