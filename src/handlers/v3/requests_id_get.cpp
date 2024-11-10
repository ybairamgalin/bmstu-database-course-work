#include "requests_id_get.hpp"

#include <http/query.hpp>
#include "gen/handlers/request_get.hpp"
#include "utils/uuid.hpp"

namespace {

handlers::v3::RequestsIdGet::Response MapResponse(services::Request&& req) {
  std::vector<gen::RequestComment> comments;
  comments.reserve(req.comments.size());
  for (auto& comment : req.comments) {
    comments.emplace_back(gen::RequestComment{
        comment.content,
        gen::UserFullInfo{comment.author.id, comment.author.name,
                          comment.author.login, comment.author.phone},
        comment.created_at,
    });
  }

  std::vector<gen::RequestGetResponse200::AttachmentsA> attachments;
  attachments.reserve(req.attachments.size());
  for (auto& attachment : req.attachments) {
    attachments.emplace_back(gen::RequestGetResponse200::AttachmentsA{
        attachment.download_url, attachment.filename});
  }

  return handlers::v3::RequestsIdGet::Response{
      gen::RequestGetResponse200{
          req.event_id,
          std::move(attachments),
          gen::UserFullInfo{
              req.author.id,
              req.author.name,
              req.author.login,
              req.author.phone,
          },
          req.description,
          req.status,
          std::move(comments),
          req.created_at,
          req.updated_at,
      },
      200,
      {},
  };
}

}  // namespace

namespace handlers::v3 {

RequestsIdGet::RequestsIdGet(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestsIdGet::Response RequestsIdGet::HandleJson(
    RequestsIdGet::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");
  auto request_id = utils::ToUuid(request.path_args.front());
  if (!request_id) {
    throw http::HttpException(400, "Invalid request id");
  }

  auto request_full = services_->MakeRequestManagementService()->GetRequestById(
      request_id.value(), auth);

  return MapResponse(std::move(request_full));
}

}  // namespace handlers::v3
