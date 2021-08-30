#include "enn/individual.h"
#include "lib/toposort.h"
#include "lib/vectorlib.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

void enn::individual::insert(
	std::vector<unsigned long> source_ids, std::vector<unsigned long> destination_ids,
	std::unordered_map<unsigned long, node> nodes, std::vector<std::vector<edge *>> matrix) {
	if (source_ids.size() + destination_ids.size() > nodes.size() ||
		source_ids.size() + destination_ids.size() > matrix.size()) {
		throw std::runtime_error(
			"size of each arguments are inappropriate in individual::insert()");
	}
	auto node_id_order         = vectorlib::inverse(node_order);
	auto jack                  = node_id_order[source_ids.back()] + 1;
	auto additional_nodes_size = nodes.size() - source_ids.size() + destination_ids.size();
	auto new_nodes_size        = nodes.size() + additional_nodes_size;
	// *** 一旦node_id_order[source_id]の後ろに挿入 *** ここから
	for (unsigned long i = 0; i < new_nodes_size; i++) {
		add_node(node_type::Hidden, jack + i);
	}
	// ノードが増えたのでnode_id_orderを再計算
	node_id_order = vectorlib::inverse(node_order);
	// 挿入する行列のnode_orderも計算
	std::vector<unsigned long> hidden_ids(additional_nodes_size);
	std::iota(hidden_ids.begin(), hidden_ids.end(), new_nodes_size - additional_nodes_size);
	auto node_order_insertion = vectorlib::concat(source_ids, hidden_ids, destination_ids);

	// 各rowに挿入
	for (auto &row : adjacency_matrix) {
		row.insert(row.begin() + jack, additional_nodes_size, nullptr);
	}
	// rowを挿入
	for (unsigned long i = 0; i < additional_nodes_size; i++) {
		adjacency_matrix.insert(
			adjacency_matrix.begin() + jack + i,
			std::vector<edge *>(additional_nodes_size, nullptr));
	}
	// edgeを代入
	for (unsigned long y = 0; y < matrix.size(); y++) {
		auto y_ = node_id_order[node_order_insertion[y]];
		for (unsigned long x = 0; x < matrix[y].size(); x++) {
			auto x_                  = node_id_order[node_order_insertion[x]];
			adjacency_matrix[y_][x_] = matrix[y][x];
		}
	}
	// *** 一旦node_id_order[source_id]の後ろに挿入 *** ここまで

	// *** ソート *** ここから
	// 隣接行列のうち入力も出力もhiddenである正方形の領域について
	// bool型の配列を用意
	auto matrix_bool = get_bool_matrix_hidden();
	// argsortっぽい感じ
	auto sorted_indices = topo_sort(matrix_bool);
	// node_orderを更新
	auto old_node_order = std::vector<unsigned long>(node_order);
	auto jack_          = num_input + 1;
	for (unsigned long i = 0; i < nodes.size() - jack_ - num_output; i++) {
		node_order[i + jack_] = old_node_order[sorted_indices[i] + jack_];
	}
	// matrixを更新
	auto new_node_id_order = vectorlib::inverse(node_order);
	std::vector<std::vector<edge *>> new_adjacency_matrix(
		adjacency_matrix.size(), std::vector<edge *>(adjacency_matrix.size()));
	for (unsigned long x = 0; x < nodes.size(); x++) {
		for (unsigned long y = 0; y < nodes.size(); y++) {
			auto x_new                         = new_node_id_order[old_node_order[x]];
			auto y_new                         = new_node_id_order[old_node_order[y]];
			new_adjacency_matrix[y_new][x_new] = adjacency_matrix[y][x];
		}
	}
	adjacency_matrix = new_adjacency_matrix;
	// *** ソート *** ここまで
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
