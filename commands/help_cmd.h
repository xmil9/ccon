//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd.h"
#include "cmd_spec.h"
#include <set>
#include <string>


namespace ccon
{
///////////////////

namespace helpCmdSpec
{

const std::string cmdName = "help";

} // namespace helpCmdSpec


inline CmdSpec makeHelpCmdSpec()
{
   return {helpCmdSpec::cmdName, "?", "lists all commands", {}, ""};
}


///////////////////

class HelpCmd : public Cmd
{
 public:
   HelpCmd() = default;
   explicit HelpCmd(const std::set<CmdSpec>& specs);
   ~HelpCmd() = default;
   HelpCmd(const HelpCmd&) = default;
   HelpCmd(HelpCmd&&) = default;
   HelpCmd& operator=(const HelpCmd&) = default;
   HelpCmd& operator=(HelpCmd&&) = default;

   CmdOutput execute(const VerifiedCmd& input) override;

 private:
   std::set<CmdSpec> m_specs;
};

} // namespace ccon
