//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "cmd_parser.h"
#include "console_util.h"
#include "essentutils/color.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <cassert>
#include <map>

using namespace ccon;
using namespace std;
using namespace sutil;


namespace
{
///////////////////

pair<bool, VerifiedArg> matchArgSpec(const ArgSpec& spec,
                                     CmdArgs::const_iterator& actualArgs,
                                     CmdArgs::const_iterator actualArgsEnd)
{
   CmdArgs::const_iterator iterCopy = actualArgs;

   bool haveMatch = false;
   VerifiedArg match;
   tie(haveMatch, match) = spec.match(iterCopy, actualArgsEnd);
   if (!haveMatch)
      return {false, {}};

   // Only advance the iterator over the actual arguments if we found a match.
   actualArgs = iterCopy;
   return {haveMatch, match};
}


pair<bool, VerifiedArgs> parsePositionalCmdArgs(CmdSpec::ArgSpecIter_t posSpec,
                                                CmdSpec::ArgSpecIter_t posSpecEnd,
                                                CmdArgs::const_iterator& actualArgs,
                                                CmdArgs::const_iterator actualArgsEnd)
{
   VerifiedArgs verifiedArgs;

   for (; posSpec != posSpecEnd; ++posSpec)
   {
      bool haveMatch = false;
      VerifiedArg matchedArg;
      tie(haveMatch, matchedArg) = matchArgSpec(*posSpec, actualArgs, actualArgsEnd);

      if (haveMatch)
         verifiedArgs.push_back(matchedArg);
      else if (!haveMatch)
         return {false, {}};
   }

   return {true, verifiedArgs};
}


pair<bool, VerifiedArgs> parseOptionalCmdArgs(CmdSpec::ArgSpecIter_t optSpecBegin,
                                              CmdSpec::ArgSpecIter_t optSpecEnd,
                                              CmdArgs::const_iterator& actualArgs,
                                              CmdArgs::const_iterator actualArgsEnd)
{
   VerifiedArgs verifiedArgs;

   while (actualArgs != actualArgsEnd)
   {
      bool haveMatch = false;

      for (CmdSpec::ArgSpecIter_t optSpec = optSpecBegin; optSpec != optSpecEnd;
           ++optSpec)
      {
         VerifiedArg matchedArg;
         tie(haveMatch, matchedArg) = matchArgSpec(*optSpec, actualArgs, actualArgsEnd);

         if (haveMatch)
         {
            verifiedArgs.push_back(matchedArg);
            break;
         }
      }

      if (!haveMatch)
         return {false, {}};
   }

   return {true, verifiedArgs};
}


pair<bool, VerifiedArgs> parseCmdArgs(const CmdArgs& args, const CmdSpec& spec)
{
   if (containsHelpParameter(args))
      return {true, {VerifiedArg{HelpArgSpec.label()}}};

   auto posSpecs = spec.firstPositionalArgument();
   auto posSpecsEnd = spec.firstOptionalArgument();
   auto optSpecs = posSpecsEnd;
   auto optSpecsEnd = spec.end();

   auto actualArgs = begin(args);
   auto actualArgsEnd = end(args);

   bool argsMatch = false;
   VerifiedArgs verifiedArgs;
   tie(argsMatch, verifiedArgs) =
      parsePositionalCmdArgs(posSpecs, posSpecsEnd, actualArgs, actualArgsEnd);
   if (!argsMatch)
      return {false, {}};

   VerifiedArgs verifieOptArgs;
   tie(argsMatch, verifieOptArgs) =
      parseOptionalCmdArgs(optSpecs, optSpecsEnd, actualArgs, actualArgsEnd);
   if (!argsMatch)
      return {false, {}};

   const bool haveUnmacthedArgs = (actualArgs != end(args));
   if (haveUnmacthedArgs)
      return {false, {}};

   copy(verifieOptArgs.begin(), verifieOptArgs.end(), back_inserter(verifiedArgs));
   return {true, verifiedArgs};
}


///////////////////

unsigned int intFromHexDigit(char hexChar)
{
   static map<char, int> hexValue = {
      {'0', 0},  {'1', 1},  {'2', 2},  {'3', 3},  {'4', 4},  {'5', 5},
      {'6', 6},  {'7', 7},  {'8', 8},  {'9', 9},  {'a', 10}, {'A', 10},
      {'b', 11}, {'B', 11}, {'c', 12}, {'C', 12}, {'d', 13}, {'D', 13},
      {'e', 14}, {'E', 14}, {'f', 15}, {'F', 15},
   };

   auto match = hexValue.find(hexChar);
   if (match != hexValue.end())
      return match->second;
   return static_cast<unsigned int>(-1);
}


unsigned int intFromHexDoubleDigit(const std::string& hex)
{
   if (hex.size() != 2)
      return static_cast<unsigned int>(-1);

   const int high = intFromHexDigit(hex[0]);
   const int low = intFromHexDigit(hex[1]);
   if (high == -1 || low == -1)
      return static_cast<unsigned int>(-1);

   return 16 * high + low;
}


pair<bool, Rgb> colorFromString(const std::string& colorAsStr)
{
   if (colorAsStr.size() != 6)
      return {false, {}};

   const uint8_t r = static_cast<uint8_t>(intFromHexDoubleDigit(colorAsStr.substr(0, 2)));
   const uint8_t g = static_cast<uint8_t>(intFromHexDoubleDigit(colorAsStr.substr(2, 2)));
   const uint8_t b = static_cast<uint8_t>(intFromHexDoubleDigit(colorAsStr.substr(4, 2)));
   if (r == -1 || g == -1 || b == -1)
      return {false, {}};

   return {true, Rgb{r, g, b}};
}

} // namespace


namespace ccon
{
///////////////////

CmdParsingResult parseCmd(const std::string& cmd, const CmdSpec& spec)
{
   vector<string> cmdPieces = split(cmd, " ");
   if (cmdPieces.empty())
      return {};

   if (!spec.matchesName(lowercase(cmdPieces[0])))
      return {};

   VerifiedCmd verifiedCmd;
   verifiedCmd.name = spec.name();

   cmdPieces.erase(cmdPieces.begin());

   bool argsValid = false;
   tie(argsValid, verifiedCmd.args) = parseCmdArgs(cmdPieces, spec);
   if (!argsValid)
      return {true, false, {}};

   return {true, true, verifiedCmd};
}


bool containsHelpParameter(const CmdArgs& args)
{
   return any_of(begin(args), end(args), [](const string& arg) {
      return HelpArgSpec.matchLabel(stripArgSeparators(arg));
   });
}


bool containsHelpParameter(const VerifiedArgs& args)
{
   return any_of(begin(args), end(args), [](const VerifiedArg& arg) {
      return HelpArgSpec.matchLabel(stripArgSeparators(arg.label));
   });
}


bool haveArgWithLabel(VerifiedArgs::const_iterator beginOptions,
                      VerifiedArgs::const_iterator endOptions, const std::string& label)
{
   return find_if(beginOptions, endOptions,
                  [&label](const auto& arg) { return arg.label == label; }) != endOptions;
}


VerifiedArgs::const_iterator findArgWithLabel(VerifiedArgs::const_iterator beginOptions,
                                              VerifiedArgs::const_iterator endOptions,
                                              const std::string& label)
{
   return find_if(beginOptions, endOptions,
                  [&label](const auto& arg) { return arg.label == label; });
}


int parseIntArg(VerifiedArgs::const_iterator beginOptions,
                VerifiedArgs::const_iterator endOptions, const std::string& argLabel,
                int defaultValue)
{
   auto argIter = findArgWithLabel(beginOptions, endOptions, argLabel);
   if (argIter != endOptions)
      return intFromStr(argIter->values[0], defaultValue);
   return defaultValue;
}


Rgb parseColorArg(VerifiedArgs::const_iterator beginOptions,
                  VerifiedArgs::const_iterator endOptions, const std::string& argLabel,
                  const Rgb& defaultColor)
{
   auto colorIter = findArgWithLabel(beginOptions, endOptions, argLabel);
   if (colorIter == endOptions)
      return defaultColor;

   assert(colorIter->values.size() == 1);
   bool haveColor = false;
   Rgb color;
   tie(haveColor, color) = colorFromString(colorIter->values[0]);

   return haveColor ? color : defaultColor;
}

} // namespace ccon
