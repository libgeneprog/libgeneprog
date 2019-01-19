/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef _GENEPROG_CGP_TEST_H_
#define _GENEPROG_CGP_TEST_H_


#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int gp_test_cgp_setup_suite(void);

int _init_cgp_suite(void);
int _clean_cgp_suite(void);

void _gp_test_cgp_alloc(void);
void _gp_test_cgp_clone(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif

