//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "preferences_tests.h"
#include "preferences.h"
#include "test_util.h"
#include "essentutils/color.h"
#include "essentutils/filesys.h"
#include <filesystem>
#include <fstream>
#include <string>

using namespace ccon;
using namespace std;
using namespace sutil;
namespace fs = std::filesystem;


namespace
{
///////////////////

// Returns the directory that tests can use to load/save data from/to.
fs::path testDataDirectory()
{
   fs::path dataDir = appDirectory();

#ifdef _WIN32 // windows
#ifdef _WIN64 // x64
   // x64 targets have an additional '/x64' subfolder.
   constexpr size_t numSubdirs = 5;
#else
   constexpr size_t numSubdirs = 4;
#endif
#else
   assert("todo");
#endif

   for (size_t i = 0; i < numSubdirs; ++i)
      dataDir = dataDir.parent_path();
   return dataDir / "tests" / "data";
}


void createFile(const fs::path& filePath)
{
   ofstream file(filePath);
}


///////////////////

void testUserPrefsLocation(const fs::path& dataDir)
{
   {
      const string caseLabel = "UserPrefs::location";
      const fs::path filePath = dataDir / "prefs.txt";
      UserPrefs prefs{filePath};
      VERIFY(prefs.location() == filePath, caseLabel);
   }
}


void testUserPrefsSave(const fs::path& dataDir)
{
   {
      const string caseLabel = "UserPrefs::save for existing preferences";
      const fs::path caseDir = dataDir / "user_prefs_save";
      fs::create_directories(caseDir);
      const fs::path filePath = caseDir / "prefs.txt";
      createFile(filePath);

      UserPrefs prefs{filePath};
      prefs.load();
      prefs.setConsoleHeight(100);
      prefs.setConsoleWidth(200);

      const bool saved = prefs.save();
      VERIFY(saved, caseLabel);

      error_code errCode;
      const bool fileExists = fs::exists(filePath, errCode);
      VERIFY(fileExists, caseLabel);

      UserPrefs reloadedPrefs{filePath};
      const bool reloaded = reloadedPrefs.load();
      VERIFY(reloaded, caseLabel);
      VERIFY(reloadedPrefs.consoleHeight() == 100, caseLabel);
      VERIFY(reloadedPrefs.consoleWidth() == 200, caseLabel);

      fs::remove_all(caseDir);
   }
   {
      const string caseLabel = "UserPrefs::save for not existing preferences";
      const fs::path caseDir = dataDir / "user_prefs_save_not_exist";
      fs::create_directories(caseDir);
      const fs::path filePath = caseDir / "prefs.txt";

      UserPrefs prefs{filePath};
      prefs.setConsoleHeight(100);
      prefs.setConsoleWidth(200);

      const bool saved = prefs.save();
      VERIFY(saved, caseLabel);

      error_code errCode;
      const bool fileExists = fs::exists(filePath, errCode);
      VERIFY(fileExists, caseLabel);

      UserPrefs reloadedPrefs{filePath};
      const bool reloaded = reloadedPrefs.load();
      VERIFY(reloaded, caseLabel);
      VERIFY(reloadedPrefs.consoleHeight() == 100, caseLabel);
      VERIFY(reloadedPrefs.consoleWidth() == 200, caseLabel);

      fs::remove_all(caseDir);
   }
}


void testUserPrefsLoad(const fs::path& dataDir)
{
   {
      const string caseLabel = "UserPrefs::load for existing preferences";
      const fs::path caseDir = dataDir / "user_prefs_load";
      const fs::path filePath = caseDir / "prefs.txt";

      UserPrefs prefs{filePath};
      const bool loaded = prefs.load();
      VERIFY(loaded, caseLabel);

      VERIFY(prefs.consoleHeight() == 200, caseLabel);
      VERIFY(prefs.consoleWidth() == 100, caseLabel);
      VERIFY(prefs.fontSize() == 6, caseLabel);
   }
   {
      const string caseLabel = "UserPrefs::load for not existing preferences";
      const fs::path caseDir = dataDir / "user_prefs_load_not_exist";
      const fs::path filePath = caseDir / "prefs.txt";

      UserPrefs prefs{filePath};
      prefs.setFontSize(20);
      const bool loaded = prefs.load();
      VERIFY(!loaded, caseLabel);
      // Previously existing setting gets erased.
      VERIFY(prefs.fontSize() == nullopt, caseLabel);

      fs::remove_all(caseDir);
   }
}


void testUserPrefsRoundtrip(const fs::path& dataDir)
{
   {
      const string caseLabel = "UserPrefs save/load roundtrip";
      const fs::path caseDir = dataDir / "user_prefs_roundtrip";
      fs::create_directories(caseDir);
      const fs::path filePath = caseDir / "prefs.txt";

      UserPrefs prefs{filePath};
      prefs.setConsoleHeight(500);
      prefs.setConsoleWidth(300);
      prefs.setBackgroundColor({10, 20, 30});
      prefs.setTextInputColor({110, 120, 130});
      prefs.setTextOutputColor({210, 220, 230});
      prefs.setFontSize(12);

      const bool saved = prefs.save();
      VERIFY(saved, caseLabel);

      error_code errCode;
      const bool fileExists = fs::exists(filePath, errCode);
      VERIFY(fileExists, caseLabel);

      UserPrefs reloadedPrefs{filePath};
      const bool reloaded = reloadedPrefs.load();
      VERIFY(reloaded, caseLabel);
      VERIFY(reloadedPrefs.consoleHeight() == 500, caseLabel);
      VERIFY(reloadedPrefs.consoleWidth() == 300, caseLabel);
      VERIFY(reloadedPrefs.backgroundColor() == Rgb(10, 20, 30), caseLabel);
      VERIFY(reloadedPrefs.textInputColor() == Rgb(110, 120, 130), caseLabel);
      VERIFY(reloadedPrefs.textOutputColor() == Rgb(210, 220, 230), caseLabel);
      VERIFY(reloadedPrefs.fontSize() == 12, caseLabel);

      fs::remove_all(caseDir);
   }
}

} // namespace


void testPreferences()
{
   const fs::path dataDir = testDataDirectory();
   testUserPrefsLocation(dataDir);
   testUserPrefsSave(dataDir);
   testUserPrefsLoad(dataDir);
   testUserPrefsRoundtrip(dataDir);
}
