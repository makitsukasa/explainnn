#include "enn/individual.h"
#include "lib/graphlib.h"
#include "lib/vectorlib.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

// 位置を指定してそこに挿入
void enn::individual::update(
	std::vector<unsigned long> source_ids, std::vector<unsigned long> destination_ids,
	std::unordered_map<unsigned long, node> nodes, std::vector<std::vector<edge *>> matrix) {
	if (source_ids.size() + destination_ids.size() > nodes.size() ||
		source_ids.size() + destination_ids.size() > matrix.size()) {
		throw std::runtime_error(
			"size of each arguments are inappropriate in individual::update()");
	}
	auto node_id_order = vectorlib::inverse(node_order);

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
	for (auto &row : this->adjacency_matrix) {
		row.insert(row.begin() + jack, additional_nodes_size, nullptr);
	}
	// rowを挿入
	for (unsigned long i = 0; i < additional_nodes_size; i++) {
		this->adjacency_matrix.insert(
			this->adjacency_matrix.begin() + jack + i, std::vector<edge *>(new_node_size, nullptr));
	}

	// edgeを代入
	for (unsigned long y = 0; y < matrix.size(); y++) {
		auto y_new = node_id_order[node_order_insertion[y]];
		for (unsigned long x = 0; x < matrix[y].size(); x++) {
			auto x_new                           = node_id_order[node_order_insertion[x]];
			this->adjacency_matrix[y_new][x_new] = matrix[y][x];
		}
	}
	// *** 一旦node_id_order[source_id]の後ろに挿入 *** ここまで

	// *** ソート *** ここから
	// 隣接行列のうち入力も出力もhiddenである正方形の領域について
	// bool型の配列を用意
	auto matrix_bool = get_bool_matrix_hidden();
	// argsortっぽい感じ
	auto sorted_indices = topo_sort(matrix_bool);
	// node_order、node_id_orderを更新
	auto old_node_order = std::vector<unsigned long>(node_order);
	auto jack_          = num_input + 1;
	for (unsigned long i = 0; i < this->nodes.size() - jack_ - num_output; i++) {
		node_order[i + jack_] = old_node_order[sorted_indices[i] + jack_];
	}
	auto new_node_id_order = vectorlib::inverse(node_order);
	// matrixを更新
	std::vector<std::vector<edge *>> new_adjacency_matrix(
		this->adjacency_matrix.size(), std::vector<edge *>(this->adjacency_matrix.size()));
	for (unsigned long x = 0; x < this->nodes.size(); x++) {
		for (unsigned long y = 0; y < this->nodes.size(); y++) {
			auto x_new                         = new_node_id_order[old_node_order[x]];
			auto y_new                         = new_node_id_order[old_node_order[y]];
			new_adjacency_matrix[y_new][x_new] = this->adjacency_matrix[y][x];
		}
	}
	this->adjacency_matrix = new_adjacency_matrix;
	// *** ソート *** ここまで
}
