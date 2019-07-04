//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <cstddef>
#include <string>

namespace ccon
{
struct ConsoleUI;
}


namespace ccon
{
///////////////////

// Abstracts the console's content.
struct ConsoleContent
{
   virtual ~ConsoleContent() = default;

   virtual void setUI(ConsoleUI* ui) = 0;
   // The input line is the last of the console lines but can also be accessed
   // individually through InputLine().
   virtual std::size_t countLines() const = 0;
   virtual std::string lineText(std::size_t lineIdx) const = 0;
   virtual bool isEnteredLine(std::size_t lineIdx) const = 0;
   // Minimal position (as zero-based character index) of cursor on the input line.
   // The UI uses this position to make sure the cursor cannot be moved into the
   // console prompt.
   virtual std::size_t minInputCursorPosition() const = 0;
   // Text of the input line (including the console prompt).
   virtual std::string inputLineText() const = 0;
   virtual void setInputLine(const std::string& text) = 0;
   virtual void processInputLine() = 0;
   virtual void goToPreviousInput() = 0;
   virtual void goToNextInput() = 0;
   virtual void nextAutoCompletion() = 0;
};

} // namespace ccon
