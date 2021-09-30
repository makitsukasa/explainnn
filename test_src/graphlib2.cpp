#include "lib/graphlib.h"
#include <gtest/gtest.h>

TEST(graphlib_tester, get_reachable_node_table) {
	std::vector<std::vector<bool>> adjacency_matrix = {
		{false, false, false, false, false, false, true, false, false},
		{false, false, false, false, false, false, false, true, false},
		{false, false, false, false, false, false, true, false, true},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, true, false, false, false, false, false},
		{false, false, false, true, false, false, false, false, false},
		{false, false, false, false, true, true, false, false, false},
	};

	std::vector<unsigned long> source_node_ids = {2};
	std::vector<bool> expected = {false, false, true, true, true, true, true, false, true};

	auto actual = get_reachable_node_table(adjacency_matrix, source_node_ids);

	EXPECT_EQ(expected.size(), actual.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expected.size(); i++) {
		EXPECT_EQ(expected[i], actual[i]) << "Matrices differ at index (" << i << ")";
	}
}
