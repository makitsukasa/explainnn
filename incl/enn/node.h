#pragma once

#include <iostream>

namespace enn {
enum class node_type {
	Input,
	Output,
	Hidden,
	Bias,
};

enum class activation_function {
	Linear,
	UnsignedStep,
	Sine,
	Gausian,
	HyperbolicTangent,
	UnsignedSigmoid,
	Inverse,
	Absolute,
	Relu,
	Cosine,
	Squared,
};

class node {
	friend struct std::hash<enn::node>;

private:
	int id;
	node_type type;
	activation_function act_func;

public:
	double input;
	double output;

	node(
		node_type type               = node_type::Hidden,
		activation_function act_func = activation_function::Linear);
	int get_id();
	node_type get_type();
	double calculate();
};

} // namespace enn

// unorder_map用のハッシュ関数
// https://suzulang.com/cpp-unordered_map-my-type/
namespace std {
template <> struct hash<enn::node_type> {
public:
	size_t operator()(const enn::node_type &node_type) const {
		std::size_t seed = 0;
		seed ^= std::hash<int>()((int)node_type) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

template <> struct hash<enn::activation_function> {
public:
	size_t operator()(const enn::activation_function &activation_function) const {
		std::size_t seed = 0;
		seed ^= std::hash<int>()((int)activation_function) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

template <> struct hash<enn::node> {
public:
	size_t operator()(const enn::node &node) const {
		std::size_t seed = 0;
		seed ^= std::hash<int>()(node.id) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<enn::node_type>()(node.type) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<enn::activation_function>()(node.act_func) + 0x9e3779b9 + (seed << 6) +
				(seed >> 2);
		return seed;
	}
};
} // namespace std
