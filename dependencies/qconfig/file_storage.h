//
// Quick config
// File storage for configuration settings.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "storage.h"
#include "qconfig_api.h"
#include <filesystem>


namespace qcfg
{
///////////////////

// File storage for config settings.
// Supports full line comments with '#'.
class QCONFIG_API FileStorage : public Storage
{
 public:
   FileStorage() = default;
   FileStorage(const std::filesystem::path& path);
   ~FileStorage() = default;
   FileStorage(const FileStorage&) = default;
   FileStorage(FileStorage&&) = default;
   FileStorage& operator=(const FileStorage&) = default;
   FileStorage& operator=(FileStorage&&) = default;

   bool load(Settings& settings) override;
   bool save(const Settings& settings) override;

 private:
   std::filesystem::path m_filePath;
};

} // namespace qcfg
