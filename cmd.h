//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <functional>
#include <string>
#include <vector>


namespace ccon
{
///////////////////

// Command argument that was verified by the parser to be valid.
struct VerifiedArg
{
   std::string label;
   std::vector<std::string> values;
};

using VerifiedArgs = std::vector<VerifiedArg>;


// Command that was verified by the parser to be valid.
struct VerifiedCmd
{
   std::string name;
   VerifiedArgs args;
};


///////////////////

// A command line split into words.
using CmdArgs = std::vector<std::string>;
// The output lines of a command.
using CmdOutput = std::vector<std::string>;


// Abstracts console commands.
struct Cmd
{
   virtual ~Cmd() = default;
   virtual CmdOutput execute(const VerifiedCmd& input) = 0;
};


// Type of factory functions for commands.
using CmdFactoryFn = std::function<std::unique_ptr<Cmd>()>;

} // namespace ccon
