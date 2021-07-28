#pragma once

#include <iostream>
#include <random>

namespace enn {
class edge {
private:
	static double sharedWeight;
	static std::mt19937 random_engine;
	double weight;
	bool enabled;

public:
	edge();
	edge(double weight);
	friend std::ostream &operator<<(std::ostream &os, const edge &edge);
	double calculate(double input);
	double calculateWithSharedWeight(double input);
	double calculateWithSpecifiedWeight(double input, double weight);
};

} // namespace enn
