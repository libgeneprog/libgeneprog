// SPDX-License-Identifier: BSD-3-Clause
#include "bst_test.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "bst-data.h"

void _gp_test_bst_alloc(void);
int _init_bst_suite(void) { return 0; }
int _clean_bst_suite(void) { return 0; }

int gp_test_bst_setup_suite(void)
{
	CU_pSuite pSuite = NULL;

	/* add a suite to the registry */
	pSuite = CU_add_suite("bst_test_suite",
			      _init_bst_suite,
			      _clean_bst_suite);
	if (pSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (CU_add_test(pSuite, "_gp_test_bst_alloc", _gp_test_bst_alloc) ==
	     NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	return CU_get_error();
}

void _gp_test_bst_alloc(void)
{
	struct GP_Gene *gene = GP_BST_alloc(1, 2, 3);
	// It should not have null data:
	CU_ASSERT_PTR_NOT_NULL(gene->data);

	// Get our data for more testing:
	struct GP_BSTData *bstdata = (struct GP_BSTData *)(gene->data);

	// It should set the variables:
	CU_ASSERT_EQUAL(bstdata->num_inputs, 1);
	CU_ASSERT_EQUAL(bstdata->depth, 2);
	CU_ASSERT_EQUAL(bstdata->num_outputs, 3);
}
