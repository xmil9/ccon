//
// ccon example
//
// Jul-2019, Michael Lindner
// MIT license
//
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

using namespace ccon;
using namespace std;
using namespace sutil;
using namespace win32;
namespace fs = std::filesystem;


namespace
{
///////////////////

class AppWindow : public Window
{
 public:
 protected:
   const TCHAR* windowClassName() const override;
   bool registerWindowClass() const override;

   CreationResult onCreate(const CREATESTRUCT* createInfo) override;
   bool onDestroy() override;
   bool onCommand(int id, UINT notificationCode, HWND ctrlWnd) override;

 private:
   bool createPreferences(const fs::path& prefsPath);
   bool loadPreferences();
   void showConsole();
   void makeConsoleUi();

 private:
   UserPrefs m_prefs;
   Console m_console;
   unique_ptr<ConsoleUIWin32> m_consoleUi;
};


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


Window::CreationResult AppWindow::onCreate(const CREATESTRUCT* createInfo)
{
   loadPreferences();
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
   ofstream prefsFile(prefsPath, fstream::out);
   return !prefsFile.fail();
}


bool AppWindow::loadPreferences()
{
   const fs::path prefsPath = userDirectory() / "ExampleConsole" / "prefs.txt";

   error_code errCode;
   if (!fs::exists(prefsPath, errCode) && !createPreferences(prefsPath))
      return false;

   return m_prefs.load(prefsPath);
}


void AppWindow::showConsole()
{
   if (!m_consoleUi)
      makeConsoleUi();
   m_consoleUi->showConsole();
}


void AppWindow::makeConsoleUi()
{
   m_consoleUi =
      make_unique<ConsoleUIWin32>(hwnd(), L"Example Console", m_prefs, m_console);
   m_console.setUI(m_consoleUi.get());
}

} // namespace


int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance,
                      _In_ LPWSTR cmdLine, _In_ int cmdShow)
{
   AppWindow appWnd;
   if (!appWnd.create(NULL, Rect{100, 100, 1000, 500}, L"Example Console",
                      WS_OVERLAPPEDWINDOW))
   {
      return EXIT_FAILURE;
   }
   appWnd.setVisible(true);

   return mainMessageLoop(LoadAccelerators(instance, MAKEINTRESOURCE(IDC_EXAMPLECONSOLE)))
      .exitCode;
}
