// SPDX-License-Identifier: BSD-3-Clause
#include "genepool_test.h"

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
	if (CU_add_test(pSuite,
			"_gp_test_genepool_alloc",
			_gp_test_genepool_alloc) ==
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
	return GP_CGP_alloc(1, 2, 3);
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
