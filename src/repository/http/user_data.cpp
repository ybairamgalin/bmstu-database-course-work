#include "user_data.hpp"

#include <fmt/format.h>

#include <userver/formats/json/serialize.hpp>

namespace {

std::optional<repository::AuthData> MapToResponse(
    const std::string& yandex_response, const std::string& token) {
  repository::AuthData response;
  try {
    auto json = userver::formats::json::FromString(yandex_response);
    response.user_id = std::stoi(json["id"].As<std::string>());
    response.login = json["login"].As<std::string>();
    response.name = json["real_name"].As<std::string>();
    response.phone = json["default_phone"]["number"].As<std::string>();
    response.token = token;
  } catch (const std::exception& e) {
    LOG_ERROR() << e.what();
    return std::nullopt;
  }
  return response;
}

}  // namespace

namespace repository {

YandexLoginDataProvider::YandexLoginDataProvider(
    userver::clients::http::Client& http_client)
    : http_client_(http_client) {}

std::optional<AuthData> YandexLoginDataProvider::GetUserData(
    const std::string& token) {
  const auto response =
      http_client_.CreateRequest()
          .post(
              fmt::format("https://login.yandex.ru/info?oauth_token={}", token))
          .timeout(5000)
          .perform();
  if (response->status_code() != 200) {
    return std::nullopt;
  }
  LOG_INFO() << response->body();
  return MapToResponse(response->body(), token);
}

std::optional<AuthData> YandexLoginDataProvider::GetUserDataByLogin(
    const std::string&) {
  return std::nullopt;
}

void YandexLoginDataProvider::SaveUserData(const AuthData&) {}

}  // namespace repository