#pragma once

#include <boost/uuid/uuid.hpp>

#include <string>

namespace services {

struct File {
  std::string content;
};

struct FileToDownload {
  std::string download_url;
  std::string filename;
};

class IFileService {
 public:
  virtual ~IFileService() = default;

  /*!
   @return uuid сохраненного или найденного файла
   */
  virtual boost::uuids::uuid UploadFile(File&& file) = 0;
  virtual std::optional<FileToDownload> GetFile(
      const boost::uuids::uuid& file_uuid) = 0;
};

}  // namespace services
