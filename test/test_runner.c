#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "bst_test.h"

#include <stdio.h>


int main ( void )
{
   // Set up the test suites:
   gp_test_bst_setup_suite();

   // Run them:
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");
   CU_basic_show_failures(CU_get_failure_list());
   printf("\n\n");

   CU_cleanup_registry();
   return CU_get_error();
}
