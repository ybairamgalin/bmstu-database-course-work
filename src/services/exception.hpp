#pragma once

#include <stdexcept>

namespace services {

class ServiceLevelException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

}
