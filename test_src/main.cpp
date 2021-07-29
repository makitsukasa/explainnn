#include "../incl/lib/randomlib.h"
#include "../incl/wann/individual.h"
#include <gtest/gtest.h>

TEST(Individual, SameSeed) {
	enn::random_engine = std::mt19937(6700417);
	auto ind1          = enn::individual(3, 2);
	auto out1          = ind1.calculate(std::vector<double>({0.1, 0.2, 0.3}));

	enn::random_engine = std::mt19937(6700417);
	auto ind2          = enn::individual(3, 2);
	auto out2          = ind2.calculate(std::vector<double>({0.1, 0.2, 0.3}));

	ASSERT_EQ(out1.size(), out2.size()) << "Vectors are of unequal length";
	for (unsigned long i = 0; i < out1.size(); i++) {
		EXPECT_FLOAT_EQ(out1[i], out2[i]) << "Vectors differ at index " << i;
	}
}
