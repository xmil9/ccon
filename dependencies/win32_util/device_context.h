//
// Win32 utilities library
// Device context utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "gdi_object.h"
#include "win32_util_api.h"
#include "win32_windows.h"
#include <cstring>
#include <utility>


namespace win32
{
///////////////////

// Provides DC functionality without any ownership sematics.
class WIN32UTIL_API DC
{
 public:
   DC() = default;
   explicit DC(HDC hdc);
   virtual ~DC() = default;
   DC(const DC&) = default;
   DC(DC&& other) noexcept;
   DC& operator=(const DC&) = default;
   DC& operator=(DC&& other) noexcept;

   HDC hdc() const;
   explicit operator bool() const;
   operator HDC() const;
   friend void swap(DC& a, DC& b) noexcept;
   virtual void clear();
   template <typename H> H selectedObject(GdiObjectType objType) const;
   template <typename H> DeselectedObj<H> selectObject(H h);

 private:
   HDC m_hdc = NULL;
};


inline DC::DC(HDC hdc) : m_hdc{hdc}
{
}

inline DC::DC(DC&& other) noexcept
{
   swap(*this, other);
}

inline DC& DC::operator=(DC&& other) noexcept
{
   m_hdc = other.m_hdc;
   other.m_hdc = NULL;
   return *this;
}

inline HDC DC::hdc() const
{
   return m_hdc;
}

inline DC::operator bool() const
{
   return (m_hdc != NULL);
}

inline DC::operator HDC() const
{
   return m_hdc;
}

inline void swap(DC& a, DC& b) noexcept
{
   std::swap(a.m_hdc, b.m_hdc);
}

inline void DC::clear()
{
   m_hdc = NULL;
}

template <typename H> H DC::selectedObject(GdiObjectType objType) const
{
   if (hdc())
      return reinterpret_cast<H>(::GetCurrentObject(hdc(), objType));
   return NULL;
}

template <typename H> DeselectedObj<H> DC::selectObject(H h)
{
   if (hdc())
      return win32::selectObject(hdc(), h);
   return {};
}


///////////////////

// RAII class for DCs that are obtained by calling GetDC() or GetWindowDC() and freed by
// calling ReleaseDC().
class WIN32UTIL_API SharedDC : public DC
{
 public:
   SharedDC() = default;
   SharedDC(HDC hdc, HWND sourceWnd);
   ~SharedDC();
   SharedDC(const SharedDC&) = delete;
   SharedDC(SharedDC&& other) noexcept;
   SharedDC& operator=(const SharedDC&) = delete;
   SharedDC& operator=(SharedDC&& other) noexcept;

   HWND hwnd() const;
   friend WIN32UTIL_API void swap(SharedDC& a, SharedDC& b) noexcept;
   void clear() override;
   void release();

 private:
   HWND m_sourceWnd = NULL;
};


inline SharedDC::SharedDC(HDC hdc, HWND sourceWnd) : DC{hdc}, m_sourceWnd{sourceWnd}
{
}

inline SharedDC::~SharedDC()
{
   release();
}

inline SharedDC::SharedDC(SharedDC&& other) noexcept
{
   swap(*this, other);
}

inline HWND SharedDC::hwnd() const
{
   return m_sourceWnd;
}



///////////////////

// RAII class for DCs that are created by calling CreateDC() or CreateCompatibleDC() and
// freed by calling DeleteDC().
class WIN32UTIL_API OwnedDC : public DC
{
 public:
   OwnedDC() = default;
   explicit OwnedDC(HDC hdc);
   ~OwnedDC();
   OwnedDC(const OwnedDC&) = delete;
   OwnedDC(OwnedDC&& other) noexcept;
   OwnedDC& operator=(const OwnedDC&) = delete;
   OwnedDC& operator=(OwnedDC&& other) noexcept;

   friend WIN32UTIL_API void swap(OwnedDC& a, OwnedDC& b) noexcept;
   void release();
};


inline OwnedDC::OwnedDC(HDC hdc) : DC{hdc}
{
}

inline OwnedDC::~OwnedDC()
{
   release();
}

inline OwnedDC::OwnedDC(OwnedDC&& other) noexcept : DC{std::move(other)}
{
}


///////////////////

// RAII class for the HDC used in paint operations.
// Can easily be extended to also select and restore DC properties for specific
// paint operations.
class WIN32UTIL_API PaintDC : public DC
{
 public:
   PaintDC();
   explicit PaintDC(HWND paintedWnd);
   virtual ~PaintDC();
   PaintDC(const PaintDC&) = delete;
   PaintDC(PaintDC&& other) noexcept;
   PaintDC& operator=(const PaintDC&) = delete;
   PaintDC& operator=(PaintDC&& other) noexcept;

   HWND hwnd() const;
   const PAINTSTRUCT& paintInfo() const;
   friend WIN32UTIL_API void swap(PaintDC& a, PaintDC& b) noexcept;
   void clear() override;
   // End the paint operation.
   void release();

 private:
   static void clearPaintInfo(PAINTSTRUCT& info);

 private:
   HWND m_paintedWnd = NULL;
   PAINTSTRUCT m_paintInfo;
};


inline PaintDC::PaintDC() : m_paintedWnd{NULL}
{
   clearPaintInfo(m_paintInfo);
}

inline PaintDC::~PaintDC()
{
   release();
}

inline PaintDC::PaintDC(PaintDC&& other) noexcept
: DC{std::move(other)}, m_paintedWnd{other.m_paintedWnd}, m_paintInfo{other.m_paintInfo}
{
}

inline HWND PaintDC::hwnd() const
{
   return m_paintedWnd;
}

inline const PAINTSTRUCT& PaintDC::paintInfo() const
{
   return m_paintInfo;
}

} // namespace win32

#endif //_WIN32
