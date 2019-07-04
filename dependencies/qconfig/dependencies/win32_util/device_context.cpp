//
// Win32 utilities library
// Device context utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "device_context.h"


namespace win32
{

///////////////////

SharedDC& SharedDC::operator=(SharedDC&& other) noexcept
{
   release();

   DC::operator=(std::move(other));
   m_sourceWnd = other.m_sourceWnd;
   other.m_sourceWnd = NULL;
   return *this;
}


void swap(SharedDC& a, SharedDC& b) noexcept
{
   swap(static_cast<DC&>(a), static_cast<DC&>(b));
   std::swap(a.m_sourceWnd, b.m_sourceWnd);
}


void SharedDC::clear()
{
   DC::clear();
   m_sourceWnd = NULL;
}


void SharedDC::release()
{
   if (hdc())
   {
      ::ReleaseDC(m_sourceWnd, hdc());
      clear();
   }
}


///////////////////

OwnedDC& OwnedDC::operator=(OwnedDC&& other) noexcept
{
   release();

   DC::operator=(std::move(other));
   return *this;
}


void swap(OwnedDC& a, OwnedDC& b) noexcept
{
   swap(static_cast<DC&>(a), static_cast<DC&>(b));
}


void OwnedDC::release()
{
   if (hdc())
   {
      ::DeleteDC(hdc());
      clear();
   }
}


///////////////////

PaintDC::PaintDC(HWND paintedWnd)
: DC{paintedWnd ? ::BeginPaint(paintedWnd, &m_paintInfo) : NULL}, m_paintedWnd{paintedWnd}
{
   if (!paintedWnd)
      clearPaintInfo(m_paintInfo);
}


PaintDC& PaintDC::operator=(PaintDC&& other) noexcept
{
   release();

   DC::operator=(std::move(other));
   m_paintedWnd = other.m_paintedWnd;
   m_paintInfo = other.m_paintInfo;
   return *this;
}


void swap(PaintDC& a, PaintDC& b) noexcept
{
   swap(static_cast<DC&>(a), static_cast<DC&>(b));
   std::swap(a.m_paintedWnd, b.m_paintedWnd);
   std::swap<PAINTSTRUCT>(a.m_paintInfo, b.m_paintInfo);
}


void PaintDC::clear()
{
   DC::clear();
   m_paintedWnd = NULL;
   clearPaintInfo(m_paintInfo);
}


void PaintDC::release()
{
   if (hdc())
   {
      ::EndPaint(m_paintedWnd, &m_paintInfo);
      clear();
   }
}


void PaintDC::clearPaintInfo(PAINTSTRUCT& info)
{
   std::memset(&info, 0, sizeof(PAINTSTRUCT));
}

} // namespace win32

#endif //_WIN32
