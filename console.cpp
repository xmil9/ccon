//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "console.h"
#include "cmd_parser.h"
#include "commands/colors_cmd.h"
#include "commands/exit_cmd.h"
#include "commands/font_size_cmd.h"
#include "commands/help_cmd.h"
#include "console_ui.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <cassert>
#include <iterator>


namespace
{
///////////////////

ccon::CmdOutput splitAtNewlines(const ccon::CmdOutput& src)
{
   ccon::CmdOutput out;
   for (const std::string& line : src)
   {
      const std::vector<std::string> pieces = sutil::split(line, "\n");
      std::copy(pieces.begin(), pieces.end(), std::back_inserter(out));
   }

   return out;
}

} // namespace


namespace ccon
{
///////////////////

Console::Console(ConsoleUI& ui, const std::string& prompt)
: m_ui{ui}, m_blackboard{prompt}
{
   m_ui.setContent(this);
   initCommands();
   m_autoCompletion.setCmds(m_cmds.availableCommands());
}


void Console::addCommand(const CmdSpec& spec, CmdFactoryFn factoryFn)
{
   m_cmds.addCommand(spec, factoryFn);
}


std::size_t Console::countLines() const
{
   return m_blackboard.countLines();
}


std::string Console::lineText(size_t lineIdx) const
{
   return m_blackboard.lineText(lineIdx);
}


bool Console::isEnteredLine(std::size_t lineIdx) const
{
   return m_blackboard.isEnteredLine(lineIdx);
}


std::size_t Console::minInputCursorPosition() const
{
   return m_blackboard.promptLength();
}


std::string Console::inputLineText() const
{
   return m_blackboard.inputLineText();
}


void Console::setInputLine(const std::string& text)
{
   m_blackboard.setInputLine(text);
   m_autoCompletion.reset();
}


void Console::processInputLine()
{
   m_blackboard.commitInputLine();

   const CmdOutput output = processRawInput(m_blackboard.enteredInputText());
   for (const std::string& outLine : output)
      m_blackboard.appendLine(outLine);

   // Put empty line between output and next input line to create a visual
   // break.
   if (!output.empty())
      m_blackboard.appendLine("\n");

   m_blackboard.startNewInputLine();
   m_autoCompletion.reset();
}


void Console::goToPreviousInput()
{
   m_blackboard.goToPreviousInput();
   m_autoCompletion.reset();
}


void Console::goToNextInput()
{
   m_blackboard.goToNextInput();
   m_autoCompletion.reset();
}


void Console::nextAutoCompletion()
{
   const std::string completion = m_autoCompletion.next(m_blackboard.enteredInputText());
   if (!completion.empty())
      m_blackboard.setEnteredInputText(completion);
}


void Console::initCommands()
{
   m_cmds.addCommand(makeConsoleColorsCmdSpec(),
                     [this]() { return std::make_unique<ConsoleColorsCmd>(&m_ui); });
   m_cmds.addCommand(makeConsoleFontSizeCmdSpec(),
                     [this]() { return std::make_unique<ConsoleFontSizeCmd>(&m_ui); });
   m_cmds.addCommand(makeExitCmdSpec(),
                     [this]() { return std::make_unique<ExitCmd>(&m_ui); });
   m_cmds.addCommand(makeHelpCmdSpec(), [this]() {
      return std::make_unique<HelpCmd>(m_cmds.availableCommands());
   });
}


CmdOutput Console::processRawInput(const std::string& rawInput) const
{
   // No input. Output nothing.
   if (rawInput.empty())
      return {};

   CmdSpec::Match cmdMatch;
   for (const CmdSpec& spec : m_cmds.availableCommands())
   {
      cmdMatch = spec.match(rawInput);
      if (cmdMatch.isMatching)
         break;
   }

   if (cmdMatch.isMatching && cmdMatch.areArgsValid)
      return executeCommand(cmdMatch.matchedCmd);
   else if (cmdMatch.isMatching && !cmdMatch.areArgsValid)
      return {"Command syntax error."};
   return {"Command not found."};
}


CmdOutput Console::executeCommand(const VerifiedCmd& cmdInput) const
{
   if (containsHelpParameter(cmdInput.args))
      return m_cmds.getCommandHelp(cmdInput.name);

   auto cmd = m_cmds.makeCommand(cmdInput.name);
   if (cmd)
      return splitAtNewlines(cmd->execute(cmdInput));

   assert(false && "Command was already validated against a spec. We should be able to "
                   "instantiate it!");
   return {"Internal error. Failed to instantiate command."};
}

} // namespace ccon
