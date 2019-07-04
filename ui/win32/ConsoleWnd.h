#pragma once
#include "ConsoleInputCursor.h"
#include "ConsoleLayout.h"
#include "ConsolePreferences.h"
#include "Win32GdiObject.h"
#include "Win32Timer.h"
#include "Win32Window.h"
#include <type_traits>

struct ConsoleContent;
namespace cvfc
{
struct Rgb;
}
namespace win32
{
struct Rect;
}


///////////////////

class ConsoleWnd : public win32::Window
{
 public:
   ConsoleWnd(const ConsoleUIUserPrefs& prefs, ConsoleContent& content);
   ~ConsoleWnd() = default;
   ConsoleWnd(const ConsoleWnd&) = delete;
   ConsoleWnd& operator=(const ConsoleWnd&) = delete;
   ConsoleWnd(ConsoleWnd&& src) = default;
   ConsoleWnd& operator=(ConsoleWnd&& src) = default;

   void SetBackgroundColor(const cvfc::Rgb& color);
   void SetOutputTextColor(const cvfc::Rgb& color);
   void SetInputTextColor(const cvfc::Rgb& color);
   void ResetColors();
   void SetFontSize(int sizeInPoints);

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
   ConsoleContent& GetContent();
   const ConsoleContent& GetContent() const;
   bool SetupColors();
   bool SetupFont();
   bool SetupBackgroundBrush();
   void UpdateContentLayout(HDC hdc);
   void UpdateInputLine(HDC hdc);
   void InvalInputLine();
   void RefreshInputLine();
   void UpdateInputCursor();
   void CleanupInputCursor();
   void UpdateScrollbar();
   void DrawBackground(HDC hdc, const win32::Rect& bounds);
   void DrawContent(HDC hdc, const win32::Rect& bounds);
   void DrawLine(HDC hdc, win32::Rect bounds, CVString text);
   void DrawEnteredLine(HDC hdc, win32::Rect bounds, CVString text);
   void DrawInputCursor(HDC hdc);
   void MoveInputCursor(CVINT32 offset);
   void MoveInputCursorToEnd();
   bool HandleEditKey(UINT virtKeyCode);
   bool HandleInputKey(TCHAR ch);
   void DeleteInputCharacter(std::size_t charIdx, CVINT32 cursorOffset);
   void ProcessInputLine();
   void DisplayPreviousInput();
   void DisplayNextInput();
   void AutoCompleteInput();
   void ScrollVertical(UINT scrollAction, UINT thumbPos);
   void ScrollVerticalBy(int numLines);
   void ScrollIntoView();

 private:
   ConsoleUIUserPrefs m_userPrefs;
   std::reference_wrapper<ConsoleContent> m_content;
   ConsoleLayout m_layout;
   bool m_isLayoutInited = false;
   ConsoleInputCursor m_inputCursor;
   win32::GdiObj<HFONT> m_font;
   win32::GdiObj<HBRUSH> m_backgroundBrush;
   COLORREF m_textOutputColor = 0;
   COLORREF m_textInputColor = 0;
};


inline ConsoleContent& ConsoleWnd::GetContent()
{
   return m_content.get();
}

inline const ConsoleContent& ConsoleWnd::GetContent() const
{
   return m_content.get();
}
