//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "cmd_depot.h"
#include "cmd_spec.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <functional>

using namespace std;
using namespace sutil;


namespace ccon
{
///////////////////

void CmdDepot::addCommand(const CmdSpec& spec, CmdFactoryFn factoryFn)
{
   const bool haveCommand = (m_specs.find(spec) != m_specs.end());
   if (!haveCommand)
   {
      m_specs.insert(spec);
      m_cmdFactory[spec.name()] = factoryFn;
   }
}


const std::set<CmdSpec>& CmdDepot::availableCommands() const
{
   return m_specs;
}


std::unique_ptr<Cmd> CmdDepot::makeCommand(const std::string& cmdName) const
{
   const auto factoryIter = m_cmdFactory.find(cmdName);
   if (factoryIter != m_cmdFactory.end())
      return factoryIter->second();
   return {};
}


std::vector<std::string> CmdDepot::getCommandHelp(const std::string& cmdName) const
{
   const auto match =
      find_if(begin(m_specs), end(m_specs),
              [&cmdName](const CmdSpec& spec) { return spec.matchesName(cmdName); });
   if (match != end(m_specs))
      return split(match->help(), "\n");
   return {};
}

} // namespace ccon
