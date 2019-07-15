//
// Win32 utilities library
// Messaging utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "message_util.h"
#include <cassert>


namespace win32
{
///////////////////

MsgLoopResult mainMessageLoop(HACCEL accelerators)
{
   MsgLoopResult result;

   MSG msg;
   while (true)
   {
      int getResult = ::GetMessage(&msg, nullptr, 0, 0);
      if (getResult > 0)
      {
         if (!::TranslateAccelerator(msg.hwnd, accelerators, &msg))
         {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
         }
      }
      else if (getResult < 0)
      {
         result.exitCode = EXIT_FAILURE;
         result.err = ::GetLastError();
         break;
      }
      else
      {
         // WM_QUIT message received.
         assert(getResult == 0);
         result.exitCode = static_cast<int>(msg.wParam);
         break;
      }
   }

   return result;
}


void modalMessageLoop(HWND modalWnd, bool& stopFlag, HWND parentWnd)
{
   // Disable the parent.
   if (parentWnd)
      ::EnableWindow(parentWnd, false);

   MSG msg;
   while (!stopFlag)
   {
      // Yield to other threads while there are no messages.
      ::WaitMessage();

      while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
      {
         // Detect and forward WM_QUIT messages. Stop the message loop immediately.
         if (msg.message == WM_QUIT)
         {
            stopFlag = true;
            ::PostMessage(NULL, WM_QUIT, 0, 0);
            break;
         }

         // Dispatch messages.
         if (!::IsDialogMessage(modalWnd, &msg))
         {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
         }
      }
   }

   // Re-enable parent.
   if (parentWnd)
      ::EnableWindow(parentWnd, true);
}

} // namespace win32

#endif //_WIN32
