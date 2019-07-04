//
// Win32 utilities library
// Window utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "device_context.h"
#include "geometry.h"
#include "tstring.h"
#include "win32_util_api.h"
#include "win32_windows.h"
#include <string>
#include <utility>


namespace win32
{

///////////////////

// Wrapper for Win32 HWND handles.
// Provides interface to interact with windows. No ownership semantics.
// Destroying the window clears the internal hwnd handle.
class WIN32UTIL_API Window
{
 public:
   Window() = default;
   explicit Window(HWND hwnd);
   virtual ~Window() = default;
   Window(const Window&) = delete;
   Window(Window&& src) = default;
   Window& operator=(const Window&) = delete;
   Window& operator=(Window&& src) = default;

   explicit operator bool() const;
   operator HWND() const;
   friend void swap(Window& a, Window& b) noexcept;

   HWND hwnd() const;
   bool create(HWND parent, const Rect& bounds, const TString& title, unsigned long style,
               unsigned long extStyle = 0);
   // Posts WM_CLOSE message.
   void close();
   // Sends WM_CLOSE message.
   void closeImmediately();
   // Checks if the window exists. Calls IsWindow() Win32 function.
   bool exists() const;
   bool isVisible() const;
   void setVisible(bool visible);
   Rect clientBounds() const;
   Rect windowBounds() const;
   SharedDC clientDC() const;
   SharedDC windowDC() const;
   HWND parent() const;
   HWND owner() const;
   TString title() const;
   void setTitle(const TString& title);
   void inval(bool erase);
   void inval(const win32::Rect& bounds, bool erase);
   std::pair<bool, Rect> invalBounds() const;
   bool haveInvalBounds() const;
   bool postMessage(UINT msgId, WPARAM wParam = 0, LPARAM lParam = 0) const;
   LRESULT sendMessage(UINT msgId, WPARAM wParam = 0, LPARAM lParam = 0) const;

 protected:
   enum class CreationResult
   {
      Handled,
      NotHandled,
      Abort
   };

   virtual HINSTANCE appInstance() const;
   virtual const TCHAR* windowClassName() const;
   bool isWindowClassRegistered() const;
   virtual bool registerWindowClass() const;

   static LRESULT CALLBACK windowProc(HWND hwnd, UINT msgId, WPARAM wParam,
                                      LPARAM lParam);
   virtual LRESULT handleMessage(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam);
   virtual CreationResult onNcCreate(HWND hwnd, const CREATESTRUCT* createInfo);
   virtual bool onNcDestroy() { return false; }
   virtual CreationResult onCreate(const CREATESTRUCT* createInfo);
   virtual bool onDestroy() { return false; }
   virtual bool onClose();
   virtual bool onNcPaint(HRGN updateRegion) { return false; }
   virtual bool onPaint() { return false; }
   virtual bool onEraseBkgnd(HDC hdc) { return false; }
   virtual bool onSize(long width, long height, UINT resizeFlag);
   virtual bool onCommand(int id, UINT notificationCode, HWND ctrlWnd);
   virtual bool onKeyDown(UINT virtKeyCode, UINT repeatCount, BYTE scanCode,
                          bool isExtendedKey, bool wasPreviouslyDown);
   virtual bool onChar(TCHAR ch, UINT repeatCount, BYTE scanCode, bool isExtendedKey,
                       bool wasPreviouslyDown, bool isAltDown, bool isReleased);
   virtual bool onSetFocus(HWND unfocusedWnd) { return false; }
   virtual bool onKillFocus(HWND focusedWnd) { return false; }
   virtual bool onHScroll(UINT scrollAction, UINT thumbPos, HWND scrollCtrl);
   virtual bool onVScroll(UINT scrollAction, UINT thumbPos, HWND scrollCtrl);
   virtual bool onMouseMove(win32::Point mousePos, UINT virtKeyCode);
   virtual bool onMouseWheel(int delta, UINT keyState, Point mousePos);
   virtual bool onMouseHorzWheel(int delta, UINT keyState, Point mousePos);
   virtual bool onTimer(UINT_PTR timerId, TIMERPROC callback) { return false; }

 private:
   void setupWindow(HWND hwnd);
   void cleanupWindow();

 private:
   HWND m_hwnd = NULL;
};


inline Window::Window(HWND hwnd) : m_hwnd{hwnd}
{
}

inline Window::operator bool() const
{
   return hwnd() != NULL;
}

inline Window::operator HWND() const
{
   return hwnd();
}

inline void swap(Window& a, Window& b) noexcept
{
   std::swap(a.m_hwnd, b.m_hwnd);
}

inline HWND Window::hwnd() const
{
   return m_hwnd;
}

inline bool Window::haveInvalBounds() const
{
   return invalBounds().first;
}

inline bool Window::onSize(long width, long height, UINT resizeFlag)
{
   return false;
}

inline bool Window::onCommand(int id, UINT notificationCode, HWND ctrlWnd)
{
   return false;
}

inline bool Window::onKeyDown(UINT virtKeyCode, UINT repeatCount, BYTE scanCode,
                              bool isExtendedKey, bool wasPreviouslyDown)
{
   return false;
}

inline bool Window::onChar(TCHAR ch, UINT repeatCount, BYTE scanCode, bool isExtendedKey,
                           bool wasPreviouslyDown, bool isAltDown, bool isReleased)
{
   return false;
}

inline bool Window::onHScroll(UINT scrollAction, UINT thumbPos, HWND scrollCtrl)
{
   return false;
}

inline bool Window::onVScroll(UINT scrollAction, UINT thumbPos, HWND scrollCtrl)
{
   return false;
}

inline bool Window::onMouseMove(win32::Point mousePos, UINT virtKeyCode)
{
   return false;
}

inline bool Window::onMouseWheel(int delta, UINT keyState, Point mousePos)
{
   return false;
}

inline bool Window::onMouseHorzWheel(int delta, UINT keyState, Point mousePos)
{
   return false;
}

} // namespace win32

#endif //_WIN32
