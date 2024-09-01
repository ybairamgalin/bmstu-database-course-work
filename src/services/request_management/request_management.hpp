#pragma once

#include <memory>

#include "../../repository/factory.hpp"
#include "../request_management_service.hpp"

namespace services {

class RequestManagementService : public IRequestManagementService {
 public:
  explicit RequestManagementService(
      const std::shared_ptr<repository::IRepositoryFactory>&
          repository_factory);
  Request GetRequestById(const boost::uuids::uuid& request_id) override;
  boost::uuids::uuid AddRequest(
      const services::RequestToCreateOrUpdate& request) override;
  void UpdateRequest(const boost::uuids::uuid& request_id,
                     const services::RequestToCreateOrUpdate& request) override;
  void AddComment(const boost::uuids::uuid& request_id,
                  const std::string& content, int64_t author_id) override;
  std::vector<RequestShort> GetAll() override;

 private:
  std::unique_ptr<repository::RequestsRepository> request_repository_;
  std::unique_ptr<repository::EventRepository> event_repository_;
  std::unique_ptr<repository::UserDataRepository> user_data_repository_;
};

}  // namespace services
