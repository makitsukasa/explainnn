#pragma once

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
	double calculate();
};

} // namespace enn
