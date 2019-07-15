//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "font_size_cmd.h"
#include "console_ui.h"
#include "preferences.h"
#include "essentutils/string_util.h"
#include <cassert>
#include <tuple>
#include <utility>


namespace
{
///////////////////

int interpretArgs(const ccon::VerifiedArgs& args)
{
   return sutil::intFromStr(args[0].values[0], 0);
}

} // namespace


namespace ccon
{
///////////////////

ConsoleFontSizeCmd::ConsoleFontSizeCmd(ConsoleUI* ui)
   : m_ui{ui}
{
}


CmdOutput ConsoleFontSizeCmd::execute(const VerifiedCmd& input)
{
   assert(input.name == fontSizeCmd::cmdName);

   m_fontSize = interpretArgs(input.args);

   if (const auto [canExec, failReason] = canExecute(); !canExec)
      return {failReason};

   if (m_fontSize == 0)
      m_fontSize = DefaultPrefs::fontSize();

   if (m_ui)
      m_ui->setFontSize(m_fontSize);

   return {};
}


std::pair<bool, std::string> ConsoleFontSizeCmd::canExecute() const
{
   if (m_fontSize < 0)
      return {false, "Command arguments: Font size has to be positive."};
   return {true, {}};
}

} // namespace ccon
