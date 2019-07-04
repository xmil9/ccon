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

using namespace std;
using namespace sutil;


namespace
{
///////////////////

using Setting = pair<string, string>;

const string KvSeparator = "=";
const string CommentPrefix = "#";


void saveSetting(const Setting& setting, ofstream& file)
{
   file << setting.first;
   file << KvSeparator;
   file << setting.second;
   file << '\n';
}


optional<Setting> parseSetting(const std::string& line)
{
   string trimmedLine = trimLeft(line, ' ');
   if (startsWith(trimmedLine, CommentPrefix))
      return {};

   const auto pos = line.find(KvSeparator);
   if (pos != string::npos)
      return make_pair(line.substr(0, pos), line.substr(pos + KvSeparator.size()));
   return {};
}

} // namespace


namespace qcfg
{
///////////////////

FileStorage::FileStorage(const std::filesystem::path& path)
   : m_filePath{path}
{
}


bool FileStorage::load(Settings& settings)
{
   if (m_filePath.empty())
      return false;

   ifstream file(m_filePath.c_str());
   if (!file.is_open())
      return false;

   string line;
   while (getline(file, line))
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

   ofstream file(m_filePath.c_str());
   if (!file.is_open())
      return false;

   for (const auto setting : settings)
      saveSetting(setting, file);

   return true;
}

} // namespace qcfg
