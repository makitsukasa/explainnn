#include "lib/toposort.h"
#include "enn/edge.h"
#include <set>

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
std::vector<std::vector<bool>> calc_reachablility_nodes(
	std::vector<std::vector<bool>> adjacency_matrix,
	std::vector<std::vector<bool>> reachablility_matrix, std::vector<bool> is_visited) {
	if (s == g) return 1;
	if (visited[s]) return 0;
	visited[s] = 1;
	for (int i = 0; i < N; i++) {
		if (a[s][i] && calc_reachablility_nodes(i, g)) return 1;
	}
	return 0;
}

// 到達可能性行列
std::vector<std::vector<bool>> calc_reachablility_matrix(
	std::vector<std::vector<bool>> adjacency_matrix, std::vector<unsigned long> source_ids) {
	std::vector<bool> is_visited(adjacency_matrix.size(), false);
	for (auto s : source_ids) {
		calc_reachablility_matrix(adjacency_matrix, reachablility_matrix, is_visited);
	}
	return reachablility_matrix;
}
