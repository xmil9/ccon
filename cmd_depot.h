//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd.h"
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace ccon
{
class CmdSpec;
struct ConsoleUI;
}


namespace ccon
{
///////////////////

// Respository of console commands.
class CmdDepot
{
public:
   CmdDepot();
   ~CmdDepot();
   CmdDepot(const CmdDepot&) = delete;
   CmdDepot(CmdDepot&&) = default;
   CmdDepot& operator=(const CmdDepot&) = delete;
   CmdDepot& operator=(CmdDepot&&) = default;

   void setUI(ConsoleUI* ui);
   void addCommand(const CmdSpec& spec, CmdFactoryFn factoryFn);

   // Returns a collection of all available console command specs.
   const std::set<CmdSpec>& availableCommands() const;
   // Instantiates a command with a given name.
   std::unique_ptr<Cmd> makeCommand(const std::string& cmdName) const;
   // Returns the help description for a given command.
   std::vector<std::string> getCommandHelp(const std::string& cmdName) const;

private:
   class Impl;
   std::unique_ptr<Impl> m_pimpl;
};

} // namespace ccon
