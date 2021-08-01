#include "enn/individual.h"
#include "lib/randomlib.h"
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

TEST(Individual, calculate) {
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

TEST(Individual, same_seed) {
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

TEST(Individual, add_node_between_in_and_out) {
	enn::random_engine = std::mt19937(6700417);
	auto ind           = enn::individual(3, 2);

	ind.add_node(0, 4, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(1, 5, new enn::edge(0.1), new enn::edge(0.1));

	auto actual_order = ind.get_node_order();
	std::vector<unsigned long> expect_order{0, 1, 2, 3, 7, 6, 4, 5};
	ASSERT_EQ(expect_order.size(), actual_order.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_order.size(); i++) {
		EXPECT_FLOAT_EQ(expect_order[i], actual_order[i]) << "Vectors differ at index " << i;
	}

	auto actual_out = ind.calculate(std::vector<double>({0.1, 0.2, 0.3}));
	std::vector<double> expect_out{0.39071396, 0.50443918};
	ASSERT_EQ(expect_out.size(), actual_out.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_out.size(); i++) {
		EXPECT_FLOAT_EQ(expect_out[i], actual_out[i]) << "Vectors differ at index " << i;
	}
}

TEST(Individual, add_node_between_hiddens) {
	enn::random_engine = std::mt19937(6700417);
	auto ind           = enn::individual(3, 2);

	ind.add_node(0, 4, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(1, 5, new enn::edge(0.1), new enn::edge(0.1));

	ind.add_node(7, 6, new enn::edge(0.1), new enn::edge(0.1));

	auto actual_order = ind.get_node_order();
	std::vector<unsigned long> expect_order{0, 1, 2, 3, 7, 8, 6, 4, 5};
	ASSERT_EQ(expect_order.size(), actual_order.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_order.size(); i++) {
		EXPECT_FLOAT_EQ(expect_order[i], actual_order[i]) << "Vectors differ at index " << i;
	}

	auto actual_out = ind.calculate(std::vector<double>({0.1, 0.2, 0.3}));
	std::vector<double> expect_out{0.39073068, 0.50443918};
	ASSERT_EQ(expect_out.size(), actual_out.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_out.size(); i++) {
		EXPECT_FLOAT_EQ(expect_out[i], actual_out[i]) << "Vectors differ at index " << i;
	}
}
} // namespace enn
