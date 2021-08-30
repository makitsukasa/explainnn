#pragma once

#include <unordered_map>

namespace enn {
class maplib {
public:
	template <typename K, typename V>
	static std::unordered_map<V, K> inverse(std::unordered_map<K, V> &map);
};
} // namespace enn
