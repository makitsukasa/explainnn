#pragma once

#include <vector>

std::vector<unsigned long> topo_sort(std::vector<std::vector<bool>> matrix);
std::vector<std::vector<bool>> calc_reachablility_matrix(
	std::vector<std::vector<bool>> adjacency_matrix, std::vector<unsigned long> source_node_ids);
std::vector<unsigned long> get_unreachable_nodes(
	std::vector<std::vector<bool>> adjacency_matrix, std::vector<unsigned long> source_node_ids);
