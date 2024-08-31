#pragma once

#include <stdexcept>

#include "gen/common.hpp"

namespace http {

class HttpException : public std::runtime_error {
 public:
  explicit HttpException(int code, const std::string& message)
      : std::runtime_error(message), error_response{code, message} {};

  [[nodiscard]] const gen::ErrorResponse& Get() const { return error_response; }

 private:
  gen::ErrorResponse error_response;
};

}  // namespace http
