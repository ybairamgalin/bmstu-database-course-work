#pragma once

#include <memory>

#include "../../repository/factory.hpp"
#include "../../repository/file_meta_repository.hpp"
#include "../../repository/file_storage_repository.hpp"
#include "../file_service.hpp"

namespace services {

class FileService : public IFileService {
 public:
  explicit FileService(const std::shared_ptr<repository::IRepositoryFactory>&
                           repository_factory);

  boost::uuids::uuid UploadFile(File&& file) override;
  std::optional<File> GetFile(const boost::uuids::uuid& file_uuid) override;

 private:
  std::unique_ptr<repository::FileMetaRepository> file_meta_repository_;
  std::unique_ptr<repository::FileStorageRepository> file_storage_repository_;
};

}  // namespace services
