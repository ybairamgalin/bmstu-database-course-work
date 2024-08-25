#pragma once

#include <optional>
#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>

#include "repository/exception.h"

namespace repository {

class FileStorageException : public RepositoryLevelException {
  using RepositoryLevelException::RepositoryLevelException;
};

class FileStorageRepository {
 public:
  virtual ~FileStorageRepository() = default;
  virtual void UploadFile(std::string&& file_content,
                          boost::uuids::uuid uuid) = 0;
  virtual std::optional<std::string> GetFile(boost::uuids::uuid uuid) = 0;
};

}  // namespace repository
