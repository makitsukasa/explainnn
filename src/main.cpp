#include "enn/individual.h"
#include "lib/randomlib.h"
#include <iostream>

int main() {
	auto ind     = enn::individual(3, 2);
	auto output1 = ind.calculate(std::vector<double>({0.1, 0.2, 0.3}));
	for (auto x : output1) {
		std::cout << x << ",";
	}
	std::cout << std::endl;

	ind.add_node(0, 4, new enn::edge(0.1), new enn::edge(0.1));
	ind.add_node(1, 5, new enn::edge(0.1), new enn::edge(0.1));

	auto output2 = ind.calculate(std::vector<double>({0.1, 0.2, 0.3}));
	for (auto x : output2) {
		std::cout << x << ",";
	}
	std::cout << std::endl;

	ind.add_node(6, 7, new enn::edge(0.1), new enn::edge(0.1));

	auto output3 = ind.calculate(std::vector<double>({0.1, 0.2, 0.3}));
	for (auto x : output3) {
		std::cout << x << ",";
	}
	std::cout << std::endl;
}
