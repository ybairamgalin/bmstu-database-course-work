#pragma once

#include <optional>
#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>

namespace repository {

struct FileMeta {
  boost::uuids::uuid uuid;
  std::string hash;
};

class FileMetaRepository {
 public:
  virtual ~FileMetaRepository() = default;
  virtual void UpsertFileMeta(const FileMeta&) = 0;
  virtual std::vector<FileMeta> GetFileMetaByRequestId(int64_t request_id) = 0;
  virtual std::optional<FileMeta> GetFileMetaByHash(
      const std::string& hash) = 0;
  virtual std::optional<FileMeta> GetFileMetaByFileUuid(
      const boost::uuids::uuid& uuid) = 0;
};

}  // namespace repository
