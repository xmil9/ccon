//
// ccon example
//
// Jul-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd.h"
#include "cmd_spec.h"
#include <utility>


///////////////////

namespace greetCmd
{

const std::string cmdName = "greet";

} // namespace greetCmd


inline ccon::CmdSpec makeGreetCmdSpec()
{
   return {greetCmd::cmdName,
           "g",
           "shows greeting",
           {
              ccon::ArgSpec::makePositionalArg(1, "name of user"),
           },
           ""};
}


///////////////////

class GreetCmd : public ccon::Cmd
{
 public:
   ccon::CmdOutput execute(const ccon::VerifiedCmd& input) override;

 private:
   std::string m_userName;
};
