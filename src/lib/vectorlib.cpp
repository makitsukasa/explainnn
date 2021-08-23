#include "lib/vectorlib.h"

template <typename T> std::vector<T> concat(
		std::vector<T> a, std::vector<T> b, std::vector<T> c){
	auto ret = std::vector(a);
	ret.insert(ret.end(), b.begin(), b.end());
	ret.insert(ret.end(), c.begin(), c.end());
	return ret;
}

std::vector<unsigned long> concat(
		std::vector<unsigned long> a, std::vector<unsigned long> b, std::vector<unsigned long> c);
