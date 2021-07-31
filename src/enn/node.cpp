#include "enn/node.h"
#include <cmath>

enn::node::node(node_type type, activation_function act_func) : type(type), act_func(act_func) {}

int enn::node::get_id() {
	return id;
}

double enn::node::calculate() {
	output = std::tanh(input);
	return output;
}
