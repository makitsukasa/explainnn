#pragma once

#include <unordered_map>
#include <vector>

namespace enn {
class inverse {
public:
	template <typename K, typename V>
	static std::unordered_map<V, K> inverse_map(std::unordered_map<K, V> &map);

	template <typename V> static std::vector<V> inverse_vector(std::vector<V> &vec);
};
} // namespace enn
