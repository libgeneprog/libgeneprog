// SPDX-License-Identifier: BSD-3-Clause
#include "cgp_test.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "cgp-data.h"

int _init_cgp_suite(void) { return 0; }
int _clean_cgp_suite(void) { return 0; }

int gp_test_cgp_setup_suite(void)
{
	CU_pSuite pSuite = NULL;

	/* add a suite to the registry */
	pSuite = CU_add_suite("cgp_test_suite",
			      _init_cgp_suite,
			      _clean_cgp_suite);
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (CU_add_test(pSuite,
			"_gp_test_cgp_alloc",
			_gp_test_cgp_alloc) == NULL ||
	    CU_add_test(pSuite,
			"_gp_test_cgp_clone",
			_gp_test_cgp_clone) == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	return CU_get_error();
}

void _gp_test_cgp_alloc(void)
{
	struct GP_Gene *gene = GP_CGP_alloc(1, 2, 3);
	// It should not have null data:
	CU_ASSERT_PTR_NOT_NULL(gene->data);

	// Get our data for more testing:
	struct GP_CGPData *cgpdata = (struct GP_CGPData *) gene->data;

	// It should set the variables:
	CU_ASSERT_EQUAL(cgpdata->num_inputs, 1);
	CU_ASSERT_EQUAL(cgpdata->num_middle_nodes, 2);
	CU_ASSERT_EQUAL(cgpdata->num_outputs, 3);
}

void _gp_test_cgp_clone(void)
{
	// Make a gene and randomize it:
	struct GP_Gene *original_gene = GP_CGP_alloc(1, 2, 3);

	GP_CGP_randomize(original_gene);

	// Make a clone:
	struct GP_Gene *clone_gene = GP_CGP_clone(original_gene);
	// It should not be null:
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_gene);
	// It should not have null data:
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_gene->data);

	// It shouldn't be the same as the original gene:
	CU_ASSERT_PTR_NOT_EQUAL(clone_gene, original_gene);

	// Get the data of both for using:
	struct GP_CGPData *original_data;
	struct GP_CGPData *clone_data;

	original_data = (struct GP_CGPData *)(original_gene->data);
	clone_data = (struct GP_CGPData *)(clone_gene->data);

	// It'd shouldn't have the same data address as the original gene
	CU_ASSERT_PTR_NOT_EQUAL(clone_data, original_data);

	// Make sure the non-tree is the same
	CU_ASSERT_EQUAL(clone_data->num_inputs, original_data->num_inputs);
	CU_ASSERT_EQUAL(clone_data->num_outputs, original_data->num_outputs);
	CU_ASSERT_EQUAL(clone_data->num_middle_nodes,
		  original_data->num_middle_nodes);

}
