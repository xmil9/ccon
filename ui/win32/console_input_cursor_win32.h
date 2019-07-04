//
// ccon
//
// Jul-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "win32_util/gdi_object.h"
#include "win32_util/geometry.h"
#include "win32_util/timer.h"
#include "win32_util/win32_windows.h"


namespace ccon
{
///////////////////

class ConsoleInputCursorWin32
{
 public:
   ConsoleInputCursorWin32() = default;
   explicit ConsoleInputCursorWin32(HWND hwnd, COLORREF color);
   ~ConsoleInputCursorWin32();
   ConsoleInputCursorWin32(const ConsoleInputCursorWin32&) = delete;
   ConsoleInputCursorWin32(ConsoleInputCursorWin32&& other) noexcept;
   ConsoleInputCursorWin32& operator=(const ConsoleInputCursorWin32&) = delete;
   ConsoleInputCursorWin32& operator=(ConsoleInputCursorWin32&& other) noexcept;

   // Settings will only take effect after stopping and restarting the cursor.
   void setBlinkRate(unsigned int blinkRateMs);
   void setWidth(long w);
   void setHeight(long h);
   void setColor(COLORREF color);
   void setTopLeftPosition(const win32::Point& pos);

   void start();
   void stop();

   void inval();
   void draw(HDC hdc, const win32::Rect& wndBounds) const;

 private:
   void onBlink(DWORD sysTime);

 private:
   HWND m_hwnd = NULL;
   win32::TimedCallback m_blickCallback;
   bool m_isVisible = false;
   unsigned int m_blinkRateMs = 500;
   long m_width = 1;
   long m_height = 10;
   win32::Point m_topLeft;
   win32::GdiObj<HBRUSH> m_brush;
};

} // namespace ccon

#endif // _WIN32
