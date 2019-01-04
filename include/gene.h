/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef _GENEPROG_GENE_H_
#define _GENEPROG_GENE_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

struct GP_Gene {
	void *data;
	void (*evaluate)(double *in, double *out, void *data);
};


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
