#include "../../incl/wann/edge.h"

enn::edge::edge(double weight) : weight(weight) {}

double enn::edge::calculate(double input) {
	return input * weight;
}

// double enn::edge::calculateWithSharedWeight(double input) {
// 	return input * sharedWeight;
// }

namespace enn {
std::ostream &operator<<(std::ostream &os, const edge &edge) {
	os << edge.weight;
	return os;
}
} // namespace enn
