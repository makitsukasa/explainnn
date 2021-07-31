#include "enn/edge.h"
#include "lib/randomlib.h"

double enn::edge::sharedWeight = 1.0;

enn::edge::edge() {
	weight = random_distribution(random_engine);
}

enn::edge::edge(double weight) : weight(weight) {}

double enn::edge::calculate(double input) {
	return input * weight;
}

double enn::edge::calculateWithSharedWeight(double input) {
	return input * sharedWeight;
}

double enn::edge::calculateWithSpecifiedWeight(double input, double specifiedWeight) {
	return input * specifiedWeight;
}

namespace enn {
std::ostream &operator<<(std::ostream &os, const edge &edge) {
	os << edge.weight;
	return os;
}
} // namespace enn
