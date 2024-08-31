#pragma once

#include <stdexcept>

namespace repository {

class RepositoryLevelException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

}
