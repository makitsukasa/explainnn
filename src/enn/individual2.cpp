#include "enn/individual.h"
#include "lib/toposort.h"
#include "lib/vectorlib.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

void enn::individual::update(
	std::vector<unsigned long> source_ids, std::vector<unsigned long> destination_ids,
	std::unordered_map<unsigned long, node> nodes, std::vector<std::vector<edge *>> matrix) {
	if (source_ids.size() + destination_ids.size() > nodes.size() ||
		source_ids.size() + destination_ids.size() > matrix.size()) {
		throw std::runtime_error(
			"size of each arguments are inappropriate in individual::insert()");
	}
	auto node_id_order         = vectorlib::inverse(node_order);
	auto jack                  = std::max(node_id_order[source_ids.back()] + 1, num_input + 1);
	auto additional_nodes_size = nodes.size() - source_ids.size() - destination_ids.size();
	auto new_node_size         = this->nodes.size() + additional_nodes_size;
	// *** 一旦node_id_order[source_id]の後ろに挿入 *** ここから
	std::vector<unsigned long> hidden_ids(additional_nodes_size);
	for (unsigned long i = 0; i < additional_nodes_size; i++) {
		auto id       = add_node(node_type::Hidden, jack + i);
		hidden_ids[i] = id;
	}
	// ノードが増えたのでnode_id_orderを再計算
	node_id_order = vectorlib::inverse(node_order);
	// 挿入する行列のnode_orderも計算
	auto node_order_insertion = vectorlib::concat(source_ids, hidden_ids, destination_ids);

	// 各rowに挿入
	for (auto &row : adjacency_matrix) {
		row.insert(row.begin() + jack, additional_nodes_size, nullptr);
	}
	// rowを挿入
	for (unsigned long i = 0; i < additional_nodes_size; i++) {
		adjacency_matrix.insert(
			adjacency_matrix.begin() + jack + i, std::vector<edge *>(new_node_size, nullptr));
	}

	// std::cout << "node_id_order: ";
	// for (auto i : node_id_order) {
	// 	std::cout << i;
	// }
	// std::cout << std::endl;
	// std::cout << "node_order_insertion: ";
	// for (auto i : node_order_insertion) {
	// 	std::cout << i;
	// }
	// std::cout << std::endl;

	// for (auto row : adjacency_matrix) {
	// 	for (auto i : row) {
	// 		std::cout << (i ? "e" : "_");
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;

	// edgeを代入
	for (unsigned long y = 0; y < matrix.size(); y++) {
		auto y_new = node_id_order[node_order_insertion[y]];
		for (unsigned long x = 0; x < matrix[y].size(); x++) {
			auto x_new                     = node_id_order[node_order_insertion[x]];
			adjacency_matrix[y_new][x_new] = matrix[y][x];
			// std::cout << y_new << x_new << y << x << ":" << (matrix[y][x] ? "e" : "_") << ",";
		}
	}
	// std::cout << std::endl;

	// for (auto row : adjacency_matrix) {
	// 	for (auto i : row) {
	// 		std::cout << (i ? "e" : "_");
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;
	// *** 一旦node_id_order[source_id]の後ろに挿入 *** ここまで

	// *** ソート *** ここから
	// 隣接行列のうち入力も出力もhiddenである正方形の領域について
	// bool型の配列を用意
	auto matrix_bool = get_bool_matrix_hidden();
	// for (auto row : matrix_bool) {
	// 	for (auto i : row) {
	// 		std::cout << (i ? "1" : "_");
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;
	// argsortっぽい感じ
	auto sorted_indices = topo_sort(matrix_bool);
	// for (auto i : sorted_indices) {
	// 	std::cout << i << ",";
	// }
	// std::cout << std::endl;
	// node_orderを更新
	auto old_node_order = std::vector<unsigned long>(node_order);
	auto jack_          = num_input + 1;
	for (unsigned long i = 0; i < this->nodes.size() - jack_ - num_output; i++) {
		// std::cout << i << "+" << jack_ << ":" << sorted_indices[i] << "+" << jack_ << std::endl;
		node_order[i + jack_] = old_node_order[sorted_indices[i] + jack_];
	}
	// matrixを更新
	auto new_node_id_order = vectorlib::inverse(node_order);
	std::vector<std::vector<edge *>> new_adjacency_matrix(
		adjacency_matrix.size(), std::vector<edge *>(adjacency_matrix.size()));
	for (unsigned long x = 0; x < this->nodes.size(); x++) {
		for (unsigned long y = 0; y < this->nodes.size(); y++) {
			auto x_new                         = new_node_id_order[old_node_order[x]];
			auto y_new                         = new_node_id_order[old_node_order[y]];
			new_adjacency_matrix[y_new][x_new] = adjacency_matrix[y][x];
			// std::cout << y_new << x_new << y << x << ",";
		}
	}
	// std::cout << std::endl;
	// for (auto row : adjacency_matrix) {
	// 	for (auto i : row) {
	// 		std::cout << (i ? "e" : "_");
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;
	// for (auto row : new_adjacency_matrix) {
	// 	for (auto i : row) {
	// 		std::cout << (i ? "e" : "_");
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;
	adjacency_matrix = new_adjacency_matrix;
	// *** ソート *** ここまで
}
