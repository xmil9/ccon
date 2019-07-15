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


namespace ccon
{
///////////////////

ConsoleColorsCmd::ConsoleColorsCmd(ConsoleUI* ui) : m_ui{ui}
{
}


CmdOutput ConsoleColorsCmd::execute(const VerifiedCmd& input)
{
   assert(input.name == colorsCmd::cmdName);

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


ConsoleColorsCmd::Options ConsoleColorsCmd::interpretArgs(const VerifiedArgs& args)
{
   auto optArgs = args.begin();
   auto optArgsEnd = args.end();

   ConsoleColorsCmd::Options opt;
   if (haveArgWithLabel(optArgs, optArgsEnd, colorsCmd::backgroundOption))
      opt.background =
         parseColorArg(optArgs, optArgsEnd, colorsCmd::backgroundOption, {});
   if (haveArgWithLabel(optArgs, optArgsEnd, colorsCmd::outputTextOption))
      opt.outputText =
         parseColorArg(optArgs, optArgsEnd, colorsCmd::outputTextOption, {});
   if (haveArgWithLabel(optArgs, optArgsEnd, colorsCmd::inputTextOption))
      opt.inputText = parseColorArg(optArgs, optArgsEnd, colorsCmd::inputTextOption, {});
   opt.defaults = haveArgWithLabel(optArgs, optArgsEnd, colorsCmd::defaultsOption);

   return opt;
}

} // namespace ccon
