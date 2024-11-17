#pragma once

#include <gmock/gmock.h>
#include "repository/factory.hpp"
#include "repository/file_meta_repository.hpp"
#include "repository/file_storage_repository.hpp"

namespace repository {

class MockRequestsRepository : public RequestsRepository {
 public:
  MOCK_METHOD(std::optional<RequestFull>, GetRequestById,
              (const boost::uuids::uuid& uuid), (override));
  MOCK_METHOD(void, Insert, (const Request& request), (override));
  MOCK_METHOD(void, Update, (const Request& request), (override));
  MOCK_METHOD(void, AddComment,
              (const boost::uuids::uuid& id, const std::string& content,
               int64_t author_id),
              (override));
  MOCK_METHOD(std::vector<RequestShort>, GetAll, (), (override));
  MOCK_METHOD(std::vector<RequestShort>, GetFiltered,
              (const RequestFilters& filters), (override));
};

class MockEventRepository : public EventRepository {
 public:
  MOCK_METHOD(std::vector<Event>, GetEventsByIds,
              (const std::vector<boost::uuids::uuid>& event_id), (override));
  MOCK_METHOD(void, AddEvent, (const Event& event), (override));
  MOCK_METHOD(std::vector<Event>, GetAll, (), (override));
  MOCK_METHOD(void, DeleteEvent, (const boost::uuids::uuid&), (override));
};

class MockUserDataRepository : public UserDataRepository {
 public:
  MOCK_METHOD(std::optional<AuthData>, GetUserData, (const std::string& token),
              (override));
  MOCK_METHOD(std::optional<AuthData>, GetUserDataByLogin,
              (const std::string& login), (override));
  MOCK_METHOD(std::vector<AuthData>, GetUserDataByIds,
              (const std::vector<int64_t>& id), (override));
  MOCK_METHOD(void, SerUserRole,
              (const std::string& login, const std::string& role), (override));
  MOCK_METHOD(void, SaveUserData, (const AuthData&), (override));
  MOCK_METHOD(void, AddUserToPermissionGroups,
              (int64_t user_id,
               const std::vector<std::string>& permission_group));
};

class MockFileMetaRepository : public FileMetaRepository {
 public:
  MOCK_METHOD(void, UpsertFileMeta, (const FileMeta&), (override));
  MOCK_METHOD(std::vector<FileMeta>, GetFileMetaByRequestId,
              (int64_t request_id), (override));
  MOCK_METHOD(std::optional<FileMeta>, GetFileMetaByHash,
              (const std::string& hash), (override));
  MOCK_METHOD(std::optional<FileMeta>, GetFileMetaByFileUuid,
              (const boost::uuids::uuid& uuid), (override));
};

class MockFileStorageRepository : public FileStorageRepository {
 public:
  MOCK_METHOD(void, UploadFile,
              (std::string && file_content, boost::uuids::uuid uuid),
              (override));
  MOCK_METHOD(std::optional<std::string>, GetFile, (boost::uuids::uuid uuid),
              (override));
};

class MockRepositoryFactory : public IRepositoryFactory {
 public:
  MOCK_METHOD(std::unique_ptr<RequestsRepository>, MakeRequestsRepository, (),
              (override));
  MOCK_METHOD(std::unique_ptr<UserDataRepository>, MakeUserDataDbRepository, (),
              (override));
  MOCK_METHOD(std::unique_ptr<UserDataRepository>, MakeUserDataHttpRepository,
              (), (override));
  MOCK_METHOD(std::unique_ptr<FileMetaRepository>, MakeFileMetaRepository, (),
              (override));
  MOCK_METHOD(std::unique_ptr<EventRepository>, MakeEventsRepository, (),
              (override));
  MOCK_METHOD(std::unique_ptr<FileStorageRepository>, MakeFileStorageRepository,
              (), (override));
};

}  // namespace repository
