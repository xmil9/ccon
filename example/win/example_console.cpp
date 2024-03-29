//
// ccon example
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "greet_cmd.h"
#include "resource.h"
#include "targetver.h"
#include "ccon/console.h"
#include "ccon/preferences.h"
#include "ccon/ui/win32/console_ui_win32.h"
#include "essentutils/filesys.h"
#include "win32_util/message_util.h"
#include "win32_util/window.h"
#include <tchar.h>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>

namespace fs = std::filesystem;


namespace
{
///////////////////

class AppWindow : public win32::Window
{
 public:
   AppWindow();

 protected:
   const TCHAR* windowClassName() const override;
   bool registerWindowClass() const override;

   CreationResult onCreate(const CREATESTRUCT* createInfo) override;
   bool onDestroy() override;
   bool onCommand(int id, UINT notificationCode, HWND ctrlWnd) override;

 private:
   bool createPreferences(const fs::path& prefsPath);
   bool loadPreferences();
   void setupConsole();
   void showConsole();

 private:
   ccon::UserPrefs m_prefs;
   std::unique_ptr<ccon::ConsoleUIWin32> m_consoleUi;
   std::unique_ptr<ccon::Console> m_console;
};


AppWindow::AppWindow() : m_prefs{sutil::userDirectory() / "ExampleConsole" / "prefs.txt"}
{
}


const TCHAR* AppWindow::windowClassName() const
{
   return _T("ExampleConsoleAppWnd");
}


bool AppWindow::registerWindowClass() const
{
   if (isWindowClassRegistered())
      return true;

   const HINSTANCE appInst = appInstance();

   WNDCLASSEX classInfo;
   std::memset(&classInfo, 0, sizeof(WNDCLASSEX));
   classInfo.cbSize = sizeof(WNDCLASSEX);
   classInfo.hInstance = appInst;
   classInfo.lpszClassName = windowClassName();
   classInfo.lpfnWndProc = windowProc;
   classInfo.style = CS_HREDRAW | CS_VREDRAW;
   classInfo.cbClsExtra = 0;
   classInfo.cbWndExtra = 0;
   classInfo.hIcon = LoadIcon(appInst, MAKEINTRESOURCE(IDI_EXAMPLECONSOLE));
   classInfo.hIconSm = LoadIcon(appInst, MAKEINTRESOURCE(IDI_SMALL));
   classInfo.hCursor = ::LoadCursor(NULL, IDC_ARROW);
   // Either a HBRUSH for the background, a std system color value + 1, or
   // NULL.
   // The '+ 1' makes sure COLOR_SCROLLBAR and NULL can be distinguished!
   // The system deletes the background brush when the class is unregistered.
   classInfo.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   classInfo.lpszMenuName = MAKEINTRESOURCEW(IDC_EXAMPLECONSOLE);

   const ATOM token = ::RegisterClassEx(&classInfo);
   assert(token != 0);
   return (token != 0);
}


win32::Window::CreationResult AppWindow::onCreate(const CREATESTRUCT* createInfo)
{
   loadPreferences();
   setupConsole();
   return CreationResult::Handled;
}


bool AppWindow::onDestroy()
{
   PostQuitMessage(0);
   return true;
}


bool AppWindow::onCommand(int id, UINT notificationCode, HWND ctrlWnd)
{
   switch (id)
   {
   case ID_VIEW_CONSOLE:
      showConsole();
      return true;
   case IDM_EXIT:
      close();
      return true;
   }
   return false;
}


bool AppWindow::createPreferences(const fs::path& prefsPath)
{
   fs::create_directories(prefsPath.parent_path());
   // Create empty file.
   std::ofstream prefsFile(prefsPath, std::fstream::out);
   return !prefsFile.fail();
}


bool AppWindow::loadPreferences()
{
   std::error_code errCode;
   if (!fs::exists(m_prefs.location(), errCode) && !createPreferences(m_prefs.location()))
      return false;

   return m_prefs.load();
}


void AppWindow::setupConsole()
{
   m_consoleUi =
      std::make_unique<ccon::ConsoleUIWin32>(hwnd(), L"Example Console", m_prefs);
   m_console = std::make_unique<ccon::Console>(*m_consoleUi.get());
   m_console->addCommand(makeGreetCmdSpec(),
                         []() { return std::make_unique<GreetCmd>(); });
}


void AppWindow::showConsole()
{
   m_consoleUi->showConsole();
}

} // namespace


int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance,
                      _In_ LPWSTR cmdLine, _In_ int cmdShow)
{
   AppWindow appWnd;
   if (!appWnd.create(NULL, win32::Rect{100, 100, 1000, 500}, L"Example Console",
                      WS_OVERLAPPEDWINDOW))
   {
      return EXIT_FAILURE;
   }
   appWnd.setVisible(true);

   return win32::mainMessageLoop(
             LoadAccelerators(instance, MAKEINTRESOURCE(IDC_EXAMPLECONSOLE)))
      .exitCode;
}
