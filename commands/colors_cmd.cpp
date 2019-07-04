//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "colors_cmd.h"
#include "cmd_parser.h"
#include "console_ui.h"
#include <cassert>

using namespace ccon;
using namespace ccon::consoleColorsCmdSpec;
using namespace std;


namespace
{
///////////////////

ConsoleColorsCmd::Options interpretArgs(const VerifiedArgs& args)
{
   auto optArgs = args.begin();
   auto optArgsEnd = args.end();

   ConsoleColorsCmd::Options opt;
   if (haveArgWithLabel(optArgs, optArgsEnd, backgroundOption))
      opt.background = parseColorArg(optArgs, optArgsEnd, backgroundOption, {});
   if (haveArgWithLabel(optArgs, optArgsEnd, outputTextOption))
      opt.outputText = parseColorArg(optArgs, optArgsEnd, outputTextOption, {});
   if (haveArgWithLabel(optArgs, optArgsEnd, inputTextOption))
      opt.inputText = parseColorArg(optArgs, optArgsEnd, inputTextOption, {});
   opt.defaults = haveArgWithLabel(optArgs, optArgsEnd, defaultsOption);

   return opt;
}

} // namespace


namespace ccon
{
///////////////////

ConsoleColorsCmd::ConsoleColorsCmd(ConsoleUI* ui)
   : m_ui{ui}
{
}



CmdOutput ConsoleColorsCmd::execute(const VerifiedCmd& input)
{
   assert(input.name == cmdName);

   m_opt = interpretArgs(input.args);

   if (m_opt.background.has_value() && m_ui)
      m_ui->setBackgroundColor(m_opt.background.value());
   if (m_opt.outputText.has_value() && m_ui)
      m_ui->setOutputTextColor(m_opt.outputText.value());
   if (m_opt.inputText.has_value() && m_ui)
      m_ui->setInputTextColor(m_opt.inputText.value());
   if (m_opt.defaults && m_ui)
      m_ui->resetColors();

   return {};
}

} // namespace ccon
