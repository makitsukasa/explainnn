#include "lib/toposort.h"
#include "enn/edge.h"
#include <gtest/gtest.h>

TEST(topo_sort_tester, two_as_is) {
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

TEST(topo_sort_tester, two_reverse) {
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

TEST(topo_sort_tester, three_as_is) {
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

TEST(topo_sort_tester, three_reverse) {
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
