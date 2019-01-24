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
	// It should not be null:
	CU_ASSERT_PTR_NOT_NULL_FATAL(gene);

	// It should not have null functions:
	CU_ASSERT_PTR_NOT_NULL(gene->evaluate);
	CU_ASSERT_PTR_NOT_NULL(gene->clone);
	CU_ASSERT_PTR_NOT_NULL(gene->mutate);
	CU_ASSERT_PTR_NOT_NULL(gene->free);

	// It should not have null data:
	CU_ASSERT_PTR_NOT_NULL_FATAL(gene->data);

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
	// It should not have null functions:
	CU_ASSERT_PTR_NOT_NULL(clone_gene->evaluate);
	CU_ASSERT_PTR_NOT_NULL(clone_gene->clone);
	CU_ASSERT_PTR_NOT_NULL(clone_gene->free);
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

	// Make sure the non-nodes the same
	CU_ASSERT_EQUAL(clone_data->num_inputs, original_data->num_inputs);
	CU_ASSERT_EQUAL(clone_data->num_outputs, original_data->num_outputs);
	CU_ASSERT_EQUAL(clone_data->num_middle_nodes,
		  original_data->num_middle_nodes);

	// Check each of the node data pointers,
	// make sure they're not null or the same address:
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_data->middle_node_left_sources);
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_data->middle_node_right_sources);
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_data->middle_node_ops);
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_data->output_nodes);
	CU_ASSERT_PTR_NOT_EQUAL(clone_data->middle_node_left_sources,
				original_data->middle_node_left_sources);
	CU_ASSERT_PTR_NOT_EQUAL(clone_data->middle_node_right_sources,
				original_data->middle_node_right_sources);
	CU_ASSERT_PTR_NOT_EQUAL(clone_data->middle_node_ops,
				original_data->middle_node_ops);
	CU_ASSERT_PTR_NOT_EQUAL(clone_data->output_nodes,
				original_data->output_nodes);

	// Make sure the underlying data is the same:
	unsigned int num_mid = clone_data->num_middle_nodes;
	unsigned int num_out = clone_data->num_outputs;

	for (int idx = 0; idx < num_mid; idx++) {
		CU_ASSERT_EQUAL(clone_data->middle_node_left_sources[idx],
				original_data->middle_node_left_sources[idx]);
		CU_ASSERT_EQUAL(clone_data->middle_node_right_sources[idx],
				original_data->middle_node_right_sources[idx]);
		CU_ASSERT_EQUAL(clone_data->middle_node_ops[idx],
				original_data->middle_node_ops[idx]);
	}
	for (int idx = 0; idx < num_out; idx++) {
		CU_ASSERT_EQUAL(clone_data->output_nodes[idx],
				original_data->output_nodes[idx]);
	}


}
