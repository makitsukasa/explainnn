#include "enn/individual.h"
#include "lib/randomlib.h"
#include <iostream>

int main() {
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
	for (auto i : out1) {
		std::cout << i << ",";
	}
	std::cout << std::endl;
}
