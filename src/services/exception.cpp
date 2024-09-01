#include "exception.hpp"

namespace services {

ServiceLevelException::ServiceLevelException(const std::string& text,
                                             ErrorType error_type)
    : std::runtime_error(text), error_type(error_type) {}

ErrorType ServiceLevelException::GetErrorType() const { return error_type; }

int ServiceLevelException::GetStatusCode() const {
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

}  // namespace services
