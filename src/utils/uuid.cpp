#include "uuid.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace utils {

std::optional<boost::uuids::uuid> ToUuid(const std::string& value) {
  try {
    return boost::lexical_cast<boost::uuids::uuid>(value);
  } catch (const boost::bad_lexical_cast& bad_lexical_cast) {
    return std::nullopt;
  }
}

}  // namespace utils
