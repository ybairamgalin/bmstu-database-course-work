#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "../file_meta_repository.hpp"

namespace repository {

class DbMetaRepository : public FileMetaRepository {
 public:
  explicit DbMetaRepository(
      userver::storages::postgres::ClusterPtr cluster_ptr);

  void UpsertFileMeta(const repository::FileMeta&) override;
  std::optional<FileMeta> GetFileMetaByHash(const std::string& hash) override;
  std::vector<FileMeta> GetFileMetaByRequestId(int64_t request_id) override;
  std::optional<FileMeta> GetFileMetaByFileUuid(
      const boost::uuids::uuid& uuid) override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
