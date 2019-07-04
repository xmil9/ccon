//
// Win32 utilities library
// Error utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "tstring.h"
#include "win32_util_api.h"
#include "win32_windows.h"
#include "essentutils/string_util.h"
#include <comdef.h>

namespace win32::detail {
WIN32UTIL_API TString winErrorNativeText(DWORD errCode);
}


namespace win32
{
///////////////////

// Interface

// Returns the description of a COM error.
template <typename Str> Str comErrorText(HRESULT hr) noexcept;
// Returns the description of a Win32 error.
template <typename Str> Str winErrorText(DWORD err) noexcept;
// Returns the description of the last Win32 error or a passed default text if no last
// error exists.
template <typename Str> Str lastWinErrorText(const Str& defaultText = {}) noexcept;


///////////////////

// Implementation

template <typename Str> Str comErrorText(HRESULT hr) noexcept
{
   try
   {
      _com_error err(hr);
      return sutil::convertTo<Str>(err.ErrorMessage());
   }
   catch (...)
   {
      // We cannot let an exception escape from this function because it might
      // get called from an already exceptional workflow.
      return sutil::convertTo<Str>("<<Failed to get description of COM error.>>");
   }
}


template <typename Str> Str winErrorText(DWORD err) noexcept
{
   try
   {
      return sutil::convertTo<Str>(detail::winErrorNativeText(err));
   }
   catch (...)
   {
      // We cannot let an exception escape from this function because it might
      // get called from an already exceptional workflow.
      return sutil::convertTo<Str>("<<Failed to get description of Windows error.>>");
   }
}


template <typename Str> Str lastWinErrorText(const Str& defaultText) noexcept
{
   try
   {
      const DWORD err = GetLastError();
      return (err != 0) ? winErrorText<Str>(err) : defaultText;
   }
   catch (...)
   {
      // We cannot let an exception escape from this function because it might
      // get called from an already exceptional workflow.
      return sutil::convertTo<Str>("<<Failed to get description of last Windows error.>>");
   }
}

} // namespace win32

#endif //_WIN32
