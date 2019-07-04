//
// Win32 utilities library
// Error utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "err_util.h"
#include <tchar.h>


namespace
{
///////////////////

// RAII class for freeing memory allocated by LocalAlloc.
// Implemented locally to keep the err_util component as free of dependencies as
// possible.
class LocalMem
{
 public:
   LocalMem() = delete;
   LocalMem(LPTSTR p) : m_ptr{p} {}
   ~LocalMem()
   {
      if (m_ptr)
         LocalFree(m_ptr);
   }
   // Copy-ctor and copy-assignment should not be available.
   LocalMem(const LocalMem&) = delete;
   LocalMem& operator=(const LocalMem&) = delete;
   // Copy-ctor and copy-assignment can be available but for this local implementation
   // are not necessary.
   LocalMem(LocalMem&&) = delete;
   LocalMem& operator=(LocalMem&&) = delete;

 private:
   LPTSTR m_ptr = nullptr;
};

} // namespace


namespace win32
{
namespace detail
{
///////////////////

// Returns error text for given Win32 error code.
TString winErrorNativeText(DWORD errCode)
{
   // Pointer to a buffer that will be allocated by FormatMessage().
   TCHAR* buffer = nullptr;
   const DWORD charsWritten =
      ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                         FORMAT_MESSAGE_IGNORE_INSERTS,
                      nullptr, errCode, 0, (LPTSTR)&buffer,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), nullptr);
   if (charsWritten == 0)
      return _T("<<Failed to get description of Windows error.>>");
   // FormatMessage documentation states to free returned memory with LocalFree.
   LocalMem guardedMem(buffer);

   TString errText = buffer;

   // Win32 API error messages sometimes contain newline character sequences at
   // the end. Strip them off.
   const TString Newline = _T("\x0d\x0a");
   const TString::size_type pos = errText.rfind(Newline);
   if (pos != TString::npos)
      errText.replace(pos, Newline.size(), _T(""));

   return errText;
}

} // namespace detail
} // namespace win32

#endif //_WIN32
