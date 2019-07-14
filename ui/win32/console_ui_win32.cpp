//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "console_ui_win32.h"
#include "essentutils/string_util.h"
#include "win32_util/tstring.h"
#include <cassert>

using namespace sutil;
using namespace win32;


namespace ccon
{
///////////////////

ConsoleUIWin32::ConsoleUIWin32(HWND parentWnd, const std::wstring& title,
                               const UserPrefs& prefs)
: m_parentWnd(parentWnd), m_title{title}, m_userPrefs{prefs}
{
}


void ConsoleUIWin32::setContent(ConsoleContent* content)
{
   m_content = content;
}


void ConsoleUIWin32::showConsole()
{
   assert(m_content);

   if (!m_wnd)
      m_wnd = std::make_unique<ConsoleWndWin32>(m_userPrefs, *m_content);
   assert(m_wnd);

   if (!m_wnd->exists())
   {
      constexpr unsigned long wndStyle =
         WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE | WS_VSCROLL;
      constexpr long extWndStyle = WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW;
      m_wnd->create(m_parentWnd, calcConsoleBounds(), convertTo<TString>(m_title),
                    wndStyle, extWndStyle);
   }

   if (!m_wnd->isVisible())
      m_wnd->setVisible(true);
}


void ConsoleUIWin32::hideConsole()
{
   if (m_wnd && m_wnd->isVisible())
      m_wnd->setVisible(false);
}


void ConsoleUIWin32::setBackgroundColor(const sutil::Rgb& color)
{
   if (m_wnd)
      m_wnd->setBackgroundColor(color);
}


void ConsoleUIWin32::setOutputTextColor(const sutil::Rgb& color)
{
   if (m_wnd)
      m_wnd->setOutputTextColor(color);
}


void ConsoleUIWin32::setInputTextColor(const sutil::Rgb& color)
{
   if (m_wnd)
      m_wnd->setInputTextColor(color);
}


void ConsoleUIWin32::resetColors()
{
   if (m_wnd)
      m_wnd->resetColors();
}


void ConsoleUIWin32::setFontSize(int sizeInPoints)
{
   if (m_wnd)
      m_wnd->setFontSize(sizeInPoints);
}


win32::Rect ConsoleUIWin32::calcConsoleBounds() const
{
   win32::Rect bounds = DefaultPrefs::consoleWindowBounds();

   if (const auto width = m_userPrefs.consoleWidth())
      bounds.right = bounds.left + width.value();
   if (const auto height = m_userPrefs.consoleHeight())
      bounds.bottom = bounds.top + height.value();

   return bounds;
}

} // namespace ccon

#endif // _WIN32
