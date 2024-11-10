#include "request_object_mother.hpp"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>

#include "repository/requests_repository.hpp"

namespace repository {

repository::RequestFull RequestFullObjectMother::GetNewRequest() {
  const auto now = userver::storages::postgres::Now();

  repository::RequestFull request{};
  request.request_id = boost::uuids::random_generator()();
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.status = "new";
  request.description = "description";
  request.created_at = now;
  request.updated_at = now;

  return request;
}

}  // namespace repository
