#pragma once

#include <stdexcept>
#include <string>
#include <userver/server/handlers/exceptions.hpp>

namespace services {

enum class ErrorType {
  kNotFound,
  kConflict,
  kInvalidInput,
};

class ServiceLevelException : public std::runtime_error {
 public:
  explicit ServiceLevelException(const std::string& text,
                                 ErrorType error_type = ErrorType::kNotFound);
  [[nodiscard]] ErrorType GetErrorType() const;

 private:
  ErrorType error_type;
};

}  // namespace services
