//
// Win32 utilities library
// Messaging utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "tstring.h"
#include "win32_util_api.h"
#include "win32_windows.h"


namespace win32
{

///////////////////

struct MsgLoopResult
{
   int exitCode = EXIT_SUCCESS;
   DWORD err = ERROR_SUCCESS;
};

// Runs a main message loop.
WIN32UTIL_API MsgLoopResult mainMessageLoop(HACCEL accelerators);

// Runs a message loop for a given window. Allows to bring up windows within sequential
// code (i.e. code that does not directly interact with the main message loop).
// If a parent window is given, it will be disabled while the message loop is active.
// The caller is responsible to flag when the message loop should stop executing by
// setting a given stop flag to 'true'.
// Posting a WM_QUIT message will also stop the message loop and will be forwarded to
// the main message loop.
WIN32UTIL_API void modalMessageLoop(HWND wnd, bool& stopFlag, HWND parentWnd = NULL);

} // namespace win32

#endif //_WIN32
