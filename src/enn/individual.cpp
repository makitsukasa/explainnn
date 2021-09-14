#include "enn/individual.h"
#include "lib/toposort.h"
#include "lib/vectorlib.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

enn::individual::individual() {}

enn::individual::individual(unsigned long num_input, unsigned long num_output)
	: num_input(num_input), num_output(num_output) {

	// 初期のsizeは(nIn + nBias + nOut)
	unsigned long size = num_input + 1 + num_output;
	adjacency_matrix.assign(size, std::vector<edge *>(size, nullptr));

	// input node
	for (unsigned long i = 0; i < num_input; i++) {
		add_node(node_type::Input);
	}
	// bias node
	add_node(node_type::Bias);
	// output node
	for (unsigned long i = 0; i < num_output; i++) {
		add_node(node_type::Output);
	}

	// (input, bias)とoutputの間を全部繋ぐ
	for (unsigned long i = 0; i < num_input + 1; i++) {
		for (unsigned long j = num_input + 1; j < size; j++) {
			add_edge(i, j, new edge());
		}
	}
}

std::vector<unsigned long> enn::individual::get_node_order() const {
	return node_order;
}

unsigned long enn::individual::add_node(node_type node_type, unsigned long index) {
	if (node_type == node_type::Hidden) {
		// 計算中に挿入するHiddenはbiasとoutputの間に挿入
		index = std::clamp(index, num_input + 1, (unsigned long)nodes.size());
	} else {
		// コンストラクタで挿入するInput,Bias,Outputは単に末尾に挿入
		index = std::fmin(index, (unsigned long)nodes.size());
	}

	node_order.insert(node_order.begin() + index, nodes.size());
	auto id = nodes.size();
	nodes.insert(std::make_pair(id, node(node_type)));
	return id;
}

void enn::individual::add_node(
	unsigned long source_id, unsigned long destination_id, edge *edge1, edge *edge2) {
	auto new_size      = nodes.size() + 1;
	auto new_node_id   = nodes.size();
	auto node_id_order = vectorlib::inverse(node_order);

	// 明らかに不適な操作は弾く
	if (source_id == destination_id) {
		throw "source_id == destination_id in individual::add_node()";
	}
	if (nodes[source_id].get_type() == node_type::Output) {
		throw "nodes[source_id] is output node in individual::add_node()";
	}
	if (nodes[destination_id].get_type() == node_type::Input) {
		throw "nodes[destination_id] is input node in individual::add_node()";
	}
	if (nodes[destination_id].get_type() == node_type::Bias) {
		throw "nodes[destination_id] is bias node in individual::add_node()";
	}

	// *** 一旦node_id_order[source_id]の後ろに挿入 *** ここから
	auto index = node_id_order[source_id];
	add_node(node_type::Hidden, index + 1);

	// ノードが増えたのでnode_id_orderを再計算
	node_id_order = vectorlib::inverse(node_order);

	// 各rowに挿入
	for (auto &row : adjacency_matrix) {
		row.insert(row.begin() + node_id_order[new_node_id], nullptr);
	}
	// rowを挿入
	adjacency_matrix.insert(
		adjacency_matrix.begin() + node_id_order[new_node_id],
		std::vector<edge *>(new_size, nullptr));

	// edgeを代入
	adjacency_matrix[node_id_order[source_id]][node_id_order[new_node_id]]      = edge1;
	adjacency_matrix[node_id_order[new_node_id]][node_id_order[destination_id]] = edge2;

	// *** 一旦node_id_order[source_id]の後ろに挿入 *** ここまで

	// *** ソート *** ここから
	// 隣接行列のうち入力も出力もhiddenである正方形の領域について
	// bool型の配列を用意
	auto matrix_bool = get_bool_matrix_hidden();

	// argsortっぽい感じ
	auto sorted_indices = topo_sort(matrix_bool);

	// node_orderを更新
	auto old_node_order = std::vector<unsigned long>(node_order);
	auto jack           = num_input + 1;
	for (unsigned long i = 0; i < nodes.size() - jack - num_output; i++) {
		node_order[i + jack] = old_node_order[sorted_indices[i] + jack];
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

void enn::individual::add_edge(unsigned long source_id, unsigned long destination_id, edge *edge) {
	adjacency_matrix[source_id][destination_id] = edge;
}
