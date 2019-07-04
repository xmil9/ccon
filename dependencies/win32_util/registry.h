//
// Win32 utilities library
// Registry utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "win32_util_api.h"
#include "win32_windows.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <variant>


namespace win32
{
///////////////////

// Represents a registry key.
// RAII for opening/closing key.
class WIN32UTIL_API RegKey
{
 public:
   RegKey() = default;
   RegKey(HKEY parent, const std::wstring& keyPath, REGSAM accessRights = KEY_ALL_ACCESS);
   ~RegKey();
   RegKey(const RegKey&) = delete;
   RegKey(RegKey&& other) noexcept;
   RegKey& operator=(const RegKey&) = delete;
   RegKey& operator=(RegKey&& other) noexcept;

   explicit operator bool() const;
   operator HKEY() const;
   HKEY hkey() const;
   friend WIN32UTIL_API void swap(RegKey& a, RegKey& b) noexcept;

   bool create(HKEY parent, const std::wstring& keyPath,
               REGSAM accessRights = KEY_ALL_ACCESS);
   bool open(HKEY parent, const std::wstring& keyPath,
             REGSAM accessRights = KEY_ALL_ACCESS);
   void close();
   // Clears the stored HKEY handle without closing the key, if it is open.
   void clear();
   bool wasCreated() const;
   bool wasOpened() const;
   static bool keyExists(HKEY parent, const std::wstring& keyPath);
   static bool removeKey(HKEY parent, const std::wstring& keyPath);

   std::optional<int32_t> readInt32(const std::wstring& entryName) const;
   std::optional<int64_t> readInt64(const std::wstring& entryName) const;
   std::optional<std::string> readString(const std::wstring& entryName) const;
   std::optional<std::wstring> readWString(const std::wstring& entryName) const;
   // Reads binary data from given entry. The function given as second argument has
   // to return a pointer to a buffer of given size in bytes or return null.
   std::size_t readBinary(const std::wstring& entryName,
                          std::function<BYTE*(std::size_t)> getBuffer) const;
   bool writeInt32(const std::wstring& entryName, int32_t val) const;
   bool writeInt64(const std::wstring& entryName, int64_t val) const;
   bool writeString(const std::wstring& entryName, const std::string& val) const;
   bool writeWString(const std::wstring& entryName, const std::wstring& val) const;
   bool writeBinary(const std::wstring& entryName, const BYTE* data,
                    std::size_t numBytes) const;

   bool removeEntry(const std::wstring& entryName) const;
   std::size_t countSubkeys() const;
   std::vector<std::wstring> subkeyNames() const;
   std::size_t countEntries() const;
   std::vector<std::wstring> entryNames() const;

 private:
   HKEY m_key = NULL;
   bool m_created = false;
};


inline RegKey::operator bool() const
{
   return (m_key != NULL);
}

inline RegKey::operator HKEY() const
{
   return m_key;
}

inline HKEY RegKey::hkey() const
{
   return m_key;
}

inline bool RegKey::wasCreated() const
{
   return m_created;
}

inline bool RegKey::wasOpened() const
{
   return !m_created;
}

} // namespace win32

#endif //_WIN32
