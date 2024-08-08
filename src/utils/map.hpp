#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

namespace utils {

template <typename ElementType, typename GetKey>
auto AsMap(std::vector<ElementType>&& vector, GetKey get_key) {
  using KeyType = decltype(get_key(vector.front()));

  std::unordered_map<KeyType, ElementType> result;
  result.reserve(vector.size());
  for (auto&& element : vector) {
    result.try_emplace(get_key(element), std::move(element));
  }
  return result;
}

}  // namespace utils
