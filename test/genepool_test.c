// SPDX-License-Identifier: BSD-3-Clause
#include "genepool_test.h"

#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "genepool.h"
// Necessary for the builder function
#include "cgp-data.h"

int _init_genepool_suite(void) { return 0; }
int _clean_genepool_suite(void) { return 0; }

int gp_test_genepool_setup_suite(void)
{
	CU_pSuite pSuite = NULL;

	/* add a suite to the registry */
	pSuite = CU_add_suite("genepool_test_suite",
			      _init_genepool_suite,
			      _clean_genepool_suite);
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
//void _gp_test_genepool_evaluate(void)
	if (CU_add_test(pSuite,
			"_gp_test_genepool_alloc",
			_gp_test_genepool_alloc) ==
	    NULL ||
	    CU_add_test(pSuite,
			"_gp_test_genepool_evaluate",
			_gp_test_genepool_evaluate) ==
	    NULL ||
	    CU_add_test(pSuite,
			"_gp_test_genepool_genesis_with_builder",
			_gp_test_genepool_genesis_with_builder) ==
	    NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	return CU_get_error();

}

void _gp_test_genepool_alloc(void)
{
	struct GP_GenePool *pool;

	pool = GP_GenePool_alloc(5);
	// It should not be null:
	CU_ASSERT_PTR_NOT_NULL_FATAL(pool);

	// The poolsize should be right:
	CU_ASSERT_EQUAL(pool->poolsize, 5);
	// Genes should be NULL until genesis:
	CU_ASSERT_PTR_NULL(pool->genes);

}

struct GP_Gene *_gp_cgp_gene_builder(void)
{
	// Allocate:
	struct GP_Gene *gene = GP_CGP_alloc(1, 2, 3);
	// Randomize (so we can evaluate):
	GP_CGP_randomize(gene);
	return gene;
}

void _gp_test_genepool_genesis_with_builder(void)
{
	struct GP_GenePool *pool;

	pool = GP_GenePool_alloc(5);

	// Set up our builder function:
	struct GP_Gene *(*builder)() = _gp_cgp_gene_builder;

	// Genesis:
	GP_GenePool_genesis_with_builder(pool, builder);

	// Let's check:
	CU_ASSERT_PTR_NOT_NULL(pool->genes);

}

double _gp_static_fitness(double** actual, double** expected)
{
	return 0.5;
}

void _gp_test_genepool_evaluate(void)
{
	struct GP_GenePool *pool;
	pool = GP_GenePool_alloc(5);
	// Set up our builder function:
	struct GP_Gene *(*builder)() = _gp_cgp_gene_builder;
	// Genesis:
	GP_GenePool_genesis_with_builder(pool, builder);

	double **in = malloc(sizeof(double *) * 3);
	double **expected_out = malloc(sizeof(double *) * 3);

	for (int idx = 0; idx < 3; idx++) {
		in[idx] = malloc(sizeof(double) * 1);
		expected_out[idx] = malloc(sizeof(double) * 3);
	}

	// Evaluate:
	GP_GenePool_evaluate(pool,
		       3,
		       3,
		       in,
		       expected_out,
		       _gp_static_fitness);

	// At this point, we should have fitnesses
	CU_ASSERT_PTR_NOT_NULL(pool->fitnesses);
	// And based on our static fitness, they should all be 0.5:
	CU_ASSERT_EQUAL(pool->fitnesses[0], 0.5);

}
