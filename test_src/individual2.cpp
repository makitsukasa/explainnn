#include "enn/individual.h"
#include "lib/randomlib.h"
#include <gtest/gtest.h>

TEST(individual_tester, update_same_seed_hidden_as_is) {
	std::vector<unsigned long> source_ids              = {0};
	std::vector<unsigned long> destination_ids         = {4};
	std::unordered_map<unsigned long, enn::node> nodes = {
		{0, enn::node(enn::node_type::Input)},
		{1, enn::node(enn::node_type::Hidden)},
		{2, enn::node(enn::node_type::Output)},
	};
	std::vector<std::vector<enn::edge *>> matrix;
	matrix.assign(3, std::vector<enn::edge *>(3, nullptr));
	matrix[0][1] = new enn::edge(0.1);
	matrix[1][2] = new enn::edge(0.1);

	enn::random_engine = std::mt19937(6700417);
	auto ind1          = enn::individual(3, 2);
	ind1.update(source_ids, destination_ids, nodes, matrix);
	auto out1 = ind1.calculate(std::vector<double>({0.1, 0.2, 0.3}));

	enn::random_engine = std::mt19937(6700417);
	auto ind2          = enn::individual(3, 2);
	ind2.update(source_ids, destination_ids, nodes, matrix);
	auto out2 = ind2.calculate(std::vector<double>({0.1, 0.2, 0.3}));

	ASSERT_EQ(out1.size(), out2.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < out1.size(); i++) {
		EXPECT_FLOAT_EQ(out1[i], out2[i]) << "Vectors differ at index " << i;
	}
}

TEST(individual_tester, update_same_seed_hidden_sort) {
	std::vector<unsigned long> source_ids_1              = {0};
	std::vector<unsigned long> destination_ids_1         = {2};
	std::unordered_map<unsigned long, enn::node> nodes_1 = {
		{0, enn::node(enn::node_type::Input)},
		{1, enn::node(enn::node_type::Hidden)},
		{2, enn::node(enn::node_type::Hidden)},
		{3, enn::node(enn::node_type::Output)},
	};
	std::vector<std::vector<enn::edge *>> matrix_1;
	matrix_1.assign(4, std::vector<enn::edge *>(4, nullptr));
	matrix_1[0][1] = new enn::edge(0.51);
	matrix_1[0][2] = new enn::edge(0.52);
	matrix_1[1][2] = new enn::edge(0.53);
	matrix_1[1][3] = new enn::edge(0.54);
	matrix_1[2][3] = new enn::edge(0.55);

	std::vector<unsigned long> source_ids              = {4};
	std::vector<unsigned long> destination_ids         = {3};
	std::unordered_map<unsigned long, enn::node> nodes = {
		{0, enn::node(enn::node_type::Input)},
		{2, enn::node(enn::node_type::Output)},
	};
	std::vector<std::vector<enn::edge *>> matrix;
	matrix.assign(2, std::vector<enn::edge *>(2, nullptr));
	matrix[0][1] = new enn::edge(0.56);

	enn::random_engine = std::mt19937(6700417);
	auto ind1          = enn::individual(1, 1);
	ind1.update(source_ids_1, destination_ids_1, nodes_1, matrix_1);
	ind1.update(source_ids, destination_ids, nodes, matrix);
	auto out1 = ind1.calculate(std::vector<double>({0.59}));

	enn::random_engine = std::mt19937(6700417);
	auto ind2          = enn::individual(1, 1);
	ind2.update(source_ids_1, destination_ids_1, nodes_1, matrix_1);
	ind2.update(source_ids, destination_ids, nodes, matrix);
	auto out2 = ind2.calculate(std::vector<double>({0.59}));

	ASSERT_EQ(out1.size(), out2.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < out1.size(); i++) {
		EXPECT_FLOAT_EQ(out1[i], out2[i]) << "Vectors differ at index " << i;
	}
}

TEST(individual_tester, update_hidden_sort) {
	std::vector<unsigned long> source_ids_actual_1              = {0};
	std::vector<unsigned long> destination_ids_actual_1         = {2};
	std::unordered_map<unsigned long, enn::node> nodes_actual_1 = {
		{0, enn::node(enn::node_type::Input)},
		{1, enn::node(enn::node_type::Hidden)},
		{2, enn::node(enn::node_type::Hidden)},
		{3, enn::node(enn::node_type::Output)},
	};
	std::vector<std::vector<enn::edge *>> matrix_actual_1;
	matrix_actual_1.assign(4, std::vector<enn::edge *>(4, nullptr));
	matrix_actual_1[0][1] = new enn::edge(0.51);
	matrix_actual_1[0][2] = new enn::edge(0.52);
	matrix_actual_1[1][2] = new enn::edge(0.53);
	matrix_actual_1[1][3] = new enn::edge(0.54);
	matrix_actual_1[2][3] = new enn::edge(0.55);

	std::vector<unsigned long> source_ids_actual_2              = {4};
	std::vector<unsigned long> destination_ids_actual_2         = {3};
	std::unordered_map<unsigned long, enn::node> nodes_actual_2 = {
		{0, enn::node(enn::node_type::Input)},
		{2, enn::node(enn::node_type::Output)},
	};
	std::vector<std::vector<enn::edge *>> matrix_actual_2;
	matrix_actual_2.assign(2, std::vector<enn::edge *>(2, nullptr));
	matrix_actual_2[0][1] = new enn::edge(0.56);

	std::vector<unsigned long> source_ids_expected              = {0};
	std::vector<unsigned long> destination_ids_expected         = {2};
	std::unordered_map<unsigned long, enn::node> nodes_expected = {
		{0, enn::node(enn::node_type::Input)},
		{1, enn::node(enn::node_type::Hidden)},
		{2, enn::node(enn::node_type::Hidden)},
		{3, enn::node(enn::node_type::Output)},
	};
	std::vector<std::vector<enn::edge *>> matrix_expected;
	matrix_expected.assign(4, std::vector<enn::edge *>(4, nullptr));
	matrix_expected[0][1] = new enn::edge(0.52);
	matrix_expected[0][2] = new enn::edge(0.51);
	matrix_expected[1][2] = new enn::edge(0.56);
	matrix_expected[1][3] = new enn::edge(0.55);
	matrix_expected[2][3] = new enn::edge(0.54);

	enn::random_engine = std::mt19937(6700417);
	auto ind_actual    = enn::individual(1, 1);
	ind_actual.update(
		source_ids_actual_1, destination_ids_actual_1, nodes_actual_1, matrix_actual_1);
	ind_actual.update(
		source_ids_actual_2, destination_ids_actual_2, nodes_actual_2, matrix_actual_2);
	auto actual = ind_actual.calculate(std::vector<double>({0.59}));

	enn::random_engine = std::mt19937(6700417);
	auto ind_expected  = enn::individual(1, 1);
	ind_expected.update(
		source_ids_expected, destination_ids_expected, nodes_expected, matrix_expected);
	auto expected = ind_expected.calculate(std::vector<double>({0.59}));

	ASSERT_EQ(actual.size(), expected.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < expected.size(); i++) {
		EXPECT_FLOAT_EQ(actual[i], expected[i]) << "Vectors differ at index " << i;
	}
}
