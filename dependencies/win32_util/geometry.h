//
// Win32 utilities library
// Geometry utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "win32_windows.h"
#include <algorithm>
#include <cmath>
#include <utility>

#ifdef max
#  undef max
#endif
#ifdef min
#  undef min
#endif


namespace win32
{

///////////////////

// Point data structure that is compatible with Win32's POINT but more
// convenient to use.
struct Point : POINT
{
   Point();
   explicit Point(const POINT& pt);
   Point(long x, long y);
   Point(const Point&) = default;
   Point(Point&&) = default;
   Point& operator=(const Point&) = default;
   Point& operator=(Point&&) = default;

   friend void swap(Point& a, Point& b) noexcept;
};


inline Point::Point() : Point{0, 0}
{
}

inline Point::Point(const POINT& pt) : Point{pt.x, pt.y}
{
}

inline Point::Point(long x, long y) : POINT{x, y}
{
}

inline void swap(Point& a, Point& b) noexcept
{
   std::swap(a.x, b.x);
   std::swap(a.y, b.y);
}

// Comparision.

inline bool operator==(const Point& a, const Point& b)
{
   return (a.x == b.x && a.y == b.y);
}

inline bool operator!=(const Point& a, const Point& b)
{
   return !(a == b);
}


///////////////////

// Rectangle data structure that is compatible with Win32's RECT but more
// convenient to use.
struct Rect : RECT
{
   Rect();
   Rect(const RECT& r);
   Rect(long l, long t, long r, long b);
   Rect(const Rect&) = default;
   Rect(Rect&&) = default;
   Rect& operator=(const Rect&) = default;
   Rect& operator=(Rect&&) = default;

   friend void swap(Rect& a, Rect& b) noexcept;
   long width() const;
   long height() const;
   void offset(long x, long y);
};


inline Rect::Rect() : Rect{0, 0, 0, 0}
{
}

inline Rect::Rect(const RECT& r) : Rect{r.left, r.top, r.right, r.bottom}
{
}

inline Rect::Rect(long l, long t, long r, long b) : RECT{l, t, r, b}
{
}

inline void swap(Rect& a, Rect& b) noexcept
{
   std::swap(a.left, b.left);
   std::swap(a.top, b.top);
   std::swap(a.right, b.right);
   std::swap(a.bottom, b.bottom);
}

inline long Rect::width() const
{
   return std::abs(right - left);
}

inline long Rect::height() const
{
   return std::abs(bottom - top);
}

inline void Rect::offset(long x, long y)
{
   left += x;
   right += x;
   top += y;
   bottom += y;
}

// Comparision.

inline bool operator==(const Rect& a, const Rect& b)
{
   return (a.left == b.left && a.top == b.top && a.right == b.right &&
           a.bottom == b.bottom);
}

inline bool operator!=(const Rect& a, const Rect& b)
{
   return !(a == b);
}

// Operations

inline std::pair<bool, Rect> intersect(const Rect& a, const Rect& b)
{
   const Rect intersection{std::max(a.left, b.left), std::max(a.top, b.top),
                           std::min(a.right, b.right), std::min(a.bottom, b.bottom)};
   const bool haveIntersection = (intersection.right > intersection.left &&
                                  intersection.bottom > intersection.top);
   return {haveIntersection, intersection};
}

inline Rect unite(const Rect& a, const Rect& b)
{
   return Rect{std::min(a.left, b.left), std::min(a.top, b.top),
               std::max(a.right, b.right), std::max(a.bottom, b.bottom)};
}

} // namespace win32

#endif //_WIN32
