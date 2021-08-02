#include "enn/individual.h"
#include "lib/randomlib.h"
#include <gtest/gtest.h>

namespace enn {
class individual_tester : public ::testing::Test {
public:
	static std::unordered_map<unsigned long, node> get_nodes(individual &individual) {
		return individual.nodes;
	}
	static void set_nodes(individual &individual, std::unordered_map<unsigned long, node> nodes) {
		individual.nodes = nodes;
	}
	static std::vector<std::vector<edge *>> get_matrix(individual &individual) {
		return individual.adjacency_matrix;
	}
	static std::vector<std::vector<bool>> get_bool_matrix_hidden(individual &individual) {
		return individual.get_bool_matrix_hidden();
	}
};

TEST(individual_tester, calculate) {
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

TEST(individual_tester, constructor_same_seed) {
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

TEST(individual_tester, get_bool_matrix_hidden_as_is) {
	auto ind = enn::individual(3, 2);
	ind.add_node(0, 4, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(1, 5, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(7, 6, new enn::edge(0.1), new enn::edge(0.1));

	auto actual_order = ind.get_node_order();
	std::vector<unsigned long> expect_order{0, 1, 2, 3, 7, 8, 6, 4, 5};
	ASSERT_EQ(actual_order.size(), expect_order.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_order.size(); i++) {
		EXPECT_FLOAT_EQ(expect_order[i], actual_order[i]) << "Vectors differ at index " << i;
	}

	auto actual_matrix = individual_tester::get_bool_matrix_hidden(ind);
	std::vector<std::vector<bool>> expect_matrix{
		{false, true, false},
		{false, false, true},
		{false, false, false},
	};

	ASSERT_EQ(expect_matrix.size(), actual_matrix.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_matrix.size(); i++) {
		ASSERT_EQ(expect_matrix[i].size(), actual_matrix[i].size())
			<< "Matrices are of unequal length at index" << i;
		for (unsigned long j = 0; j < expect_matrix[i].size(); j++) {
			ASSERT_EQ(expect_matrix[i][j], actual_matrix[i][j])
				<< "Matrices differ at index (" << i << "," << j << ")";
		}
	}
}

TEST(individual_tester, get_bool_matrix_hidden_sort) {
	auto ind = enn::individual(3, 2);
	ind.add_node(0, 4, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(1, 5, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(6, 7, new enn::edge(0.1), new enn::edge(0.1));

	auto actual_order = ind.get_node_order();
	std::vector<unsigned long> expect_order{0, 1, 2, 3, 6, 8, 7, 4, 5};
	ASSERT_EQ(actual_order.size(), expect_order.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_order.size(); i++) {
		EXPECT_FLOAT_EQ(expect_order[i], actual_order[i]) << "Vectors differ at index " << i;
	}

	auto actual_matrix = individual_tester::get_bool_matrix_hidden(ind);
	std::vector<std::vector<bool>> expect_matrix{
		{false, true, false},
		{false, false, true},
		{false, false, false},
	};

	ASSERT_EQ(expect_matrix.size(), actual_matrix.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_matrix.size(); i++) {
		ASSERT_EQ(expect_matrix[i].size(), actual_matrix[i].size())
			<< "Matrices are of unequal length at index" << i;
		for (unsigned long j = 0; j < expect_matrix[i].size(); j++) {
			ASSERT_EQ(expect_matrix[i][j], actual_matrix[i][j])
				<< "Matrices differ at index (" << i << "," << j << ")";
		}
	}
}

TEST(individual_tester, add_node_between_in_and_out) {
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

	auto actual_matrix = individual_tester::get_matrix(ind);
	std::vector<std::vector<bool>> expect_matrix{
		{false, false, false, false, false, true, true, true},
		{false, false, false, false, true, false, true, true},
		{false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, true},
		{false, false, false, false, false, false, true, false},
		{false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false},
	};
	ASSERT_EQ(expect_matrix.size(), actual_matrix.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_matrix.size(); i++) {
		ASSERT_EQ(expect_matrix[i].size(), actual_matrix[i].size())
			<< "Matrices are of unequal length at index" << i;
		for (unsigned long j = 0; j < expect_matrix[i].size(); j++) {
			if (expect_matrix[i][j]) {
				if (actual_matrix[i][j] == nullptr) {
					auto node_order    = ind.get_node_order();
					auto node_id_order = inverse::inverse_vector(node_order);
					FAIL() << "Matrices differ at index (" << i << "," << j << "), which is (#"
						   << node_id_order[i] << ",#" << node_id_order[j] << ")";
				}
			} else {
				auto node_order    = ind.get_node_order();
				auto node_id_order = inverse::inverse_vector(node_order);
				if (actual_matrix[i][j] != nullptr) {
					FAIL() << "Matrices differ at index (" << i << "," << j << "), which is (#"
						   << node_id_order[i] << ",#" << node_id_order[j] << ")";
				}
			}
		}
	}

	auto actual_out = ind.calculate(std::vector<double>({0.1, 0.2, 0.3}));
	std::vector<double> expect_out{0.39071396, 0.50443918};
	ASSERT_EQ(expect_out.size(), actual_out.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_out.size(); i++) {
		EXPECT_FLOAT_EQ(expect_out[i], actual_out[i]) << "Vectors differ at index " << i;
	}
}

TEST(individual_tester, add_node_between_hiddens_as_is) {
	enn::random_engine = std::mt19937(6700417);
	auto ind           = enn::individual(3, 2);

	ind.add_node(0, 4, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(1, 5, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(7, 6, new enn::edge(0.1), new enn::edge(0.1)); // ソートは不要

	auto actual_order = ind.get_node_order();
	std::vector<unsigned long> expect_order{0, 1, 2, 3, 7, 8, 6, 4, 5};
	ASSERT_EQ(expect_order.size(), actual_order.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_order.size(); i++) {
		EXPECT_FLOAT_EQ(expect_order[i], actual_order[i]) << "Vectors differ at index " << i;
	}

	auto actual_matrix = individual_tester::get_matrix(ind);
	std::vector<std::vector<bool>> expect_matrix{
		{false, false, false, false, false, false, true, true, true},
		{false, false, false, false, true, false, false, true, true},
		{false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, true, false, false, true},
		{false, false, false, false, false, false, true, false, false},
		{false, false, false, false, false, false, false, true, false},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false},
	};
	ASSERT_EQ(expect_matrix.size(), actual_matrix.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_matrix.size(); i++) {
		ASSERT_EQ(expect_matrix[i].size(), actual_matrix[i].size())
			<< "Matrices are of unequal length at index" << i;
		for (unsigned long j = 0; j < expect_matrix[i].size(); j++) {
			if (expect_matrix[i][j]) {
				if (actual_matrix[i][j] == nullptr) {
					auto node_order    = ind.get_node_order();
					auto node_id_order = inverse::inverse_vector(node_order);
					FAIL() << "Matrices differ at index (" << i << "," << j << "), which is ("
						   << node_id_order[i] << "," << node_id_order[j] << ")";
				}
			} else {
				auto node_order    = ind.get_node_order();
				auto node_id_order = inverse::inverse_vector(node_order);
				if (actual_matrix[i][j] != nullptr) {
					FAIL() << "Matrices differ at index (" << i << "," << j << "), which is ("
						   << node_id_order[i] << "," << node_id_order[j] << ")";
				}
			}
		}
	}

	auto actual_out = ind.calculate(std::vector<double>({0.1, 0.2, 0.3}));
	std::vector<double> expect_out{0.39073068, 0.50443918};
	ASSERT_EQ(expect_out.size(), actual_out.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_out.size(); i++) {
		EXPECT_FLOAT_EQ(expect_out[i], actual_out[i]) << "Vectors differ at index " << i;
	}
}

TEST(individual_tester, add_node_between_hiddens_sort) {
	enn::random_engine = std::mt19937(6700417);
	auto ind           = enn::individual(3, 2);

	ind.add_node(0, 4, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(1, 5, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(6, 7, new enn::edge(0.1), new enn::edge(0.1)); // ソートが必要

	auto actual_order = ind.get_node_order();
	std::vector<unsigned long> expect_order{0, 1, 2, 3, 6, 8, 7, 4, 5};
	ASSERT_EQ(expect_order.size(), actual_order.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_order.size(); i++) {
		EXPECT_FLOAT_EQ(expect_order[i], actual_order[i]) << "Vectors differ at index " << i;
	}

	auto actual_matrix = individual_tester::get_matrix(ind);
	std::vector<std::vector<bool>> expect_matrix{
		{false, false, false, false, true, false, false, true, true},
		{false, false, false, false, false, false, true, true, true},
		{false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, true, false, true, false},
		{false, false, false, false, false, false, true, false, false},
		{false, false, false, false, false, false, false, false, true},
		{false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false},
	};
	ASSERT_EQ(expect_matrix.size(), actual_matrix.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_matrix.size(); i++) {
		ASSERT_EQ(expect_matrix[i].size(), actual_matrix[i].size())
			<< "Matrices are of unequal length at index" << i;
		for (unsigned long j = 0; j < expect_matrix[i].size(); j++) {
			if (expect_matrix[i][j]) {
				if (actual_matrix[i][j] == nullptr) {
					auto node_order    = ind.get_node_order();
					auto node_id_order = inverse::inverse_vector(node_order);
					FAIL() << "Matrices differ at index (" << i << "," << j << "), which is ("
						   << node_id_order[i] << "," << node_id_order[j] << ")";
				}
			} else {
				if (actual_matrix[i][j] != nullptr) {
					auto node_order    = ind.get_node_order();
					auto node_id_order = inverse::inverse_vector(node_order);
					FAIL() << "Matrices differ at index (" << i << "," << j << "), which is ("
						   << node_id_order[i] << "," << node_id_order[j] << ")";
				}
			}
		}
	}

	auto actual_out = ind.calculate(std::vector<double>({0.1, 0.2, 0.3}));
	std::vector<double> expect_out{0.39071396, 0.50444657};
	ASSERT_EQ(expect_out.size(), actual_out.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expect_out.size(); i++) {
		EXPECT_FLOAT_EQ(expect_out[i], actual_out[i]) << "Vectors differ at index " << i;
	}
}
} // namespace enn
