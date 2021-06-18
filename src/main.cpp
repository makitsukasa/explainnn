#include "wann/individual.h"
#include <iostream>

int main() {
	auto ind = enn::individual(3, 2);
	for (auto x : ind.calculate(std::vector<double>({0.5, 0.5, 0.5}))) {
		std::cout << x << ",";
	}
	std::cout << std::endl;

	ind.add_node(0, 4, new enn::edge(0.2), new enn::edge(0.2));

	for (auto x : ind.calculate(std::vector<double>({0.5, 0.5, 0.5}))) {
		std::cout << x << ",";
	}
	std::cout << std::endl;
}
