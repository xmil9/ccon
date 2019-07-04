//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace ccon
{
class CmdSpec;
struct Cmd;
struct ConsoleUI;
}


namespace ccon
{
///////////////////

// Respository of commands and durable data that they require.
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
