//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "help_cmd.h"


namespace ccon
{
///////////////////

HelpCmd::HelpCmd(const std::set<CmdSpec>& specs) : m_specs(specs)
{
}


CmdOutput HelpCmd::execute(const VerifiedCmd& input)
{
   const std::string indent{"  "};

   CmdOutput out;
   out.push_back("Commands:");
   for (const CmdSpec& spec : m_specs)
      out.push_back(indent + spec.name() + " - " + spec.description());
   return out;
}

} // namespace ccon
