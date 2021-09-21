#include "enn/individual.h"
#include "lib/randomlib.h"
#include <iostream>

int main() {
	enn::random_engine = std::mt19937(6700417);

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

	const auto num_input       = 3;
	const auto num_output      = 1;
	const auto population_size = 3;
	const auto num_generation  = 1;

	// 挿入で使うミニNN
	std::tuple<std::unordered_map<unsigned long, enn::node>, std::vector<std::vector<enn::edge *>>>
		known_networks = std::tuple{nodes, matrix};

	// 初期個体の生成
	auto population = std::vector<enn::individual>(population_size);
	for (auto &individual : population) {
		individual = enn::individual(num_input, num_output);
	}

	// 以下繰返し
	for (auto gen = 0; gen < num_generation; gen++) {
		// 評価
		const std::vector<double> input = {1, 1, 1};
		for (auto &individual : population) {
			auto output        = individual.calculate(input);
			individual.fitness = std::abs(output[0] - 0.5); // 仮の評価関数
		}

		// 並べ替えて選定
		std::sort(
			population.begin(), population.end(),
			[](const enn::individual &lhs, const enn::individual &rhs) {
				return lhs.fitness < rhs.fitness;
			});
		auto parents = std::vector(population.begin(), population.begin() + 2);

		// 変異
		// 一番良かった個体から変異させて2個体、二番目に良かった個体から変異させて1個体を生成し、計3個体を生成
		// 完全にお試し用のものなので後で考えなおす
		std::vector<enn::individual> new_population;
		new_population.push_back(parents[0]);
		std::cout << new_population[0].calculate({0.5, 0.6, 0.7})[0] << std::endl;
		new_population[0].update(nodes, matrix);
		std::cout << new_population[0].calculate({0.5, 0.6, 0.7})[0] << std::endl;
		new_population.push_back(parents[0]);
		new_population.push_back(parents[1]);

		population = new_population;
	}
}
