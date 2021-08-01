#include "enn/individual.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

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

void enn::individual::add_node(node_type node_type, unsigned long index) {
	if (node_type == node_type::Hidden) {
		// 計算中に挿入するHiddenはbiasとoutputの間に挿入
		index = std::clamp(index, num_input + 1, (unsigned long)nodes.size());
	} else {
		// コンストラクタで挿入するInput,Bias,Outputは単に末尾に挿入
		index = std::fmin(index, (unsigned long)nodes.size());
	}

	node_order.insert(node_order.begin() + index, nodes.size());
	nodes.insert(std::make_pair(nodes.size(), node(node_type)));
}

void enn::individual::add_node(
	unsigned long source_id, unsigned long destination_id, edge *edge1, edge *edge2) {
	auto new_size      = nodes.size() + 1;
	auto new_node_id   = nodes.size();
	auto node_id_order = inverse_vector(node_order);

	if (source_id == destination_id) {
		throw "source_id == destination_id in individual::add_node()";
	} else if (node_id_order[source_id] > node_id_order[destination_id]) {
		// ソートが要る
		throw "Not implemented";
	} else {
		// ソートが要らない、node_id_order[source_id]の後ろに挿入すればいい
		auto index = node_id_order[source_id];
		add_node(node_type::Hidden, index + 1);

		// ノードが増えたのでnode_id_orderを再計算
		node_id_order = inverse_vector(node_order);

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
	}
}

void enn::individual::add_edge(unsigned long source_id, unsigned long destination_id, edge *edge) {
	adjacency_matrix[source_id][destination_id] = edge;
}

std::vector<double> enn::individual::calculate(std::vector<double> input) {
	// 入力層のノードにinputを代入
	for (unsigned long i = 0; i < num_input; i++) {
		nodes[node_order[i]].input = input[i];
	}
	// バイアスノードの入力は1を代入
	nodes[node_order[num_input]].input = 1.0;
	// 出力層のノードには一旦0を代入し、下で加算していく
	for (unsigned long i = num_input + 1; i < num_input + 1 + num_output; i++) {
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
