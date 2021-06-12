#include "wann/ind.h"
#include <iostream>

int main() {
	auto ind = enn::Ind(0.5);
	std::cout << ind.getFitness() << std::endl;
}
