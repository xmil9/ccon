//
// Quick config
// Registry storage for configuration settings.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "storage.h"
#include "qconfig_api.h"
#include "win32_util/win32_windows.h"
#include <string>

namespace qcfg
{
///////////////////

class QCONFIG_API RegistryStorage : public Storage
{
 public:
   RegistryStorage() = default;
   RegistryStorage(HKEY parent, const std::wstring& keyPath);
   ~RegistryStorage() = default;
   RegistryStorage(const RegistryStorage&) = default;
   RegistryStorage(RegistryStorage&&) = default;
   RegistryStorage& operator=(const RegistryStorage&) = default;
   RegistryStorage& operator=(RegistryStorage&&) = default;

   bool load(Settings& settings) override;
   bool save(const Settings& settings) override;

 private:
   HKEY m_parent = NULL;
   std::wstring m_keyPath;
};

} // namespace qcfg

#endif //_WIN32
