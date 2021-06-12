#pragma once

#include "node.h"

namespace enn {
class Ind {
private:
	double fitness;

public:
	Ind(double fitness);
	double getFitness();
};
} // namespace enn
