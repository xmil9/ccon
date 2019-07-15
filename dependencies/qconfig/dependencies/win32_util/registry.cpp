//
// Win32 utilities library
// Registry utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "registry.h"
#include "essentutils/string_util.h"
#include <cassert>


namespace
{
///////////////////

// Reads value of integer type from given registry entry.
template <typename Int>
std::optional<Int> readInt(HKEY key, const std::wstring& entryName)
{
   static_assert(std::is_integral_v<Int> && (sizeof(Int) == 4 || sizeof(Int) == 8));

   if (!key)
      return {};

   Int value = 0;
   DWORD numBytes = sizeof(value);
   DWORD entryType = REG_NONE;
   const LSTATUS res = RegQueryValueExW(key, entryName.c_str(), nullptr, &entryType,
                                        reinterpret_cast<BYTE*>(&value), &numBytes);
   constexpr DWORD expectedEntryType = (sizeof(Int) == 4) ? REG_DWORD : REG_QWORD;
   if (res != ERROR_SUCCESS || entryType != expectedEntryType)
      return {};

   assert(numBytes == sizeof(value));
   return value;
}


// Writes value of integer type to given registry entry.
template <typename Int> bool writeInt(HKEY key, const std::wstring& entryName, Int val)
{
   static_assert(std::is_integral_v<Int> && (sizeof(Int) == 4 || sizeof(Int) == 8));

   if (!key)
      return false;

   constexpr DWORD entryType = (sizeof(Int) == 4) ? REG_DWORD : REG_QWORD;
   const LSTATUS res = RegSetValueExW(key, entryName.c_str(), 0, entryType,
                                      reinterpret_cast<const BYTE*>(&val), sizeof(val));
   return (res == ERROR_SUCCESS);
}

} // namespace


namespace win32
{
///////////////////

RegKey::RegKey(HKEY parent, const std::wstring& keyPath, REGSAM accessRights)
{
   create(parent, keyPath, accessRights);
}


RegKey::~RegKey()
{
   close();
}


RegKey::RegKey(RegKey&& other) noexcept
{
   swap(*this, other);
}


RegKey& RegKey::operator=(RegKey&& other) noexcept
{
   close();

   m_key = other.m_key;
   m_created = other.m_created;
   // Make sure dtor of moved-from object does nothing.
   other.m_key = NULL;
   other.m_created = false;
   return *this;
}


void swap(RegKey& a, RegKey& b) noexcept
{
   std::swap(a.m_key, b.m_key);
   std::swap(a.m_created, b.m_created);
}


bool RegKey::create(HKEY parent, const std::wstring& keyPath, REGSAM accessRights)
{
   close();

   DWORD disposition = 0;
   const LSTATUS res = RegCreateKeyExW(parent, keyPath.c_str(), 0, nullptr, 0,
                                       accessRights, nullptr, &m_key, &disposition);
   if (res != ERROR_SUCCESS)
   {
      m_key = NULL;
      return false;
   }

   m_created = (disposition == REG_CREATED_NEW_KEY);
   return true;
}


bool RegKey::open(HKEY parent, const std::wstring& keyPath, REGSAM accessRights)
{
   close();

   const LSTATUS res = RegOpenKeyExW(parent, keyPath.c_str(), 0, accessRights, &m_key);
   if (res != ERROR_SUCCESS)
   {
      m_key = NULL;
      return false;
   }
   return true;
}


void RegKey::close()
{
   if (m_key)
   {
      RegCloseKey(m_key);
      m_key = NULL;
   }
}


void RegKey::clear()
{
   m_key = NULL;
   m_created = false;
}


bool RegKey::keyExists(HKEY parent, const std::wstring& keyPath)
{
   RegKey key;
   return key.open(parent, keyPath, KEY_READ);
}


bool RegKey::removeKey(HKEY parent, const std::wstring& keyPath)
{
   const LSTATUS res = RegDeleteTreeW(parent, keyPath.c_str());
   return (res == ERROR_SUCCESS);
}


std::optional<int32_t> RegKey::readInt32(const std::wstring& entryName) const
{
   return readInt<int32_t>(m_key, entryName);
}


std::optional<int64_t> RegKey::readInt64(const std::wstring& entryName) const
{
   return readInt<int64_t>(m_key, entryName);
}


std::optional<std::string> RegKey::readString(const std::wstring& entryName) const
{
   if (!m_key)
      return {};

   const std::string entryNameAnsi = sutil::utf8(entryName);

   DWORD numBytes = 0;
   DWORD entryType = REG_NONE;
   LSTATUS res = RegQueryValueExA(m_key, entryNameAnsi.c_str(), nullptr, &entryType,
                                  nullptr, &numBytes);
   if (res != ERROR_SUCCESS || entryType != REG_SZ)
      return {};

   constexpr size_t charBytes = sizeof(char);
   assert(numBytes % charBytes == 0);
   const std::size_t strLen = numBytes / charBytes;

   // '+ 1' for additional zero terminator to make sure the string is well formed.
   std::vector<char> buffer(strLen + 1, 0);
   DWORD numReadBytes = static_cast<DWORD>(buffer.size());
   res = RegQueryValueExA(m_key, entryNameAnsi.c_str(), nullptr, nullptr,
                          reinterpret_cast<BYTE*>(buffer.data()), &numReadBytes);
   if (res != ERROR_SUCCESS || numReadBytes != numBytes)
      return {};

   assert(numReadBytes == numBytes);
   *buffer.rbegin() = 0;
   return buffer.data();
}

std::optional<std::wstring> RegKey::readWString(const std::wstring& entryName) const
{
   if (!m_key)
      return {};

   DWORD numBytes = 0;
   DWORD entryType = REG_NONE;
   LSTATUS res =
      RegQueryValueExW(m_key, entryName.c_str(), nullptr, &entryType, nullptr, &numBytes);
   if (res != ERROR_SUCCESS || entryType != REG_SZ)
      return {};

   constexpr size_t charBytes = sizeof(wchar_t);
   assert(numBytes % charBytes == 0);
   const std::size_t strLen = numBytes / charBytes;

   // '+ 1' for additional zero terminator to make sure the string is well formed.
   std::vector<wchar_t> buffer(strLen + 1);
   DWORD numReadBytes = static_cast<DWORD>(buffer.size()) * sizeof(wchar_t);
   res = RegQueryValueExW(m_key, entryName.c_str(), nullptr, nullptr,
                          reinterpret_cast<BYTE*>(buffer.data()), &numReadBytes);
   if (res != ERROR_SUCCESS || numReadBytes != numBytes)
      return {};

   assert(numReadBytes == numBytes);
   *buffer.rbegin() = 0;
   return buffer.data();
}


std::size_t RegKey::readBinary(const std::wstring& entryName,
                               std::function<BYTE*(std::size_t)> getBuffer) const
{
   if (!m_key)
      return 0;

   DWORD numBytes = 0;
   DWORD entryType = REG_NONE;
   LSTATUS res =
      RegQueryValueExW(m_key, entryName.c_str(), nullptr, &entryType, nullptr, &numBytes);
   if (res != ERROR_SUCCESS || entryType != REG_BINARY)
      return 0;

   BYTE* buffer = getBuffer(numBytes);
   if (!buffer)
      return 0;
   DWORD numReadBytes = numBytes;
   res =
      RegQueryValueExW(m_key, entryName.c_str(), nullptr, nullptr, buffer, &numReadBytes);
   if (res != ERROR_SUCCESS || numReadBytes != numBytes)
      return 0;

   return numReadBytes;
}


bool RegKey::writeInt32(const std::wstring& entryName, int32_t val) const
{
   return writeInt(m_key, entryName, val);
}


bool RegKey::writeInt64(const std::wstring& entryName, int64_t val) const
{
   return writeInt(m_key, entryName, val);
}


bool RegKey::writeString(const std::wstring& entryName, const std::string& val) const
{
   if (!m_key)
      return {};

   const std::size_t numBytes = (val.size() + 1) * sizeof(char);
   const LSTATUS res = RegSetValueExA(m_key, sutil::utf8(entryName).c_str(), 0, REG_SZ,
                                      reinterpret_cast<const BYTE*>(val.c_str()),
                                      static_cast<DWORD>(numBytes));
   return (res == ERROR_SUCCESS);
}


bool RegKey::writeWString(const std::wstring& entryName, const std::wstring& val) const
{
   if (!m_key)
      return {};

   const std::size_t numBytes = (val.size() + 1) * sizeof(wchar_t);
   const LSTATUS res = RegSetValueExW(m_key, entryName.c_str(), 0, REG_SZ,
                                      reinterpret_cast<const BYTE*>(val.c_str()),
                                      static_cast<DWORD>(numBytes));
   return (res == ERROR_SUCCESS);
}


bool RegKey::writeBinary(const std::wstring& entryName, const BYTE* data,
                         std::size_t numBytes) const
{
   if (!m_key)
      return {};

   const LSTATUS res = RegSetValueExW(m_key, entryName.c_str(), 0, REG_BINARY, data,
                                      static_cast<DWORD>(numBytes));
   return (res == ERROR_SUCCESS);
}


bool RegKey::removeEntry(const std::wstring& entryName) const
{
   if (!m_key)
      return {};

   const LSTATUS res = RegDeleteValueW(m_key, entryName.c_str());
   return (res == ERROR_SUCCESS);
}


std::size_t RegKey::countSubkeys() const
{
   if (!m_key)
      return 0;

   DWORD numSubkeys = 0;
   const LSTATUS res =
      RegQueryInfoKeyW(m_key, nullptr, nullptr, nullptr, &numSubkeys, nullptr, nullptr,
                       nullptr, nullptr, nullptr, nullptr, nullptr);
   if (res != ERROR_SUCCESS)
      return 0;
   return numSubkeys;
}


std::vector<std::wstring> RegKey::subkeyNames() const
{
   if (!m_key)
      return {};

   DWORD maxSubkeyNameLen = 0;
   LSTATUS res =
      RegQueryInfoKeyW(m_key, nullptr, nullptr, nullptr, nullptr, &maxSubkeyNameLen,
                       nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
   if (res != ERROR_SUCCESS)
      return {};

   maxSubkeyNameLen += 1;
   std::vector<wchar_t> buffer(maxSubkeyNameLen, 0);

   std::vector<std::wstring> subkeys;
   DWORD idx = 0;
   DWORD nameLen = maxSubkeyNameLen;

   while (res == ERROR_SUCCESS)
   {
      nameLen = maxSubkeyNameLen;
      res = RegEnumKeyExW(m_key, idx++, buffer.data(), &nameLen, nullptr, nullptr,
                          nullptr, nullptr);
      if (res == ERROR_SUCCESS)
         subkeys.push_back(buffer.data());
   }

   if (res == ERROR_SUCCESS || res == ERROR_NO_MORE_ITEMS)
      return subkeys;
   return {};
}


std::size_t RegKey::countEntries() const
{
   if (!m_key)
      return 0;

   DWORD numEntries = 0;
   const LSTATUS res =
      RegQueryInfoKeyW(m_key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                       &numEntries, nullptr, nullptr, nullptr, nullptr);
   if (res != ERROR_SUCCESS)
      return 0;
   return numEntries;
}


std::vector<std::wstring> RegKey::entryNames() const
{
   if (!m_key)
      return {};

   DWORD maxEntryNameLen = 0;
   LSTATUS res =
      RegQueryInfoKeyW(m_key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                       nullptr, &maxEntryNameLen, nullptr, nullptr, nullptr);
   if (res != ERROR_SUCCESS)
      return {};

   maxEntryNameLen += 1;
   std::vector<wchar_t> buffer(maxEntryNameLen, 0);

   std::vector<std::wstring> entries;
   DWORD idx = 0;
   DWORD nameLen = maxEntryNameLen;

   while (res == ERROR_SUCCESS)
   {
      nameLen = maxEntryNameLen;
      res = RegEnumValueW(m_key, idx++, buffer.data(), &nameLen, nullptr, nullptr,
                          nullptr, nullptr);
      if (res == ERROR_SUCCESS)
         entries.push_back(buffer.data());
   }

   if (res == ERROR_SUCCESS || res == ERROR_NO_MORE_ITEMS)
      return entries;
   return {};
}

} // namespace win32

#endif //_WIN32
