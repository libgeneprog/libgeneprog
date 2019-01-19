// SPDX-License-Identifier: BSD-3-Clause
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "bst_test.h"
#include "cgp_test.h"

#include <stdio.h>


int main(void)
{
	/* initialize the CUnit test registry */
	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	// Set up the test suites:
	gp_test_bst_setup_suite();
	gp_test_cgp_setup_suite();

	// Run them:
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_ErrorCode code = CU_basic_run_tests();
	printf("\n");
	CU_basic_show_failures(CU_get_failure_list());
	printf("\n\n");

	int failures = CU_get_number_of_tests_failed();
	CU_cleanup_registry();

	return failures > 0 ? 1 : 0;
}
