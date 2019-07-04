//
// Win32 utilities library
//
// Jul-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "screen.h"
#include "win32_windows.h"


namespace win32
{
///////////////////

int vertScreenDpi()
{
   const HDC screenDc = GetDC(NULL);
   return GetDeviceCaps(screenDc, LOGPIXELSY);
}


int horzScreenDpi()
{
   const HDC screenDc = GetDC(NULL);
   return GetDeviceCaps(screenDc, LOGPIXELSX);
}

} // namespace win32

#endif //_WIN32
