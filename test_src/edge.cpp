#include "../incl/wann/edge.h"
#include "../incl/lib/randomlib.h"
#include <gtest/gtest.h>

namespace enn {
class testEdge : public ::testing::Test {
public:
	static double getWeight(edge &edge) {
		return edge.weight;
	}
	static void setWeight(edge &edge, double weight) {
		edge.weight = weight;
	}
};

TEST_F(testEdge, SameSeed) {
	random_engine = std::mt19937(6700417);
	auto edge1    = edge();
	auto weight1  = testEdge::getWeight(edge1);

	random_engine = std::mt19937(6700417);
	auto edge2    = edge();
	auto weight2  = testEdge::getWeight(edge2);

	EXPECT_FLOAT_EQ(weight1, weight2);
}

TEST_F(testEdge, Calculate) {
	auto edge1 = edge();
	testEdge::setWeight(edge1, 0.8);
	auto actual = edge1.calculate(0.7);

	EXPECT_FLOAT_EQ(0.56, actual);
}
} // namespace enn
