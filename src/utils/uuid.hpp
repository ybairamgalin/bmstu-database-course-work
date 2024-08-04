#pragma once

#include <optional>
#include <string>

#include <boost/uuid/uuid.hpp>

namespace utils {

std::optional<boost::uuids::uuid> ToUuid(const std::string& value);

}
