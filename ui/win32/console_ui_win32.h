//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "console_content.h"
#include "console_ui.h"
#include "console_wnd_win32.h"
#include "preferences.h"
#include "win32_util/win32_windows.h"

namespace win32
{
struct Rect;
}
namespace sutil
{
struct Rgb;
}


namespace ccon
{
///////////////////

class ConsoleUIWin32 : public ConsoleUI
{
 public:
   ConsoleUIWin32(HWND parentWnd, const std::wstring& title, const UserPrefs& prefs);
   ~ConsoleUIWin32() = default;
   ConsoleUIWin32(const ConsoleUIWin32&) = delete;
   ConsoleUIWin32(ConsoleUIWin32&&) = delete;
   ConsoleUIWin32& operator=(const ConsoleUIWin32&) = delete;
   ConsoleUIWin32& operator=(ConsoleUIWin32&&) = delete;

   void setContent(ConsoleContent* content) override;
   void showConsole() override;
   void hideConsole() override;
   void setBackgroundColor(const sutil::Rgb& color) override;
   void setOutputTextColor(const sutil::Rgb& color) override;
   void setInputTextColor(const sutil::Rgb& color) override;
   void resetColors() override;
   void setFontSize(int sizeInPoints) override;

 private:
   win32::Rect calcConsoleBounds() const;

 private:
   HWND m_parentWnd = NULL;
   std::wstring m_title;
   UserPrefs m_userPrefs;
   ConsoleContent* m_content = nullptr;
   std::unique_ptr<ConsoleWndWin32> m_wnd;
};

} // namespace ccon

#endif // _WIN32
