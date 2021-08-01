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
}

TEST(topo_sort_tester, two_reverse) {
	std::vector<std::vector<bool>> matrix = {
		{false, false},
		{true, false},
	};
	auto actual   = topo_sort(matrix);
	auto expected = std::vector<unsigned long>{1, 0};

	ASSERT_EQ(expected.size(), actual.size()) << "Vectors are of unequal length";
}
