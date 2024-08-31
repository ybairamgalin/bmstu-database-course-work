#pragma once

namespace utils {

template <typename T>
struct MergeResultNoChange {
  std::vector<T> added{};
  std::vector<T> deleted{};
};

template <typename T>
MergeResultNoChange<T> Merge(const std::set<T>& before,
                             const std::set<T>& after) {
  MergeResultNoChange<T> result{};
  for (const auto& element : before) {
    if (!after.count(element)) {
      result.deleted.emplace_back(element);
    }
  }
  for (const auto& element : after) {
    if (!before.count(element)) {
      result.added.emplace_back(element);
    }
  }

  return result;
}

}  // namespace utils
