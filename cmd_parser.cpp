//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "cmd_parser.h"
#include "cmd_spec.h"
#include "console_util.h"
#include "essentutils/color.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <cassert>
#include <map>
#include <optional>


namespace
{
///////////////////

unsigned int intFromHexDigit(char hexChar)
{
   static std::map<char, int> hexValue = {
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


std::optional<sutil::Rgb> colorFromString(const std::string& colorAsStr)
{
   if (colorAsStr.size() != 6)
      return std::nullopt;

   const unsigned int r = intFromHexDoubleDigit(colorAsStr.substr(0, 2));
   const unsigned int g = intFromHexDoubleDigit(colorAsStr.substr(2, 2));
   const unsigned int b = intFromHexDoubleDigit(colorAsStr.substr(4, 2));
   if (r == -1 || g == -1 || b == -1)
      return std::nullopt;

   return sutil::Rgb{static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                     static_cast<uint8_t>(b)};
}

} // namespace


namespace ccon
{
///////////////////

bool containsHelpParameter(const CmdArgs& args)
{
   return any_of(begin(args), end(args), [](const std::string& arg) {
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
   return findArgWithLabel(beginOptions, endOptions, label) != endOptions;
}


VerifiedArgs::const_iterator findArgWithLabel(VerifiedArgs::const_iterator beginOptions,
                                              VerifiedArgs::const_iterator endOptions,
                                              const std::string& label)
{
   return find_if(beginOptions, endOptions, [&label](const auto& arg) {
      return stripArgSeparators(arg.label) == stripArgSeparators(label);
   });
}


int parseIntArg(VerifiedArgs::const_iterator beginOptions,
                VerifiedArgs::const_iterator endOptions, const std::string& argLabel,
                int defaultValue)
{
   auto argIter = findArgWithLabel(beginOptions, endOptions, argLabel);
   if (argIter != endOptions)
      return sutil::intFromStr(argIter->values[0], defaultValue);
   return defaultValue;
}


sutil::Rgb parseColorArg(VerifiedArgs::const_iterator beginOptions,
                         VerifiedArgs::const_iterator endOptions,
                         const std::string& argLabel, const sutil::Rgb& defaultColor)
{
   auto colorIter = findArgWithLabel(beginOptions, endOptions, argLabel);
   if (colorIter == endOptions)
      return defaultColor;

   assert(colorIter->values.size() == 1);
   const std::optional<sutil::Rgb> color = colorFromString(colorIter->values[0]);
   return color.has_value() ? color.value() : defaultColor;
}

} // namespace ccon
