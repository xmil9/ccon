//
// Quick config
// Registry storage for configuration settings.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "registry_storage.h"
#include "essentutils/string_util.h"
#include "win32_util/registry.h"
#include <optional>
#include <vector>

using namespace std;
using namespace sutil;
using namespace win32;


namespace qcfg
{
///////////////////

RegistryStorage::RegistryStorage(HKEY parent, const std::wstring& keyPath)
   : m_parent{parent}, m_keyPath{keyPath}
{
}


bool RegistryStorage::load(Settings& settings)
{
   if (m_parent == NULL)
      return false;

   RegKey settingsKey;
   if (!settingsKey.open(m_parent, m_keyPath))
      return false;

   const vector<wstring> entries = settingsKey.entryNames();
   for (const wstring entryName : entries)
   {
      const optional<string> settingVal = settingsKey.readString(entryName);
      if (settingVal.has_value())
         settings[utf8(entryName)] = settingVal.value();
   }

   return true;
}


bool RegistryStorage::save(const Settings& settings)
{
   if (m_parent == NULL)
      return false;

   RegKey::removeKey(m_parent, m_keyPath);
   RegKey settingsKey(m_parent, m_keyPath);
   if (!settingsKey)
      return false;

   for (const auto setting : settings)
      settingsKey.writeString(utf16(setting.first), setting.second);

   return true;
}

} // namespace qcfg

#endif //_WIN32
