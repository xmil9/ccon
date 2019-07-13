//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "cmd_depot_tests.h"
#include "cmd_depot.h"
#include "cmd_spec.h"
#include "test_util.h"
#include <algorithm>
#include <memory>
#include <set>
#include <string>
#include <type_traits>

using namespace ccon;


namespace
{
///////////////////

struct TestCmd : public Cmd
{
   CmdOutput execute(const VerifiedCmd& input) override { return {"done"}; }
};


template <typename Iter> bool haveCommand(Iter b, Iter e, const std::string& name)
{
   static_assert(std::is_same_v<typename Iter::value_type, CmdSpec>);
   return std::find_if(b, e, [&name](const auto& spec) { return spec.name() == name; }) !=
          e;
}


///////////////////

void testCmdDepotAddCommand()
{
   {
      const std::string caseLabel = "CmdDepot::addCommand for one command";
      const CmdSpec spec{"test", "t", "", {}, ""};
      auto cmdFactory = []() { return std::unique_ptr<TestCmd>(); };
      CmdDepot depot;
      const std::size_t prevNumCmds = depot.availableCommands().size();

      depot.addCommand(spec, cmdFactory);

      const auto& specs = depot.availableCommands();
      VERIFY(specs.size() == prevNumCmds + 1, caseLabel);
      VERIFY(haveCommand(specs.begin(), specs.end(), "test"), caseLabel);
   }
   {
      const std::string caseLabel = "CmdDepot::addCommand for multiple commands";
      const CmdSpec spec1{"cmd1", "c1", "", {}, ""};
      const CmdSpec spec2{"cmd2", "c2", "", {}, ""};
      const CmdSpec spec3{"cmd3", "c3", "", {}, ""};
      auto cmdFactory = []() { return std::unique_ptr<TestCmd>(); };
      CmdDepot depot;
      const std::size_t prevNumCmds = depot.availableCommands().size();

      depot.addCommand(spec1, cmdFactory);
      depot.addCommand(spec2, cmdFactory);
      depot.addCommand(spec3, cmdFactory);

      const auto& specs = depot.availableCommands();
      VERIFY(specs.size() == prevNumCmds + 3, caseLabel);
      VERIFY(haveCommand(specs.begin(), specs.end(), "cmd1"), caseLabel);
      VERIFY(haveCommand(specs.begin(), specs.end(), "cmd2"), caseLabel);
      VERIFY(haveCommand(specs.begin(), specs.end(), "cmd3"), caseLabel);
   }
   {
      const std::string caseLabel = "CmdDepot::addCommand for existing command";
      const CmdSpec spec1{"cmd1", "c1", "", {}, ""};
      const CmdSpec spec2{"cmd2", "c2", "", {}, ""};
      auto cmdFactory = []() { return std::unique_ptr<TestCmd>(); };
      CmdDepot depot;
      depot.addCommand(spec1, cmdFactory);
      depot.addCommand(spec2, cmdFactory);
      const std::size_t prevNumCmds = depot.availableCommands().size();

      depot.addCommand(spec1, cmdFactory);

      const auto& specs = depot.availableCommands();
      VERIFY(specs.size() == prevNumCmds, caseLabel);
      VERIFY(haveCommand(specs.begin(), specs.end(), "cmd1"), caseLabel);
      VERIFY(haveCommand(specs.begin(), specs.end(), "cmd2"), caseLabel);
   }
}


void testCmdDepotMakeCommand()
{
   {
      const std::string caseLabel = "CmdDepot::makeCommand for existing command";
      const CmdSpec spec{"cmd1", "c1", "", {}, ""};
      auto cmdFactory = []() { return std::make_unique<TestCmd>(); };
      CmdDepot depot;
      depot.addCommand(spec, cmdFactory);

      auto cmd = depot.makeCommand("cmd1");

      VERIFY(cmd.operator bool(), caseLabel);
      VERIFY(cmd->execute(VerifiedCmd())[0] == "done", caseLabel);
   }
   {
      const std::string caseLabel = "CmdDepot::makeCommand for not existing command";
      const CmdSpec spec{"cmd1", "c1", "", {}, ""};
      auto cmdFactory = []() { return std::unique_ptr<TestCmd>(); };
      CmdDepot depot;
      depot.addCommand(spec, cmdFactory);

      auto cmd = depot.makeCommand("cmd2");

      VERIFY(!cmd.operator bool(), caseLabel);
   }
}


void testCmdDepotGetCommandHelp()
{
   {
      const std::string caseLabel = "CmdDepot::getCommandHelp for existing command";
      const CmdSpec spec{"cmd1", "c1", "", {}, ""};
      auto cmdFactory = []() { return std::unique_ptr<TestCmd>(); };
      CmdDepot depot;
      depot.addCommand(spec, cmdFactory);

      auto help = depot.getCommandHelp("cmd1");

      VERIFY(!help.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "CmdDepot::getCommandHelp for not existing command";
      const CmdSpec spec{"cmd1", "c1", "", {}, ""};
      auto cmdFactory = []() { return std::unique_ptr<TestCmd>(); };
      CmdDepot depot;
      depot.addCommand(spec, cmdFactory);

      auto help = depot.getCommandHelp("cmd2");

      VERIFY(help.empty(), caseLabel);
   }
}

} // namespace


void testCmdDepot()
{
   testCmdDepotAddCommand();
   testCmdDepotMakeCommand();
   testCmdDepotGetCommandHelp();
}
