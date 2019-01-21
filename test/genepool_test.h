/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef _GENEPROG_GENEPOOL_TEST_H_
#define _GENEPROG_GENEPOOL_TEST_H_


#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int gp_test_genepool_setup_suite(void);

// Setup functions
int _init_genepool_suite(void);
int _clean_genepool_suite(void);

// Helper functions
struct GP_Gene *_gp_cgp_gene_builder(void);

// Test functions
void _gp_test_genepool_alloc(void);
void _gp_test_genepool_genesis_with_builder(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif

