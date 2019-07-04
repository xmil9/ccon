//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "blackboard.h"
#include <cassert>


namespace
{
///////////////////

const std::string consolePrompt{"> "};

} // namespace


namespace ccon
{
///////////////////

Blackboard::Blackboard()
{
   startNewInputLine();
}


std::size_t Blackboard::countLines() const
{
   return m_content.size();
}


std::string Blackboard::lineText(size_t lineIdx) const
{
   if (0 <= lineIdx && lineIdx < countLines())
      return m_content[lineIdx].text;
   return {};
}


bool Blackboard::isEnteredLine(std::size_t lineIdx) const
{
   if (0 <= lineIdx && lineIdx < countLines())
      return (m_content[lineIdx].source == Line::Source::Entered);
   return false;
}


void Blackboard::appendLine(const std::string& text)
{
   m_content.push_back({text, Line::Source::Output});
}


std::string Blackboard::inputLineText() const
{
   assert(!m_content.empty());
   return m_content[m_content.size() - 1].text;
}


std::string Blackboard::enteredInputText() const
{
   // Strip off prompt text.
   return inputLineText().substr(consolePrompt.size());
}


void Blackboard::setInputLine(const std::string& text)
{
   m_content[m_content.size() - 1] = {text, Line::Source::Entered};
}


void Blackboard::setEnteredInputText(const std::string& text)
{
   setInputLine(consolePrompt + text);
}


void Blackboard::startNewInputLine()
{
   m_content.push_back({consolePrompt, Line::Source::Entered});
}


void Blackboard::commitInputLine()
{
   m_history.push_back(inputLineText());
   m_historyIdx = m_history.size() - 1;
}


std::size_t Blackboard::promptLength() const
{
   return consolePrompt.size();
}


void Blackboard::goToPreviousInput()
{
   if (m_history.empty())
      return;

   setInputLine(m_history[m_historyIdx]);

   if (m_historyIdx > 0)
      --m_historyIdx;
}


void Blackboard::goToNextInput()
{
   if (m_history.empty())
      return;

   if (m_historyIdx < m_history.size() - 1)
      setInputLine(m_history[++m_historyIdx]);
   else
      setInputLine(consolePrompt);
}

} // namespace ccon
