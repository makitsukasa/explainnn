#include "lib/graphlib.h"
#include <set>
#include <stdexcept>
#include <vector>

// https://en.wikipedia.org/wiki/Topological_sorting
std::vector<unsigned long> topo_sort(std::vector<std::vector<bool>> matrix) {

	for (unsigned long x = 0; x < matrix.size(); x++) {
		if (matrix[x][x]) {
			throw std::runtime_error("irregal matrix");
		}
	}

	// 入力辺を持たないすべてのノードをsetに入れる
	std::set<unsigned long> set = {};
	for (unsigned long dst = 0; dst < matrix.size(); dst++) {
		auto has_input = false;
		for (unsigned long src = 0; src < matrix[dst].size(); src++) {
			if (matrix[src][dst]) {
				has_input = true;
				break;
			}
		}
		if (!has_input) {
			set.insert(dst);
		}
	}

	// setが空になるまで
	std::vector<unsigned long> ans = {};
	while (!set.empty()) {
		auto node = *set.begin();
		set.erase(set.begin());
		ans.push_back(node);
		// srcがnodeである辺とその先のノードについて、
		for (unsigned long dst = 0; dst < matrix[node].size(); dst++) {
			if (!matrix[node][dst]) continue;

			// 辺を消す
			matrix[node][dst] = false;

			// 他に入力がないノードをsetに入れる
			auto has_input = false;
			for (unsigned long src = 0; src < matrix.size(); src++) {
				if (matrix[src][dst]) {
					has_input = true;
					break;
				}
			}
			if (!has_input) {
				set.insert(dst);
			}
		}
	}

	// 辺が残っているときはソートできなかったとき
	for (unsigned long src = 0; src < matrix.size(); src++) {
		for (unsigned long dst = 0; dst < matrix[src].size(); dst++) {
			if (matrix[src][dst]) {
				throw std::runtime_error("unsortable");
			}
		}
	}

	return ans;
}

// 下のやつで使う再帰関数
// グラフを辿りながらreachablility_matrixを更新する
void calc_reachablility_matrix(
	std::vector<std::vector<bool>> adjacency_matrix,
	std::vector<std::vector<bool>> &reachablility_matrix, std::vector<bool> &is_visited,
	unsigned long source_node) {
	// 計算済みのノードなら特に何もしない
	if (is_visited[source_node]) return;

	// 計算済みとしてマーク
	is_visited[source_node] = true;

	// 隣接行列のsource_node行目を舐めて
	for (unsigned long i = 0; i < adjacency_matrix.size(); i++) {
		// source_node→iの辺がないなら何もしない
		if (!adjacency_matrix[source_node][i]) continue;

		// source_node→iの辺があるならばiはsource_nodeから到達可能
		reachablility_matrix[source_node][i] = true;

		// 再帰 reachablility_matrixのi行目(を含む何行か)が更新される
		calc_reachablility_matrix(adjacency_matrix, reachablility_matrix, is_visited, i);

		// 更新されたi行目を参照してsource_node行目を更新
		// source_node→iの辺があるとき、iからjに到達可能であればsource_nodeからjに到達可能
		for (unsigned long j = 0; j < adjacency_matrix.size(); j++) {
			reachablility_matrix[source_node][j] =
				reachablility_matrix[source_node][j] || adjacency_matrix[i][j];
		}
	}
}

// 隣接行列から到達可能性行列を求める
std::vector<std::vector<bool>> calc_reachablility_matrix(
	std::vector<std::vector<bool>> adjacency_matrix, std::vector<unsigned long> source_node_ids) {
	std::vector<std::vector<bool>> reachablility_matrix(
		adjacency_matrix.size(), std::vector<bool>(adjacency_matrix.size()));
	std::vector<bool> is_visited(adjacency_matrix.size(), false);
	for (auto i : source_node_ids) {
		calc_reachablility_matrix(adjacency_matrix, reachablility_matrix, is_visited, i);
	}
	return reachablility_matrix;
}

// ノードidが到達可能かどうかを返す
// ここでは(calc_reachablility_matrix()とは違って)source_node_idsに含まれるノードは到達可能とする
std::vector<bool> get_reachable_node_table(
	std::vector<std::vector<bool>> adjacency_matrix, std::vector<unsigned long> source_node_ids) {
	std::vector<bool> reachable_node_table(adjacency_matrix.size(), false);
	auto reachablility_matrix = calc_reachablility_matrix(adjacency_matrix, source_node_ids);

	// reachability_matrixの列ごとの論理和
	for (unsigned long y = 0; y < reachablility_matrix.size(); y++) {
		for (unsigned long x = 0; x < reachablility_matrix.size(); x++) {
			reachable_node_table[x] = reachable_node_table[x] || reachablility_matrix[y][x];
		}
	}

	// source_node_idsに含まれるノードは到達可能とする
	for (unsigned long x = 0; x < reachablility_matrix.size(); x++) {
		reachable_node_table[x] =
			reachable_node_table[x] ||
			std::find(source_node_ids.begin(), source_node_ids.end(), x) != source_node_ids.end();
	}
	return reachable_node_table;
}
