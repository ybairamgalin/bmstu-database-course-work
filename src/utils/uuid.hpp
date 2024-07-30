#pragma once

#include <optional>

#include <boost/uuid/uuid.hpp>

namespace utils {

std::optional<boost::uuids::uuid> ToUuid(const std::string& value);

}
