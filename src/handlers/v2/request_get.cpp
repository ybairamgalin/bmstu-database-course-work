#include "request_get.hpp"

#include <http/query.hpp>
#include "gen/handlers/request_get.hpp"

namespace {

handlers::v2::RequestGet::Response MapResponse(services::Request&& req) {
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

  return handlers::v2::RequestGet::Response{
      gen::RequestGetResponse200{
          req.event_id,
          {},
          gen::UserFullInfo{
              req.author.id,
              req.author.name,
              req.author.login,
              req.author.phone,
          },
          req.description,
          std::move(comments),
          req.created_at,
          req.updated_at,
      },
      200,
      {},
  };
}

}  // namespace

namespace handlers::v2 {

RequestGet::RequestGet(const userver::components::ComponentConfig& config,
                       const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestGet::Response RequestGet::HandleJson(
    RequestGet::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");
  LOG_ERROR() << "YAROSLAV" << auth.login;

  auto request_id = http::GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
      request.query_params, "request_id");

  auto request_full =
      services_->MakeRequestManagementService()->GetRequestById(request_id);

  return MapResponse(std::move(request_full));
}

}  // namespace handlers::v2
