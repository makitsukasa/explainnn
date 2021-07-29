#pragma once

#include <iostream>
#include <random>

namespace enn {
extern std::mt19937 random_engine;
extern std::uniform_real_distribution<double> random_distribution;
} // namespace enn
