//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <cstddef>
#include <string>
#include <vector>


namespace ccon
{
///////////////////

// Keeps track of the content that is displayed in the console.
// The content lines are logical lines, i.e. lines wrapping caused by the console's
// width is not considered.
class Blackboard
{
 public:
   explicit Blackboard(const std::string& prompt);
   ~Blackboard() = default;
   Blackboard(const Blackboard&) = default;
   Blackboard(Blackboard&&) = default;
   Blackboard& operator=(const Blackboard&) = default;
   Blackboard& operator=(Blackboard&&) = default;

   std::size_t promptLength() const;
   std::size_t countLines() const;
   std::string lineText(std::size_t lineIdx) const;
   bool isEnteredLine(std::size_t lineIdx) const;
   void appendLine(const std::string& text);
   // Returns the entire text of the input line.
   std::string inputLineText() const;
   // Returns only the entered text of the input line.
   std::string enteredInputText() const;
   void setInputLine(const std::string& text);
   void setEnteredInputText(const std::string& text);
   void startNewInputLine();
   void commitInputLine();
   void goToPreviousInput();
   void goToNextInput();

 private:
   // Information about a (logical) console line.
   struct Line
   {
      enum class Source
      {
         Entered,
         Output
      };

      std::string text;
      Source source = Source::Entered;
   };

 private:
   std::string m_prompt;
   std::vector<Line> m_content;
   std::vector<std::string> m_history;
   // Holds the index of the history element that is previous to the currently displayed
   // input.
   std::size_t m_historyIdx = 0;
};

} // namespace ccon
