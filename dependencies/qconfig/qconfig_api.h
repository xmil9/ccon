//
// Quick config
// Import/export directives.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once

// Projects that use the DLL target build of quick config
// have to define the QCONFIG_DLL macro to make QCONFIG_API
// resolve to '__declspec(dllimport)'.
#ifdef QCONFIG_DLL
#  ifdef EXPORT_API
#    ifdef _MSC_VER
#      define QCONFIG_API __declspec(dllexport)
#    endif
#  else
#    ifdef _MSC_VER
#      define QCONFIG_API __declspec(dllimport)
#    endif
#  endif
#endif

// If not defined yet, define it as empty/nothing.
#ifndef QCONFIG_API
#  define QCONFIG_API
#endif
