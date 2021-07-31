#include "../incl/wann/edge.h"
#include "../incl/lib/randomlib.h"
#include <gtest/gtest.h>

namespace enn {
class test_edge : public ::testing::Test {
public:
	static double get_weight(edge &edge) {
		return edge.weight;
	}
	static void set_weight(edge &edge, double weight) {
		edge.weight = weight;
	}
};

TEST_F(test_edge, same_seed) {
	random_engine = std::mt19937(6700417);
	auto edge1    = edge();
	auto weight1  = test_edge::get_weight(edge1);

	random_engine = std::mt19937(6700417);
	auto edge2    = edge();
	auto weight2  = test_edge::get_weight(edge2);

	EXPECT_FLOAT_EQ(weight1, weight2);
}

TEST_F(test_edge, calculate) {
	auto edge1 = edge();
	test_edge::set_weight(edge1, 0.8);
	auto actual = edge1.calculate(0.7);

	EXPECT_FLOAT_EQ(0.56, actual);
}
} // namespace enn
