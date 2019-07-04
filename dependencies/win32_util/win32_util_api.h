//
// Win32 utilities library
// Macros for importing/exporting functionality from the library.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32

// Projects that use the DLL target build of win32-util have
// to define the WIN32UTIL_DLL macro to make WIN32UTIL_API
// resolve to '__declspec(dllimport)'.
#ifdef WIN32UTIL_DLL
#  ifdef EXPORT_API
#     define WIN32UTIL_API __declspec(dllexport)
#  else
#     define WIN32UTIL_API __declspec(dllimport)
#  endif
#endif

// If not defined yet, define it as empty/nothing.
#ifndef WIN32UTIL_API
#  define WIN32UTIL_API
#endif

#endif //_WIN32
