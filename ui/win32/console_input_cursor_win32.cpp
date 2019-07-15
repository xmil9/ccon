//
// ccon
//
// Jul-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "console_input_cursor_win32.h"


namespace ccon
{
///////////////////

ConsoleInputCursorWin32::ConsoleInputCursorWin32(HWND hwnd, COLORREF color)
: m_hwnd{hwnd}, m_blickCallback{std::bind(&ConsoleInputCursorWin32::onBlink, this,
                                          std::placeholders::_1)},
  m_brush{CreateSolidBrush(color)}
{
}


ConsoleInputCursorWin32::~ConsoleInputCursorWin32()
{
   stop();
}


ConsoleInputCursorWin32::ConsoleInputCursorWin32(ConsoleInputCursorWin32&& other) noexcept
{
   *this = std::move(other);
}


ConsoleInputCursorWin32& ConsoleInputCursorWin32::
operator=(ConsoleInputCursorWin32&& other) noexcept
{
   m_hwnd = other.m_hwnd;
   // Recreate timed callback with the onBlink() member function of this instance.
   m_blickCallback = std::move(win32::TimedCallback{
      std::bind(&ConsoleInputCursorWin32::onBlink, this, std::placeholders::_1)});
   m_blinkRateMs = other.m_blinkRateMs;
   m_width = other.m_width;
   m_height = other.m_height;
   m_topLeft = other.m_topLeft;
   m_brush = std::move(other.m_brush);
   // Make sure dtor of moved-from object won't do anything.
   other.m_hwnd = NULL;
   other.m_blickCallback = std::move(win32::TimedCallback{});
   return *this;
}


void ConsoleInputCursorWin32::setBlinkRate(unsigned int blinkRateMs)
{
   m_blinkRateMs = blinkRateMs;
}


void ConsoleInputCursorWin32::setWidth(long w)
{
   m_width = w;
}


void ConsoleInputCursorWin32::setHeight(long h)
{
   m_height = h;
}


void ConsoleInputCursorWin32::setColor(COLORREF color)
{
   m_brush = std::move(win32::GdiObj<HBRUSH>(CreateSolidBrush(color)));
}


void ConsoleInputCursorWin32::setTopLeftPosition(const win32::Point& pos)
{
   m_topLeft = pos;
}


void ConsoleInputCursorWin32::start()
{
   if (m_blickCallback)
   {
      // Show immediately.
      m_isVisible = true;
      inval();

      m_blickCallback.start(m_blinkRateMs);
   }
}


void ConsoleInputCursorWin32::stop()
{
   if (m_blickCallback)
      m_blickCallback.stop();

   // Hide cursor when stopped.
   if (m_isVisible)
   {
      m_isVisible = false;
      inval();
   }
}


void ConsoleInputCursorWin32::inval()
{
   win32::Rect invalBounds{m_topLeft.x, m_topLeft.y, m_topLeft.x + m_width,
                           m_topLeft.y + m_height};
   InvalidateRect(m_hwnd, &invalBounds, !m_isVisible);
}


void ConsoleInputCursorWin32::draw(HDC hdc, const win32::Rect& wndBounds) const
{
   if (m_isVisible)
   {
      const win32::Rect cursorShape(m_topLeft.x, m_topLeft.y, m_topLeft.x + m_width,
                                    m_topLeft.y + m_height);

      auto [haveIntersection, intersection] = intersect(cursorShape, wndBounds);

      const bool isFullyVisible = (haveIntersection && intersection == cursorShape);
      if (isFullyVisible)
         FillRect(hdc, &cursorShape, m_brush);
   }
}


void ConsoleInputCursorWin32::onBlink(DWORD sysTime)
{
   m_isVisible = !m_isVisible;
   inval();
}

} // namespace ccon

#endif // _WIN32
