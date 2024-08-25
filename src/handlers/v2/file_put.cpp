#include "file_put.hpp"

#include "gen/handlers/file_put.hpp"

#include "utils/json.hpp"
#include "utils/request.hpp"

namespace handlers::v2 {

FilePut::FilePut(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context)
    : BaseHandler(config, context) {}

std::string FilePut::Handle(const userver::server::http::HttpRequest& request,
                            userver::server::request::RequestContext&) const {
  auto file_uuid = services_->MakeFileService()->UploadFile(
      services::File{request.RequestBody()});

  return utils::ToJsonString(gen::FilePutResponse200{file_uuid});
}

}  // namespace handlers::v2
