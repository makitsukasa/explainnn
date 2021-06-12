#include "wann/ind.h"

enn::Ind::Ind(double fitness_) {
	fitness = fitness_;
}

double enn::Ind::getFitness() {
	return fitness;
}
