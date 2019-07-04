//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd.h"
#include "cmd_spec.h"

namespace sutil
{
struct Rgb;
}


namespace ccon
{
///////////////////

// Parses a given string against a given command spec.
struct CmdParsingResult
{
   bool foundCommand = false;
   bool areArgsValid = false;
   VerifiedCmd cmdInput;
};
CmdParsingResult parseCmd(const std::string& cmd, const CmdSpec& spec);

// Check, if given arguments contain the 'help' parameter.
bool containsHelpParameter(const CmdArgs& args);
bool containsHelpParameter(const VerifiedArgs& args);

// Checks if a given argument sequence contains an argument with a given label.
bool haveArgWithLabel(VerifiedArgs::const_iterator beginOptions,
                      VerifiedArgs::const_iterator endOptions, const std::string& label);

// Finds in a given argument sequence an argument with a given label.
// Returns the found arg's iterator.
VerifiedArgs::const_iterator findArgWithLabel(VerifiedArgs::const_iterator beginOptions,
                                              VerifiedArgs::const_iterator endOptions,
                                              const std::string& label);

// Parses a given argument sequence for an integer argument with a given label.
int parseIntArg(VerifiedArgs::const_iterator beginOptions,
                VerifiedArgs::const_iterator endOptions, const std::string& argLabel,
                int defaultValue);
// Parses a given argument sequence for a color argument with a given label.
sutil::Rgb parseColorArg(VerifiedArgs::const_iterator beginOptions,
                         VerifiedArgs::const_iterator endOptions,
                         const std::string& argLabel, const sutil::Rgb& defaultColor);

} // namespace ccon
