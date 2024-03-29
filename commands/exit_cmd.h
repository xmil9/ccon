//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd.h"
#include "cmd_spec.h"

namespace ccon
{
struct ConsoleUI;
}


namespace ccon
{
///////////////////

namespace exitCmd
{

const std::string cmdName = "exit";

} // namespace exitCmd


inline CmdSpec makeExitCmdSpec()
{
   return {exitCmd::cmdName, "x", "exits the console", {}, ""};
}


///////////////////

class ExitCmd : public Cmd
{
 public:
   ExitCmd() = default;
   explicit ExitCmd(ConsoleUI* ui);
   ~ExitCmd() = default;
   ExitCmd(const ExitCmd&) = default;
   ExitCmd(ExitCmd&&) = default;
   ExitCmd& operator=(const ExitCmd&) = default;
   ExitCmd& operator=(ExitCmd&&) = default;

   CmdOutput execute(const VerifiedCmd& input) override;

private:
   ConsoleUI* m_ui = nullptr;
};

} // namespace ccon
