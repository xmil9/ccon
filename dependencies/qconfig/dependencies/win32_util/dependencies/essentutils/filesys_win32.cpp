//
// essentutils
// File system utilities.
//
// Jul-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "filesys.h"
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#include <windows.h>
#include <array>

namespace fs = std::filesystem;


namespace
{
///////////////////

template <typename T>
struct CoTaskMem
{
   CoTaskMem(T* p)
      : ptr{p}
   {
   }
   ~CoTaskMem()
   {
      if (ptr)
         CoTaskMemFree(ptr);
   }

   T* ptr = nullptr;
};

} // namespace


namespace sutil
{
///////////////////

std::filesystem::path appDirectory()
{
   std::array<char, MAX_PATH> buffer;
   const DWORD res =
      GetModuleFileNameA(NULL, buffer.data(), static_cast<DWORD>(buffer.size()));
   if (res == 0 || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
      return {};

   fs::path appPath{buffer.data()};
   appPath.remove_filename();

   return appPath;
}


std::filesystem::path userDirectory()
{
   wchar_t* appDataDir = nullptr;
   HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &appDataDir);
   if (FAILED(hr))
      return {};

   CoTaskMem deleter{appDataDir};
   return fs::path{appDataDir};

}

} // namespace sutil

#endif // _WIN32
