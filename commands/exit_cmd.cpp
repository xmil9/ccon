//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "exit_cmd.h"
#include "console_ui.h"


namespace ccon
{
///////////////////

ExitCmd::ExitCmd(ConsoleUI* ui)
   : m_ui{ui}
{
}


CmdOutput ExitCmd::execute(const VerifiedCmd& /*input*/)
{
   if (m_ui)
      m_ui->hideConsole();
   return {};
}

} // namespace ccon
