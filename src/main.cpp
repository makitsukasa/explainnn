#include <iostream>
#include "wann/ind.h"

int main()
{
	enn::Ind ind = enn::Ind(0.5);
	std::cout << ind.getFitness() << std::endl;
}
