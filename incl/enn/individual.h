#pragma once

#include "edge.h"
#include "lib/inverse.h"
#include "node.h"
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace enn {
class individual {
	friend class test_individual;

private:
	// 値はnode、キーはそのnodeの.id
	std::unordered_map<unsigned long, node> nodes;
	// キーは隣接行列の何番目か、値はnode.id
	std::vector<unsigned long> node_order;
	// 隣接行列
	std::vector<std::vector<edge *>> adjacency_matrix;
	unsigned long num_input;
	unsigned long num_output;

public:
	individual(unsigned long num_input, unsigned long num_output);
	friend std::ostream &operator<<(std::ostream &os, const individual &ind);
	std::vector<unsigned long> get_node_order() const;
	void add_node(node_type nodeType, unsigned long index = ULONG_MAX);
	void add_node(unsigned long source_id, unsigned long destination_id, edge *edge1, edge *edge2);
	void add_edge(unsigned long source_id, unsigned long destination_id, edge *edge);
	std::vector<double> calculate(std::vector<double> input);
};
} // namespace enn
