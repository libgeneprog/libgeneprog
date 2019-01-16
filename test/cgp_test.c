// SPDX-License-Identifier: BSD-3-Clause
#include "cgp_test.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "cgp-data.h"

void _gp_test_cgp_alloc(void);
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
			_gp_test_cgp_alloc) == NULL) {
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
