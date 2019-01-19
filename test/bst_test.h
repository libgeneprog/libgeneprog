/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef _GENEPROG_BST_TEST_H_
#define _GENEPROG_BST_TEST_H_


#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int gp_test_bst_setup_suite(void);

int _init_bst_suite(void);
int _clean_bst_suite(void);

void _gp_test_bst_alloc(void);
void _gp_test_bst_clone(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif

