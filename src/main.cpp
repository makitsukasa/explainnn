#include "enn/individual.h"
#include "lib/randomlib.h"
#include <iostream>

int main() {
	auto ind    = enn::individual(3, 2);
	auto output = ind.calculate(std::vector<double>({-0.5, -0.5, -0.5}));
	for (auto x : output) {
		std::cout << x << ",";
	}
	std::cout << std::endl;

	ind.add_node(0, 4, new enn::edge(0.2), new enn::edge(0.2));

	auto output_added = ind.calculate(std::vector<double>({-0.5, -0.5, -0.5}));
	for (auto x : output_added) {
		std::cout << x << ",";
	}
	std::cout << std::endl;
}
