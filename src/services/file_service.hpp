#pragma once

#include <boost/uuid/uuid.hpp>

#include <string>

namespace services {

struct File {
  std::string filename;
  std::string content;
};

class IFileService {
 public:
  /*!
   @return uuid сохраненного или найденного файла
   */
  virtual boost::uuids::uuid UploadFile(File&& file) = 0;
  virtual std::optional<File> GetFile(const boost::uuids::uuid& file_uuid) = 0;
};

}  // namespace services
