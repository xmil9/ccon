//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "cmd_depot.h"
#include "cmd_spec.h"
#include "commands/colors_cmd.h"
#include "commands/exit_cmd.h"
#include "commands/font_size_cmd.h"
#include "commands/help_cmd.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <functional>

using namespace std;
using namespace sutil;


namespace ccon
{
///////////////////

class CmdDepot::Impl
{
 public:
   Impl();
   ~Impl() = default;
   Impl(const Impl&) = default;
   Impl(Impl&&) = default;
   Impl& operator=(const Impl&) = default;
   Impl& operator=(Impl&&) = default;

   void setUI(ConsoleUI* ui);
   void addCommand(const CmdSpec& spec, CmdFactoryFn factoryFn);
   const std::set<CmdSpec>& availableCommands() const;
   std::unique_ptr<Cmd> makeCommand(const std::string& cmdName) const;
   std::vector<std::string> getCommandHelp(const std::string& cmdName) const;

 private:
   using CmdName = std::string;

 private:
   void initCmdSpecs();
   void initCmdFactory();

 private:
   std::set<CmdSpec> m_specs;
   std::unordered_map<CmdName, CmdFactoryFn> m_cmdFactory;
   ConsoleUI* m_ui = nullptr;
};


CmdDepot::Impl::Impl()
{
   initCmdSpecs();
   initCmdFactory();
}


void CmdDepot::Impl::setUI(ConsoleUI* ui)
{
   m_ui = ui;
}


void CmdDepot::Impl::addCommand(const CmdSpec& spec, CmdFactoryFn factoryFn)
{
   const bool haveCommand = (m_specs.find(spec) != m_specs.end());
   if (!haveCommand)
   {
      m_specs.insert(spec);
      m_cmdFactory[spec.name()] = factoryFn;
   }
}


const std::set<CmdSpec>& CmdDepot::Impl::availableCommands() const
{
   return m_specs;
}


std::unique_ptr<Cmd> CmdDepot::Impl::makeCommand(const std::string& cmdName) const
{
   const auto factoryIter = m_cmdFactory.find(cmdName);
   if (factoryIter != m_cmdFactory.end())
      return factoryIter->second();
   return {};
}


std::vector<std::string> CmdDepot::Impl::getCommandHelp(const std::string& cmdName) const
{
   const auto match =
      find_if(begin(m_specs), end(m_specs),
              [&cmdName](const CmdSpec& spec) { return spec.matchesName(cmdName); });
   if (match != end(m_specs))
      return split(match->help(), "\n");
   return {};
}


void CmdDepot::Impl::initCmdSpecs()
{
   m_specs = {
      makeConsoleColorsCmdSpec(),
      makeConsoleFontSizeCmdSpec(),
      makeExitCmdSpec(),
      makeHelpCmdSpec(),
   };
}


void CmdDepot::Impl::initCmdFactory()
{
   m_cmdFactory = {
      {consoleColorsCmdSpec::cmdName,
       [this]() { return make_unique<ConsoleColorsCmd>(m_ui); }},
      {consoleFontSizeCmdSpec::cmdName,
       [this]() { return make_unique<ConsoleFontSizeCmd>(m_ui); }},
      {exitCmdSpec::cmdName, [this]() { return make_unique<ExitCmd>(m_ui); }},
      {helpCmdSpec::cmdName, [this]() { return make_unique<HelpCmd>(m_specs); }},
   };
}


///////////////////

CmdDepot::CmdDepot() : m_pimpl(make_unique<Impl>())
{
}


// Need implementation of dtor (even though it does nothing) at a point where the Impl
// class is completely known to the compiler to avoid compile error about incomplete Impl
// type with unique_ptr.
CmdDepot::~CmdDepot()
{
}


void CmdDepot::setUI(ConsoleUI* ui)
{
   m_pimpl->setUI(ui);
}


void CmdDepot::addCommand(const CmdSpec& spec, CmdFactoryFn factoryFn)
{
   m_pimpl->addCommand(spec, factoryFn);
}


const std::set<CmdSpec>& CmdDepot::availableCommands() const
{
   return m_pimpl->availableCommands();
}


std::unique_ptr<Cmd> CmdDepot::makeCommand(const std::string& cmdName) const
{
   return m_pimpl->makeCommand(cmdName);
}


std::vector<std::string> CmdDepot::getCommandHelp(const std::string& cmdName) const
{
   return m_pimpl->getCommandHelp(cmdName);
}

} // namespace ccon
