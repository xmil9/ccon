//
// Win32 utilities library
// GDI object utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "gdi_object.h"


namespace win32
{

///////////////////

bool isValidBrush(HBRUSH hbrush)
{
   if (::GetObjectType(hbrush) != GdiObjectType::Brush)
      return false;
   LOGBRUSH info;
   return (::GetObject(hbrush, sizeof(LOGBRUSH), &info) != 0);
}


bool isValidPen(HPEN hpen)
{
   if (::GetObjectType(hpen) != GdiObjectType::Pen)
      return false;
   LOGPEN info;
   return (::GetObject(hpen, sizeof(LOGPEN), &info) != 0);
}

} // namespace win32

#endif //_WIN32
