//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd.h"
#include "cmd_spec.h"
#include <utility>

namespace ccon
{
struct ConsoleUI;
}


namespace ccon
{
///////////////////

namespace fontSizeCmd
{

const std::string cmdName = ":fontsize";

} // namespace fontSizeCmd


inline CmdSpec makeConsoleFontSizeCmdSpec()
{
   return {fontSizeCmd::cmdName,
           ":fs",
           "sets the font size for the console",
           {
              ArgSpec::makePositionalArg(
                 1, "font size in points; use 0 to reset to default size"),
           },
           ""};
}


///////////////////

class ConsoleFontSizeCmd : public Cmd
{
 public:
   ConsoleFontSizeCmd() = default;
   explicit ConsoleFontSizeCmd(ConsoleUI* ui);
   ~ConsoleFontSizeCmd() = default;
   ConsoleFontSizeCmd(const ConsoleFontSizeCmd&) = default;
   ConsoleFontSizeCmd(ConsoleFontSizeCmd&&) = default;
   ConsoleFontSizeCmd& operator=(const ConsoleFontSizeCmd&) = default;
   ConsoleFontSizeCmd& operator=(ConsoleFontSizeCmd&&) = default;

   CmdOutput execute(const VerifiedCmd& input) override;

 private:
   std::pair<bool, std::string> canExecute() const;

 private:
   ConsoleUI* m_ui = nullptr;
   int m_fontSize = 0;
};

} // namespace ccon
