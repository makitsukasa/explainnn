#include "enn/individual.h"
#include "lib/graphlib.h"
#include "lib/randomlib.h"
#include "lib/vectorlib.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

// ランダムな位置に挿入
void enn::individual::update(
	std::unordered_map<unsigned long, node> nodes, std::vector<std::vector<edge *>> matrix) {
	unsigned long num_source      = 0;
	unsigned long num_destination = 0;
	for (auto pair : nodes) {
		switch (pair.second.get_type()) {
		case node_type::Input:
			num_source++;
			break;
		case node_type::Output:
			num_destination++;
			break;
		default:
			break;
		}
	}

	std::vector<unsigned long> source_candidates{};
	std::vector<unsigned long> destination_candidates(this->num_output);
	std::iota(destination_candidates.begin(), destination_candidates.end(), this->num_input + 1);

	std::vector<unsigned long> source_ids;
	std::vector<unsigned long> destination_ids;
	while (true) {
		// (隠れ、出力)ノードからdestinationノードをランダムに
		std::sample(
			destination_candidates.begin(), destination_candidates.end(),
			std::back_inserter(destination_ids), num_destination, random_engine);

		// (入力、バイアス、隠れ)ノードから上で選んだのと合うsourceノードをランダムに
		// bool型にした隣接行列を用意
		std::vector<std::vector<bool>> adjacency_matrix_bool(
			adjacency_matrix.size(), std::vector<bool>(adjacency_matrix.size()));
		for (unsigned long y = 0; y < adjacency_matrix.size(); y++) {
			for (unsigned long x = 0; x < adjacency_matrix.size(); x++) {
				adjacency_matrix_bool[y][x] = adjacency_matrix[y][x] ? true : false;
			}
		}
		// destinationから到達可能(=destinationより下流)のノードはsourceになり得ない
		auto table = get_reachable_node_table(adjacency_matrix_bool, destination_ids);
		for (unsigned long i = 0; i < this->nodes.size() - this->num_output; i++) {
			if (table[i]) continue; // 到達可能なら弾く
			source_candidates.push_back(i);
		}

		std::sample(
			source_candidates.begin(), source_candidates.end(), std::back_inserter(source_ids),
			num_source, random_engine);

		// 本当はここでupdateできるかどうかみたいなのをチェック
		// ここじゃないかもしれない
		if (source_ids.size() == num_source) {
			break;
		}
	}
	this->update(source_ids, destination_ids, nodes, matrix);
}

std::vector<double> enn::individual::calculate(std::vector<double> input) {
	// 入力層のノードにinputを代入
	for (unsigned long i = 0; i < num_input; i++) {
		nodes[node_order[i]].input = input[i];
	}
	// バイアスノードの入力は1を代入
	nodes[node_order[num_input]].input = 1.0;
	// 隠れ層、出力層のノードには一旦0を代入し、下で加算していく
	for (unsigned long i = num_input + 1; i < nodes.size(); i++) {
		nodes[node_order[i]].input = 0.0;
	}

	// 計算
	for (unsigned long i = 0; i < nodes.size(); i++) {
		auto output = nodes[node_order[i]].calculate();
		for (unsigned long j = i; j < nodes.size(); j++) {
			if (adjacency_matrix[i][j] == nullptr) continue;
			nodes[node_order[j]].input += adjacency_matrix[i][j]->calculate(output);
		}
	}

	// 返り値
	auto ret = std::vector<double>(num_output);
	for (unsigned long i = 0; i < num_output; i++) {
		ret[i] = nodes[node_order[nodes.size() - num_output + i]].output;
	}
	return ret;
}

std::vector<std::vector<bool>> enn::individual::get_bool_matrix_hidden() {
	std::vector<std::vector<bool>> ret = {};

	// num_input + 1個目(hiddenの0個目)が0になるように下駄を脱がす
	auto jack = num_input + 1;

	for (unsigned long x = 0; x < nodes.size() - jack - num_output; x++) {
		std::vector<bool> row;
		for (unsigned long y = 0; y < nodes.size() - jack - num_output; y++) {
			row.push_back(adjacency_matrix[x + jack][y + jack] == nullptr ? false : true);
		}
		ret.push_back(row);
	}
	return ret;
}

namespace enn {
std::ostream &operator<<(std::ostream &ostream, const individual &individual) {
	ostream << "   ";
	for (unsigned long i = 0; i < individual.nodes.size(); i++) {
		ostream << "   " << std::setw(2) << std::setfill(' ') << individual.get_node_order()[i];
	}
	ostream << std::endl;

	for (unsigned long i = 0; i < individual.nodes.size(); i++) {
		ostream << std::setw(2) << std::setfill(' ') << individual.get_node_order()[i] << "  ";
		for (unsigned long j = 0; j < individual.nodes.size(); j++) {
			if (individual.adjacency_matrix[i][j]) {
				ostream << std::fixed << std::setprecision(2) << *individual.adjacency_matrix[i][j]
						<< ",";
			} else {
				ostream << "____,";
			}
		}
		ostream << std::endl;
	}
	return ostream;
}
} // namespace enn
