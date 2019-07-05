//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "console.h"
#include "cmd_parser.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <cassert>
#include <iterator>

using namespace ccon;
using namespace std;
using namespace sutil;


namespace
{
///////////////////

CmdOutput splitAtNewlines(const CmdOutput& src)
{
   CmdOutput out;
   for (const string& line : src)
   {
      const vector<string> pieces = split(line, "\n");
      copy(pieces.begin(), pieces.end(), back_inserter(out));
   }

   return out;
}

} // namespace


namespace ccon
{
///////////////////

Console::Console()
{
   m_autoCompletion.setCmds(m_cmds.availableCommands());
}


void Console::setUI(ConsoleUI* ui)
{
   m_ui = ui;
   m_cmds.setUI(ui);
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


CmdOutput Console::processRawInput(const std::string& rawInput) const
{
   // No input. Output nothing.
   if (rawInput.empty())
      return {};

   CmdParsingResult res;
   for (const CmdSpec& spec : m_cmds.availableCommands())
   {
      res = parseCmd(rawInput, spec);
      if (res.foundCommand)
         break;
   }

   if (res.foundCommand && res.areArgsValid)
      return executeCommand(res.cmdInput);
   else if (res.foundCommand && !res.areArgsValid)
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


///////////////////

std::unique_ptr<Console> makeConsole()
{
   return make_unique<Console>();
}

} // namespace ccon
