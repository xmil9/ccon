//
// ccon example
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "greet_cmd.h"
#include <cassert>
#include <string>

using namespace ccon;
using namespace greetCmdSpec;
using namespace std;


namespace
{

///////////////////

string interpretArgs(const VerifiedArgs& args)
{
   return args[0].values[0];
}

} // namespace


CmdOutput GreetCmd::execute(const VerifiedCmd& input)
{
   assert(input.name == cmdName);

   m_userName = interpretArgs(input.args);

   if (m_userName.empty())
      m_userName = "Looser";

   return {string("Hello ") + m_userName + "!"};
}
