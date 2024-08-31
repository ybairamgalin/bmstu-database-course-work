#include "file_meta.hpp"

namespace repository {

DbMetaRepository::DbMetaRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

void DbMetaRepository::UpsertFileMeta(const FileMeta& file_meta) {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "insert into service.file_meta (uuid, hash) "
                        "values ($1, $2)",
                        file_meta.uuid, file_meta.hash);
}

std::optional<FileMeta> DbMetaRepository::GetFileMetaByHash(
    const std::string& hash) {
  auto result = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "select uuid, hash "
      "from service.file_meta "
      "where hash = $1",
      hash);
  if (result.IsEmpty()) {
    return std::nullopt;
  }
  return result.AsSingleRow<FileMeta>(userver::storages::postgres::kRowTag);
}

std::vector<FileMeta> DbMetaRepository::GetFileMetaByRequestId(
    int64_t request_id) {
  return std::vector<FileMeta>();
}

std::optional<FileMeta> DbMetaRepository::GetFileMetaByFileUuid(
    const boost::uuids::uuid& uuid) {
  return std::optional<FileMeta>();
}

}  // namespace repository
