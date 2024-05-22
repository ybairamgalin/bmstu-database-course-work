#include "file_meta.hpp"

namespace repository {

DbMetaRepository::DbMetaRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

void DbMetaRepository::UpsertFileMeta(const FileMeta&) {}

std::optional<FileMeta> DbMetaRepository::GetFileMetaByHash(
    const std::string& hash) {
  return std::optional<FileMeta>();
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
