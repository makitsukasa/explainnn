#pragma once

#include <vector>

namespace enn {
class vectorlib {
public:
	template <typename T> static std::vector<T> concat(
		std::vector<T> a, std::vector<T> b, std::vector<T> c);

	template <typename V> static std::vector<V> inverse(std::vector<V> &vec);
};
} // namespace enn
