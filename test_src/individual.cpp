#include "../incl/enn/individual.h"
#include "../incl/lib/randomlib.h"
#include <gtest/gtest.h>

namespace enn {
class test_individual : public ::testing::Test {
public:
	static std::unordered_map<unsigned long, node> get_nodes(individual &individual) {
		return individual.nodes;
	}
	static void set_nodes(individual &individual, std::unordered_map<unsigned long, node> nodes) {
		individual.nodes = nodes;
	}
	static std::vector<unsigned long> get_node_order(individual &individual) {
		return individual.node_order;
	}
	static void get_node_order(individual &individual, std::vector<unsigned long> node_order) {
		individual.node_order = node_order;
	}
};

TEST(Individual, Calculate) {
	enn::random_engine = std::mt19937(6700417);
	auto ind1          = enn::individual(3, 2);
	auto out1          = ind1.calculate(std::vector<double>({0.1, 0.2, 0.3}));

	enn::random_engine = std::mt19937(6700417);
	auto ind2          = enn::individual(3, 2);
	auto out2          = ind2.calculate(std::vector<double>({0.1, 0.2, 0.3}));

	ASSERT_EQ(out1.size(), out2.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < out1.size(); i++) {
		EXPECT_FLOAT_EQ(out1[i], out2[i]) << "Vectors differ at index " << i;
	}
}

TEST(Individual, SameSeed) {
	enn::random_engine = std::mt19937(6700417);
	auto ind1          = enn::individual(3, 2);
	auto out1          = ind1.calculate(std::vector<double>({0.1, 0.2, 0.3}));

	enn::random_engine = std::mt19937(6700417);
	auto ind2          = enn::individual(3, 2);
	auto out2          = ind2.calculate(std::vector<double>({0.1, 0.2, 0.3}));

	ASSERT_EQ(out1.size(), out2.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < out1.size(); i++) {
		EXPECT_FLOAT_EQ(out1[i], out2[i]) << "Vectors differ at index " << i;
	}
}
} // namespace enn
