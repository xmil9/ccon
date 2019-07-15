//
// ccon example
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "greet_cmd.h"
#include <cassert>
#include <string>


namespace
{

///////////////////

std::string interpretArgs(const ccon::VerifiedArgs& args)
{
   return args[0].values[0];
}

} // namespace


ccon::CmdOutput GreetCmd::execute(const ccon::VerifiedCmd& input)
{
   assert(input.name == greetCmd::cmdName);

   m_userName = interpretArgs(input.args);

   if (m_userName.empty())
      m_userName = "Looser";

   return {std::string("Hello ") + m_userName + "!"};
}
