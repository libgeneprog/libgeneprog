// SPDX-License-Identifier: BSD-3-Clause
#include "bst_test.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "bst-data.h"

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
	     NULL ||
	     CU_add_test(pSuite, "_gp_test_bst_clone", _gp_test_bst_clone) ==
	     NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	return CU_get_error();
}

void _gp_test_bst_alloc(void)
{
	struct GP_Gene *gene = GP_BST_alloc(1, 2, 3);

	// It should not be null:
	CU_ASSERT_PTR_NOT_NULL_FATAL(gene);

	// It should not have null functions:
	CU_ASSERT_PTR_NOT_NULL(gene->evaluate);
	CU_ASSERT_PTR_NOT_NULL(gene->clone);

	// It should not have null data:
	CU_ASSERT_PTR_NOT_NULL_FATAL(gene->data);

	// Get our data for more testing:
	struct GP_BSTData *bstdata = (struct GP_BSTData *)(gene->data);

	// It should set the variables:
	CU_ASSERT_EQUAL(bstdata->num_inputs, 1);
	CU_ASSERT_EQUAL(bstdata->depth, 2);
	CU_ASSERT_EQUAL(bstdata->num_outputs, 3);
}

void _gp_test_bst_clone_node(struct GP_BSTNode *original,
			     struct GP_BSTNode *clone)
{
	// If the original is NULL, assert the clone is NULL and return:
	if (original == NULL) {
		CU_ASSERT_PTR_NULL(clone);
	} else {
		// The original is not NULL.
		// The clone must not be null:
		CU_ASSERT_PTR_NOT_NULL_FATAL(clone);
		// Check the addresses. They *must* be different:
		CU_ASSERT_PTR_NOT_EQUAL(clone, original);
		// Node type & params must be the same:
		CU_ASSERT_EQUAL(clone->nodeType, original->nodeType);
		CU_ASSERT_EQUAL(clone->nodeParams, original->nodeParams);
		// Check the children:
		_gp_test_bst_clone_node(original->leftNode, clone->leftNode);
		_gp_test_bst_clone_node(original->rightNode, clone->rightNode);
	}
}

void _gp_test_bst_clone(void)
{
	// Make a gene and randomize it:
	struct GP_Gene *original_gene = GP_BST_alloc(1, 2, 3);

	GP_BST_randomize(original_gene);

	// Make a clone:
	struct GP_Gene *clone_gene = GP_BST_clone(original_gene);
	// It should not be null:
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_gene);

	// It should not have null functions:
	CU_ASSERT_PTR_NOT_NULL(clone_gene->evaluate);
	CU_ASSERT_PTR_NOT_NULL(clone_gene->clone);

	// It should not have null data:
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_gene->data);

	// It shouldn't be the same as the original gene:
	CU_ASSERT_PTR_NOT_EQUAL(clone_gene, original_gene);

	// Get the data of both for using:
	struct GP_BSTData *original_data;
	struct GP_BSTData *clone_data;

	original_data = (struct GP_BSTData *)(original_gene->data);
	clone_data = (struct GP_BSTData *)(clone_gene->data);

	// It'd shouldn't have the same data address as the original gene
	CU_ASSERT_PTR_NOT_EQUAL(clone_data, original_data);

	// Make sure the non-tree is the same
	CU_ASSERT_EQUAL(clone_data->num_inputs, original_data->num_inputs);
	CU_ASSERT_EQUAL(clone_data->num_outputs, original_data->num_outputs);
	CU_ASSERT_EQUAL(clone_data->depth, original_data->depth);

	// Make sure the output_nodes are set:
	CU_ASSERT_PTR_NOT_NULL_FATAL(clone_data->output_nodes);

	// Now the fun part.
	// We go through each tree, and confirm:
	// 1) The node addressed in the clone are NOT NULL
	//        if the original is not null
	// 2) The node addresses in the clone don't match the original
	// 3) The node VALUES in the clone match the original
	// 4) Repeat for each child of the node
	for (int idx = 0; idx < original_data->num_outputs; idx++) {
		_gp_test_bst_clone_node(original_data->output_nodes[idx],
					clone_data->output_nodes[idx]);
	}

}
