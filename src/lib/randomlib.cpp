#include "lib/randomlib.h"

std::mt19937 enn::random_engine(std::random_device{}());
std::uniform_real_distribution<double> enn::random_distribution(0.0, 1.0);
