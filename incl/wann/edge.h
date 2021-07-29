#pragma once

#include <iostream>
#include <random>

namespace enn {
class edge {
private:
public:
	static double sharedWeight;
	double weight;
	bool enabled;

	edge();
	edge(double weight);
	friend std::ostream &operator<<(std::ostream &os, const edge &edge);
	double calculate(double input);
	double calculateWithSharedWeight(double input);
	double calculateWithSpecifiedWeight(double input, double weight);
};

} // namespace enn
