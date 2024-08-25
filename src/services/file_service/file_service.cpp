#include "file_service.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include <userver/crypto/hash.hpp>

#include "repository/exception.h"
#include "services/exception.hpp"

namespace services {

FileService::FileService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : file_meta_repository_(repository_factory->MakeFileMetaRepository()),
      file_storage_repository_(
          repository_factory->MakeFileStorageRepository()) {}

boost::uuids::uuid FileService::UploadFile(File&& file) {
  const auto hash = userver::crypto::hash::Sha256(file.content);
  auto file_meta_opt = file_meta_repository_->GetFileMetaByHash(hash);
  if (file_meta_opt) {
    return file_meta_opt.value().uuid;
  }

  const auto file_uuid = boost::uuids::random_generator()();

  try {
    file_storage_repository_->UploadFile(std::move(file.content), file_uuid);
    file_meta_repository_->UpsertFileMeta(
        repository::FileMeta{file_uuid, std::move(file.filename), hash});
  } catch (const repository::FileStorageException& ex) {
    LOG_ERROR() << ex.what();
    throw ServiceLevelException("Failed to upload file",
                                ErrorType::kInternalError);
  }
  return file_uuid;
}

std::optional<File> FileService::GetFile(const boost::uuids::uuid& file_uuid) {
  auto file_opt = file_storage_repository_->GetFile(file_uuid);
  if (!file_opt) {
    return std::nullopt;
  }
  auto file_meta_opt = file_meta_repository_->GetFileMetaByFileUuid(file_uuid);
  if (!file_meta_opt) {
    return std::nullopt;
  }
  return File{file_meta_opt.value().source_file_name, file_opt.value()};
}

}  // namespace services
