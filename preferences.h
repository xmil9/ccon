//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "qconfig/config.h"
#include <filesystem>
#include <optional>

namespace sutil
{
struct Rgb;
}
namespace win32
{
struct Rect;
}


namespace ccon
{
///////////////////

// Default settings for the console.
struct DefaultPrefs
{
   static sutil::Rgb backgroundDefaultColor();
   static sutil::Rgb textOutputDefaultColor();
   static sutil::Rgb textInputDefaultColor();
   static win32::Rect consoleWindowBounds();
   static int fontSize();
};


///////////////////

// User preference for the console.
class UserPrefs
{
public:
   bool load(const std::filesystem::path& configPath);
   bool save();

   std::optional<int> consoleWidth() const;
   std::optional<int> consoleHeight() const;
   std::optional<sutil::Rgb> backgroundColor() const;
   std::optional<sutil::Rgb> textOutputColor() const;
   std::optional<sutil::Rgb> textInputColor() const;
   std::optional<int> fontSize() const;
   void setConsoleWidth(int w);
   void setConsoleHeight(int h);
   void setBackgroundColor(const sutil::Rgb& color);
   void setTextOutputColor(const sutil::Rgb& color);
   void setTextInputColor(const sutil::Rgb& color);
   void setFontSize(int sizeInPoints);

private:
   std::filesystem::path m_configPath;
   qcfg::Config m_config;
};


} // namespace ccon
