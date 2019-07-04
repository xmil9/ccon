//
// Win32 utilities library
//
// Jul-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "win32_util_api.h"


namespace win32
{
///////////////////

WIN32UTIL_API int vertScreenDpi();
WIN32UTIL_API int horzScreenDpi();

} // namespace win32

#endif //_WIN32
