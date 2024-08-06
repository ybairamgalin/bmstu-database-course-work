#include "exception.hpp"

namespace services {

ServiceLevelException::ServiceLevelException(const std::string& text,
                                             ErrorType error_type)
    : std::runtime_error(text), error_type(error_type) {}

ErrorType ServiceLevelException::GetErrorType() const { return error_type; }

}  // namespace services