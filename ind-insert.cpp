void enn::individual::insert(
	std::vector<unsigned long> source_ids, std::vector<unsigned long> destination_ids,
	std::unordered_map<unsigned long, node> nodes, std::vector<std::vector<edge *>> matrix) {
	if (source_ids.size() + destination_ids.size() > nodes.size() ||
		source_ids.size() + destination_ids.size() > matrix.size()) {
		throw std::runtime_error(
			"size of each arguments are inappropriate in individual::insert()");
	}
	auto jack                  = node_id_order[source_id] + 1;
	auto additional_nodes_size = nodes.size() - source_ids.size() + destination_ids.size();
	auto new_nodes_size        = nodes.size() + additional_nodes_size;
	// *** 一旦node_id_order[source_id]の後ろに挿入 *** ここから
	for (auto i = 0; i < new_nodes_size; i++) {
		add_node(node_type::Hidden, jack + i);
	}
	// ノードが増えたのでnode_id_orderを再計算
	node_id_order = inverse::inverse_vector(node_order);
	// 挿入する行列のnode_orderも計算
	std::vector<unsigned long, additional_nodes_size> hidden_ids;
	std::iota(hidden_ids.begin(), hidden_ids.end(), new_nodes_size - additional_nodes_size);
	node_order_insertion = concat(source_ids, hidden_ids, destination_ids);

	// 各rowに挿入
	for (auto &row : adjacency_matrix) {
		row.insert(row.begin() + node_id_order[new_node_id], additional_nodes_size, nullptr);
	}
	// rowを挿入
	adjacency_matrix.insert(
		adjacency_matrix.begin() + node_id_order[new_node_id],
		std::vector<edge *>(new_nodes_size, additional_nodes_size, nullptr));
	// edgeを代入
	for (auto y = 0; y < matrix.size(); y++) {
		auto y_ = node_id_order[node_order_insertion[y]];
		for (auto x = 0; x < matrix[y].size(); x++) {
			auto x_ = node_id_order[node_order_insertion[x]];
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
	auto jack           = num_input + 1;
	for (unsigned long i = 0; i < nodes.size() - jack - num_output; i++) {
		node_order[i + jack] = old_node_order[sorted_indices[i] + jack];
	}
	// matrixを更新
	auto new_node_id_order = inverse::inverse_vector(node_order);
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

