//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "preferences.h"
#include "essentutils/color.h"
#include "qconfig/file_storage.h"
#include "win32_util/geometry.h"
#include <string>

using namespace qcfg;
using namespace std;
using namespace sutil;
using namespace win32;


namespace
{
///////////////////

const string ConsoleWidthKey = "ConsoleWidth";
const string ConsoleHeightKey = "ConsoleHeight";
const string BackgroundColorKey = "BackgroundColor";
const string TextOutputColorKey = "TextOutputColor";
const string TextInputColorKey = "TextInputColor";
const string FontSizeKey = "FontSize";


///////////////////

const string RedTag = "_R";
const string GreenTag = "_G";
const string BlueTag = "_B";


void storeColor(Config& config, const string& colorKey, const Rgb& color)
{
   config.setInt8(colorKey + RedTag, color.r);
   config.setInt8(colorKey + GreenTag, color.g);
   config.setInt8(colorKey + BlueTag, color.b);
}


optional<Rgb> fetchColor(const Config& config, const string& colorKey)
{
   const auto red = config.getInt8(colorKey + RedTag);
   if (!red.has_value())
      return {};
   const auto green = config.getInt8(colorKey + GreenTag);
   if (!green.has_value())
      return {};
   const auto blue = config.getInt8(colorKey + BlueTag);
   if (!blue.has_value())
      return {};
   return Rgb(red.value(), green.value(), blue.value());
}

} // namespace


namespace ccon
{
///////////////////

sutil::Rgb DefaultPrefs::backgroundDefaultColor()
{
   return Rgb(26, 26, 26);
}


sutil::Rgb DefaultPrefs::textOutputDefaultColor()
{
   return Rgb(230, 230, 230);
}


sutil::Rgb DefaultPrefs::textInputDefaultColor()
{
   return Rgb(255, 211, 149);
}


int DefaultPrefs::fontSize()
{
   return 11;
}


win32::Rect DefaultPrefs::consoleWindowBounds()
{
   return Rect{100, 100, 750, 600};
}


///////////////////

UserPrefs::UserPrefs(const std::filesystem::path& configPath)
   : m_configPath{configPath}
{
}


bool UserPrefs::load()
{
   if (m_configPath.empty())
      return false;

   FileStorage configStorage{m_configPath};
   if (!m_config.load(configStorage))
      return false;

   return true;
}


bool UserPrefs::save()
{
   if (m_configPath.empty())
      return false;

   FileStorage configStorage{m_configPath};
   return m_config.save(configStorage);
}


std::optional<int> UserPrefs::consoleWidth() const
{
   return m_config.getInt32(ConsoleWidthKey);
}


std::optional<int> UserPrefs::consoleHeight() const
{
   return m_config.getInt32(ConsoleHeightKey);
}


std::optional<sutil::Rgb> UserPrefs::backgroundColor() const
{
   return fetchColor(m_config, BackgroundColorKey);
}


std::optional<sutil::Rgb> UserPrefs::textOutputColor() const
{
   return fetchColor(m_config, TextOutputColorKey);
}


std::optional<sutil::Rgb> UserPrefs::textInputColor() const
{
   return fetchColor(m_config, TextInputColorKey);
}


std::optional<int> UserPrefs::fontSize() const
{
   return m_config.getInt32(FontSizeKey);
}


void UserPrefs::setConsoleWidth(int w)
{
   m_config.setInt32(ConsoleWidthKey, w);
}


void UserPrefs::setConsoleHeight(int h)
{
   m_config.setInt32(ConsoleHeightKey, h);
}


void UserPrefs::setBackgroundColor(const sutil::Rgb& color)
{
   storeColor(m_config, BackgroundColorKey, color);
}


void UserPrefs::setTextOutputColor(const sutil::Rgb& color)
{
   {
      storeColor(m_config, TextOutputColorKey, color);
   }
}


void UserPrefs::setTextInputColor(const sutil::Rgb& color)
{
   storeColor(m_config, TextInputColorKey, color);
}


void UserPrefs::setFontSize(int sizeInPoints)
{
   m_config.setInt32(FontSizeKey, sizeInPoints);
}

} // namespace ccon
