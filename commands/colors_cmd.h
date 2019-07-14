//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd.h"
#include "cmd_spec.h"
#include "essentutils/color.h"
#include <optional>

namespace ccon
{
struct ConsoleUI;
}


namespace ccon
{
///////////////////

namespace consoleColorsCmdSpec
{

const std::string cmdName = ":colors";
const std::string backgroundOption = "background";
const std::string outputTextOption = "output";
const std::string inputTextOption = "input";
const std::string defaultsOption = "defaults";

} // namespace consoleColorsCmdSpec


inline CmdSpec makeConsoleColorsCmdSpec()
{
   return {consoleColorsCmdSpec::cmdName,
           ":c",
           "sets the colors for the console",
           {
              ArgSpec::makeOptionalArg(consoleColorsCmdSpec::backgroundOption, 1, "b",
                                       "background color (rrggbb)"),
              ArgSpec::makeOptionalArg(consoleColorsCmdSpec::outputTextOption, 1, "o",
                                       "output text color (rrggbb)"),
              ArgSpec::makeOptionalArg(consoleColorsCmdSpec::inputTextOption, 1, "i",
                                       "input text color (rrggbb)"),
              ArgSpec::makeFlagArg(consoleColorsCmdSpec::defaultsOption, "d",
                                   "reset all colors to defaults"),
           },
           ""};
}


///////////////////

class ConsoleColorsCmd : public Cmd
{
 public:
   ConsoleColorsCmd() = default;
   explicit ConsoleColorsCmd(ConsoleUI* ui);
   ~ConsoleColorsCmd() = default;
   ConsoleColorsCmd(const ConsoleColorsCmd&) = default;
   ConsoleColorsCmd(ConsoleColorsCmd&&) = default;
   ConsoleColorsCmd& operator=(const ConsoleColorsCmd&) = default;
   ConsoleColorsCmd& operator=(ConsoleColorsCmd&&) = default;

   CmdOutput execute(const VerifiedCmd& input) override;

 private:
   struct Options
   {
      std::optional<sutil::Rgb> background;
      std::optional<sutil::Rgb> outputText;
      std::optional<sutil::Rgb> inputText;
      bool defaults = false;
   };

private:
   static Options interpretArgs(const VerifiedArgs& args);

 private:
   ConsoleUI* m_ui = nullptr;
   Options m_opt;
};

} // namespace ccon
