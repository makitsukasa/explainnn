#include "lib/graphlib.h"
#include <gtest/gtest.h>

TEST(graphlib_tester, two_as_is) {
	std::vector<std::vector<bool>> matrix = {
		{false, true},
		{false, false},
	};
	auto actual   = topo_sort(matrix);
	auto expected = std::vector<unsigned long>{0, 1};

	ASSERT_EQ(expected.size(), actual.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expected.size(); i++) {
		EXPECT_FLOAT_EQ(expected[i], actual[i]) << "Vectors differ at index " << i;
	}
}

TEST(graphlib_tester, two_reverse) {
	std::vector<std::vector<bool>> matrix = {
		{false, false},
		{true, false},
	};
	auto actual   = topo_sort(matrix);
	auto expected = std::vector<unsigned long>{1, 0};

	ASSERT_EQ(expected.size(), actual.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expected.size(); i++) {
		EXPECT_FLOAT_EQ(expected[i], actual[i]) << "Vectors differ at index " << i;
	}
}

TEST(graphlib_tester, three_as_is) {
	std::vector<std::vector<bool>> matrix = {
		{false, true, true},
		{false, false, true},
		{false, false, false},
	};
	auto actual   = topo_sort(matrix);
	auto expected = std::vector<unsigned long>{0, 1, 2};

	ASSERT_EQ(expected.size(), actual.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expected.size(); i++) {
		EXPECT_FLOAT_EQ(expected[i], actual[i]) << "Vectors differ at index " << i;
	}
}

TEST(graphlib_tester, three_reverse) {
	std::vector<std::vector<bool>> matrix = {
		{false, false, false},
		{true, false, true},
		{true, false, false},
	};
	auto actual   = topo_sort(matrix);
	auto expected = std::vector<unsigned long>{1, 2, 0};

	ASSERT_EQ(expected.size(), actual.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expected.size(); i++) {
		EXPECT_FLOAT_EQ(expected[i], actual[i]) << "Vectors differ at index " << i;
	}
}

TEST(graphlib_tester, calc_reachablility_matrix) {
	std::vector<std::vector<bool>> adjacency_matrix = {
		{false, false, false, false, false, false, true, false, false},
		{false, false, false, false, false, false, false, true, false},
		{false, false, false, false, false, false, true, false, true},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, true, false, false, false, true, false},
		{false, false, false, true, false, false, false, false, false},
		{false, false, false, false, true, true, false, false, false},
	};

	std::vector<std::vector<bool>> expected = {
		{false, false, false, true, false, false, true, true, false},
		{false, false, false, true, false, false, false, true, false},
		{false, false, false, true, true, true, true, true, true},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, true, false, false, false, true, false},
		{false, false, false, true, false, false, false, false, false},
		{false, false, false, false, true, true, false, false, false},
	};
	std::vector<unsigned long> source_node_ids = {0, 1, 2};

	auto actual = calc_reachablility_matrix(adjacency_matrix, source_node_ids);

	EXPECT_EQ(expected.size(), actual.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expected.size(); i++) {
		EXPECT_EQ(expected[i].size(), actual[i].size())
			<< "Matrices are of unequal length at index" << i;
		for (unsigned long j = 0; j < expected[i].size(); j++) {
			EXPECT_EQ(expected[i][j], actual[i][j])
				<< "Matrices differ at index (" << i << "," << j << ")";
		}
	}
}
