#include "yandex_auth.hpp"

#include <set>

#include "../../repository/user_data_repository.hpp"
#include "../mapping/mapping.hpp"

namespace services {

YandexAuthService::YandexAuthService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : db_repository_(repository_factory->MakeUserDataDbRepository()),
      http_repository_(repository_factory->MakeUserDataHttpRepository()) {}

std::optional<services::AuthData> YandexAuthService::GetAuthDataByToken(
    const std::string& token) {
  auto db_user_opt = db_repository_->GetUserData(token);
  if (db_user_opt) {
    return internal::MapAuthData(db_user_opt.value());
  }
  auto yandex_user_opt = http_repository_->GetUserData(token);
  if (!yandex_user_opt) {
    return std::nullopt;
  }
  db_repository_->SaveUserData(yandex_user_opt.value());
  return internal::MapAuthData(yandex_user_opt.value());
}

}  // namespace services
