#include "lib/inverse.h"

template <typename K, typename V>
std::unordered_map<V, K> enn::inverse::inverse_map(std::unordered_map<K, V> &map) {
	std::unordered_map<V, K> inv;
	std::for_each(map.begin(), map.end(), [&inv](const std::pair<K, V> &p) {
		inv.insert(std::make_pair(p.second, p.first));
	});
	return inv;
}

template <typename V> std::vector<V> enn::inverse::inverse_vector(std::vector<V> &vec) {
	std::vector<V> inv(vec.size());
	for (V i = 0; i < vec.size(); i++) {
		inv[vec[i]] = i;
	}
	return inv;
}

template std::vector<unsigned long> enn::inverse::inverse_vector(std::vector<unsigned long> &vec);
