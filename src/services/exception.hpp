#pragma once

#include <stdexcept>
#include <string>
#include <userver/server/handlers/exceptions.hpp>

namespace services {

enum class ErrorType {
  kNotFound,
  kUnauthorized,
  kInvalidInput,
  kConflict,
  kPermissionDenied,
  kInternalError,
};

class ServiceLevelException : public std::runtime_error {
 public:
  explicit ServiceLevelException(const std::string& text,
                                 ErrorType error_type = ErrorType::kNotFound);
  [[nodiscard]] inline ErrorType GetErrorType() const { return error_type; }
  
  [[nodiscard]] inline int GetStatusCode() const {
    switch (error_type) {
      case ErrorType::kInvalidInput:
        return 400;
      case ErrorType::kUnauthorized:
        return 401;
      case ErrorType::kPermissionDenied:
        return 403;
      case ErrorType::kNotFound:
        return 404;
      case ErrorType::kConflict:
        return 409;
      case ErrorType::kInternalError:
        return 500;
    }
    throw std::runtime_error("Unknown status code");
  }

 private:
  ErrorType error_type;
};

}  // namespace services
