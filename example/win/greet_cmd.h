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

namespace greetCmdSpec
{

const std::string cmdName = "greet";

} // namespace greetCmdSpec


inline ccon::CmdSpec makeGreetCmdSpec()
{
   return {greetCmdSpec::cmdName,
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
