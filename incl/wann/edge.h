#pragma once

#include <iostream>

namespace enn {
class edge {
private:
	// static double sharedWeight;
	double weight;
	bool enabled;

public:
	edge(double weight);
	friend std::ostream &operator<<(std::ostream &os, const edge &edge);
	double calculate(double input);
	// double calculateWithSharedWeight(double input);
};

} // namespace enn
