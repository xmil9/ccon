//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "console_content.h"
#include "console_input_cursor_win32.h"
#include "console_layout_win32.h"
#include "preferences.h"
#include "win32_util/gdi_object.h"
#include "win32_util/window.h"

namespace sutil
{
struct Rgb;
}


namespace ccon
{
///////////////////

class ConsoleWndWin32 : public win32::Window
{
 public:
   ConsoleWndWin32(const UserPrefs& prefs, ConsoleContent& content);
   ~ConsoleWndWin32() = default;
   ConsoleWndWin32(const ConsoleWndWin32&) = delete;
   ConsoleWndWin32(ConsoleWndWin32&& src) = default;
   ConsoleWndWin32& operator=(const ConsoleWndWin32&) = delete;
   ConsoleWndWin32& operator=(ConsoleWndWin32&& src) = default;

   void setBackgroundColor(const sutil::Rgb& color);
   void setOutputTextColor(const sutil::Rgb& color);
   void setInputTextColor(const sutil::Rgb& color);
   void resetColors();
   void setFontSize(int sizeInPoints);

 protected:
   const TCHAR* windowClassName() const override;
   bool registerWindowClass() const override;

   CreationResult onCreate(const CREATESTRUCT* createInfo) override;
   bool onDestroy() override;
   bool onPaint() override;
   bool onEraseBkgnd(HDC hdc) override;
   bool onSize(long width, long height, UINT resizeFlag) override;
   bool onKeyDown(UINT virtKeyCode, UINT repeatCount, BYTE scanCode, bool isExtendedKey,
                  bool wasPreviouslyDown) override;
   bool onChar(TCHAR ch, UINT repeatCount, BYTE scanCode, bool isExtendedKey,
               bool wasPreviouslyDown, bool isAltDown, bool isReleased) override;
   bool onSetFocus(HWND unfocusedWnd) override;
   bool onKillFocus(HWND focusedWnd) override;
   bool onVScroll(UINT scrollAction, UINT thumbPos, HWND scrollCtrl) override;
   bool onMouseWheel(int delta, UINT keyState, win32::Point mousePos) override;

 private:
   bool setupColors();
   bool setupFont();
   bool setupBackgroundBrush();
   void updateContentLayout(HDC hdc);
   void updateInputLine(HDC hdc);
   void invalInputLine();
   void refreshInputLine();
   void updateInputCursor();
   void cleanupInputCursor();
   void updateScrollbar();
   void drawBackground(HDC hdc, const win32::Rect& bounds);
   void drawContent(HDC hdc, const win32::Rect& bounds);
   void drawLine(HDC hdc, win32::Rect bounds, std::string text);
   void drawEnteredLine(HDC hdc, win32::Rect bounds, const std::string& text);
   void drawInputCursor(HDC hdc);
   void moveInputCursor(int offset);
   void moveInputCursorToEnd();
   bool handleEditKey(UINT virtKeyCode);
   bool handleInputKey(TCHAR tch);
   void deleteInputCharacter(std::size_t charIdx, int cursorOffset);
   void processInputLine();
   void displayPreviousInput();
   void displayNextInput();
   void autoCompleteInput();
   void scrollVertical(UINT scrollAction, UINT thumbPos);
   void scrollVerticalBy(int numLines);
   void scrollIntoView();

 private:
   UserPrefs m_userPrefs;
   ConsoleContent& m_content;
   ConsoleLayoutWin32 m_layout;
   bool m_isLayoutInited = false;
   ConsoleInputCursorWin32 m_inputCursor;
   win32::GdiObj<HFONT> m_font;
   win32::GdiObj<HBRUSH> m_backgroundBrush;
   COLORREF m_textOutputColor = 0;
   COLORREF m_textInputColor = 0;
};

} // namespace ccon

#endif // _WIN32
