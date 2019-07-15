//
// Quick config
// File storage for configuration settings.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "file_storage.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <fstream>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>


namespace
{
///////////////////

using Setting = std::pair<std::string, std::string>;

const std::string KvSeparator = "=";
const std::string CommentPrefix = "#";


void saveSetting(const Setting& setting, std::ofstream& file)
{
   file << setting.first;
   file << KvSeparator;
   file << setting.second;
   file << '\n';
}


std::optional<Setting> parseSetting(const std::string& line)
{
   std::string trimmedLine = sutil::trimLeft(line, ' ');
   if (sutil::startsWith(trimmedLine, CommentPrefix))
      return {};

   const auto pos = line.find(KvSeparator);
   if (pos != std::string::npos)
      return make_pair(line.substr(0, pos), line.substr(pos + KvSeparator.size()));
   return {};
}

} // namespace


namespace qcfg
{
///////////////////

FileStorage::FileStorage(const std::filesystem::path& path) : m_filePath{path}
{
}


bool FileStorage::load(Settings& settings)
{
   if (m_filePath.empty())
      return false;

   std::ifstream file(m_filePath.c_str());
   if (!file.is_open())
      return false;

   std::string line;
   while (std::getline(file, line))
   {
      const auto setting = parseSetting(line);
      if (setting.has_value())
         settings[setting.value().first] = setting.value().second;
   }

   return true;
}


bool FileStorage::save(const Settings& settings)
{
   if (m_filePath.empty())
      return false;

   std::ofstream file(m_filePath.c_str());
   if (!file.is_open())
      return false;

   for (const auto setting : settings)
      saveSetting(setting, file);

   return true;
}

} // namespace qcfg
