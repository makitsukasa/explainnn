#pragma once

#include <iostream>
#include <random>

namespace enn {
class edge {
	friend class edge_tester;

private:
	static double sharedWeight;
	double weight;
	bool enabled;

public:
	edge();
	edge(double weight);
	edge(edge &obj);
	friend std::ostream &operator<<(std::ostream &os, const edge &edge);
	double calculate(double input);
	double calculateWithSharedWeight(double input);
	double calculateWithSpecifiedWeight(double input, double weight);
};

} // namespace enn
