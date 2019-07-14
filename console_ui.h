//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <memory>

namespace ccon
{
struct ConsoleContent;
}
namespace sutil
{
struct Rgb;
}


namespace ccon
{
///////////////////

// Abstracts the console's UI.
struct ConsoleUI
{
   virtual ~ConsoleUI() = default;

   virtual void setContent(ConsoleContent* content) = 0;
   virtual void showConsole() = 0;
   virtual void hideConsole() = 0;
   virtual void setBackgroundColor(const sutil::Rgb& color) = 0;
   virtual void setOutputTextColor(const sutil::Rgb& color) = 0;
   virtual void setInputTextColor(const sutil::Rgb& color) = 0;
   virtual void resetColors() = 0;
   virtual void setFontSize(int sizeInPoints) = 0;
};

} // namespace ccon
