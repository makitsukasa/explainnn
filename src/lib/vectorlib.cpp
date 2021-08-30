#include "lib/vectorlib.h"

template <typename T> std::vector<T> enn::vectorlib::concat(
		std::vector<T> a, std::vector<T> b, std::vector<T> c){
	auto ret = std::vector(a);
	ret.insert(ret.end(), b.begin(), b.end());
	ret.insert(ret.end(), c.begin(), c.end());
	return ret;
}

template std::vector<unsigned long> enn::vectorlib::concat(
		std::vector<unsigned long> a, std::vector<unsigned long> b, std::vector<unsigned long> c);

template <typename V> std::vector<V> enn::vectorlib::inverse(std::vector<V> &vec) {
	std::vector<V> inv(vec.size());
	for (V i = 0; i < vec.size(); i++) {
		inv[vec[i]] = i;
	}
	return inv;
}

template std::vector<unsigned long> enn::vectorlib::inverse(std::vector<unsigned long> &vec);
