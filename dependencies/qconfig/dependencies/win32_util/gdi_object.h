//
// Win32 utilities library
// GDI object utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "win32_util_api.h"
#include "win32_windows.h"
#include <utility>


namespace win32
{
///////////////////

// Types of GDI objects.
enum GdiObjectType : UINT
{
   None = 0,
   Pen = OBJ_PEN,
   Brush = OBJ_BRUSH,
   DeviceContext = OBJ_DC,
   MetafileDC = OBJ_METADC,
   Palette = OBJ_PAL,
   Font = OBJ_FONT,
   Bitmap = OBJ_BITMAP,
   Region = OBJ_REGION,
   Metafile = OBJ_METAFILE,
   MemoryDC = OBJ_MEMDC,
   ExtendedPen = OBJ_EXTPEN,
   EnhancedMetafileDC = OBJ_ENHMETADC,
   EnhancedMetafile = OBJ_ENHMETAFILE,
   ColorSpace = OBJ_COLORSPACE,
};


///////////////////

// RAII class for GDI objects whose resources are released by calling DeleteObject.
template <typename H> class GdiObj
{
   static_assert(
      std::is_same_v<H, HPEN> || std::is_same_v<H, HBRUSH> || std::is_same_v<H, HFONT> ||
         std::is_same_v<H, HBITMAP> || std::is_same_v<H, HRGN> ||
         std::is_same_v<H, HPALETTE>,
      "GDI object of type HPEN, HBRUSH, HFONT, HBITMAP, HRGN, or HPALETTE required.");

 public:
   GdiObj() = default;
   explicit GdiObj(H objHandle);
   ~GdiObj();
   GdiObj(const GdiObj&) = delete;
   GdiObj(GdiObj&& other) noexcept;
   GdiObj& operator=(const GdiObj&) = delete;
   GdiObj& operator=(GdiObj&& other) noexcept;

   explicit operator bool() const;
   operator H() const;
   H handle() const;
   template <typename H> friend void swap(GdiObj<H>& a, GdiObj<H>& b) noexcept;
   // Resets internal state. Does not delete the associated GDI object.
   void clear();
   // Delete the associated GDI object and resets the internal state.
   void release();

 private:
   H m_h = NULL;
};


template <typename H> GdiObj<H>::GdiObj(H objHandle) : m_h{objHandle}
{
}

template <typename H> GdiObj<H>::~GdiObj()
{
   release();
}

template <typename H> GdiObj<H>::GdiObj(GdiObj&& other) noexcept
{
   swap(*this, other);
}

template <typename H> GdiObj<H>& GdiObj<H>::operator=(GdiObj&& other) noexcept
{
   release();
   m_h = other.m_h;
   other.m_h = NULL;
   return *this;
}

template <typename H> GdiObj<H>::operator bool() const
{
   return m_h != NULL;
}

template <typename H> GdiObj<H>::operator H() const
{
   return m_h;
}

template <typename H> H GdiObj<H>::handle() const
{
   return m_h;
}

template <typename H> void swap(GdiObj<H>& a, GdiObj<H>& b) noexcept
{
   std::swap(a.m_h, b.m_h);
}

template <typename H> void GdiObj<H>::clear()
{
   m_h = NULL;
}

template <typename H> void GdiObj<H>::release()
{
   if (m_h)
   {
      ::DeleteObject(m_h);
      clear();
   }
}


///////////////////

// RAII class for restoring a GDI object that has been deselected from a HDC.
// Supports GDI object that are selected into a DC by calling SelectObject().
template <typename H> class DeselectedObj
{
   // Note that HPALETTE objects are not valid for this class because they are
   // selected/restored by calling SelectPalette().
   static_assert(std::is_same_v<H, HPEN> || std::is_same_v<H, HBRUSH> ||
                    std::is_same_v<H, HFONT> || std::is_same_v<H, HBITMAP> ||
                    std::is_same_v<H, HRGN>,
                 "GDI object of type HPEN, HBRUSH, HFONT, HBITMAP, or HRGN required.");

 public:
   DeselectedObj() = default;
   DeselectedObj(HDC hdc, H objHandle);
   ~DeselectedObj();
   DeselectedObj(const DeselectedObj&) = delete;
   DeselectedObj(DeselectedObj&& other) noexcept;
   DeselectedObj& operator=(const DeselectedObj&) = delete;
   DeselectedObj& operator=(DeselectedObj&& other) noexcept;

   HDC hdc() const;
   H handle() const;
   explicit operator bool() const;
   template <typename H>
   friend void swap(DeselectedObj<H>& a, DeselectedObj<H>& b) noexcept;
   void clear();
   void restore();

 private:
   HDC m_hdc = NULL;
   H m_h = NULL;
};


template <typename H>
DeselectedObj<H>::DeselectedObj(HDC hdc, H objHandle) : m_hdc{hdc}, m_h{objHandle}
{
}

template <typename H> DeselectedObj<H>::~DeselectedObj()
{
   restore();
}

template <typename H> DeselectedObj<H>::DeselectedObj(DeselectedObj&& other) noexcept
{
   swap(*this, other);
}

template <typename H>
DeselectedObj<H>& DeselectedObj<H>::operator=(DeselectedObj&& other) noexcept
{
   restore();

   m_hdc = other.m_hdc;
   m_h = other.m_h;
   // Make sure dtor of moved-from object does nothing.
   other.clear();
   return *this;
}

template <typename H> HDC DeselectedObj<H>::hdc() const
{
   return m_hdc;
}

template <typename H> H DeselectedObj<H>::handle() const
{
   return m_h;
}

template <typename H> DeselectedObj<H>::operator bool() const
{
   return m_hdc != NULL;
}

template <typename H> void swap(DeselectedObj<H>& a, DeselectedObj<H>& b) noexcept
{
   std::swap(a.m_hdc, b.m_hdc);
   std::swap(a.m_h, b.m_h);
}

template <typename H> void DeselectedObj<H>::clear()
{
   m_hdc = NULL;
   m_h = NULL;
}

template <typename H> void DeselectedObj<H>::restore()
{
   if (m_hdc)
      ::SelectObject(m_hdc, m_h);
   clear();
}


///////////////////

// Wraps the Win32 SelectObject() function to return a RAII object for the deselected
// GDI object.
template <typename H> DeselectedObj<H> selectObject(HDC hdc, H objHandle)
{
   return DeselectedObj<H>{hdc, reinterpret_cast<H>(::SelectObject(hdc, objHandle))};
}

// Checks if a given GDI handle is valid, i.e. created but not deleted.
WIN32UTIL_API bool isValidBrush(HBRUSH hbrush);
WIN32UTIL_API bool isValidPen(HPEN hpen);

} // namespace win32

#endif //_WIN32
