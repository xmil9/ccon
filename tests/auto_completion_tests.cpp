//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "auto_completion_tests.h"
#include "auto_completion.h"
#include "test_util.h"
#include <set>
#include <string>

using namespace ccon;


namespace
{
///////////////////

void testAutoCompletionNext()
{
   {
      const std::string caseLabel =
         "AutoCompletion::next for completing command name of single matching command";
      std::set<CmdSpec> specs = {{"test", "t", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);
      VERIFY(ac.next("te") == "test", caseLabel);
   }
   {
      const std::string caseLabel =
         "AutoCompletion::next for completing command name of mutiple matching commands";
      std::set<CmdSpec> specs = {{"test", "ts", "", {}, ""},
                                 {"teach", "ta", "", {}, ""},
                                 {"temp", "tp", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);
      VERIFY(ac.next("te") == "teach", caseLabel);
      VERIFY(ac.next("te") == "temp", caseLabel);
      VERIFY(ac.next("te") == "test", caseLabel);
   }
   {
      const std::string caseLabel =
         "AutoCompletion::next for completing command name of no matching commands";
      std::set<CmdSpec> specs = {
         {"add", "a", "", {}, ""}, {"bevel", "b", "", {}, ""}, {"zap", "z", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);
      VERIFY(ac.next("te") == "", caseLabel);
   }
   {
      const std::string caseLabel =
         "AutoCompletion::next for completing command name of some matching commands";
      std::set<CmdSpec> specs = {{"add", "a", "", {}, ""},
                                 {"teach", "ta", "", {}, ""},
                                 {"bevel", "b", "", {}, ""},
                                 {"zap", "z", "", {}, ""},
                                 {"temp", "tp", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);
      VERIFY(ac.next("te") == "teach", caseLabel);
      VERIFY(ac.next("te") == "temp", caseLabel);
   }
   {
      const std::string caseLabel = "AutoCompletion::next for returning starting over "
                                    "when completions are exhausted";
      std::set<CmdSpec> specs = {{"add", "a", "", {}, ""},
                                 {"teach", "ta", "", {}, ""},
                                 {"bevel", "b", "", {}, ""},
                                 {"zap", "z", "", {}, ""},
                                 {"temp", "tp", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);
      VERIFY(ac.next("te") == "teach", caseLabel);
      VERIFY(ac.next("te") == "temp", caseLabel);
      VERIFY(ac.next("te") == "teach", caseLabel);
      VERIFY(ac.next("te") == "temp", caseLabel);
      VERIFY(ac.next("te") == "teach", caseLabel);
      VERIFY(ac.next("te") == "temp", caseLabel);
   }
}


void testAutoCompletionAll()
{
   {
      const std::string caseLabel =
         "AutoCompletion::all for completing command name of single matching command";
      std::set<CmdSpec> specs = {{"test", "t", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);

      const auto all = ac.all("t");

      VERIFY(all.size() == 1 && all[0] == "test", caseLabel);
   }
   {
      const std::string caseLabel =
         "AutoCompletion::all for completing command name of mutiple matching commands";
      std::set<CmdSpec> specs = {{"test", "ts", "", {}, ""},
                                 {"teach", "ta", "", {}, ""},
                                 {"temp", "tp", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);

      const auto all = ac.all("te");

      VERIFY(all.size() == 3, caseLabel);
      VERIFY(std::find(all.begin(), all.end(), "test") != all.end(), caseLabel);
      VERIFY(std::find(all.begin(), all.end(), "teach") != all.end(), caseLabel);
      VERIFY(std::find(all.begin(), all.end(), "temp") != all.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "AutoCompletion::all for completing command name of no matching commands";
      std::set<CmdSpec> specs = {
         {"add", "a", "", {}, ""}, {"bevel", "b", "", {}, ""}, {"zap", "z", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);

      const auto all = ac.all("te");

      VERIFY(all.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "AutoCompletion::all for completing command name of some matching commands";
      std::set<CmdSpec> specs = {{"add", "a", "", {}, ""},
                                 {"teach", "ta", "", {}, ""},
                                 {"bevel", "b", "", {}, ""},
                                 {"zap", "z", "", {}, ""},
                                 {"temp", "tp", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);

      const auto all = ac.all("te");

      VERIFY(all.size() == 2, caseLabel);
      VERIFY(std::find(all.begin(), all.end(), "teach") != all.end(), caseLabel);
      VERIFY(std::find(all.begin(), all.end(), "temp") != all.end(), caseLabel);
   }
   {
      const std::string caseLabel = "AutoCompletion::all for empty pattern";
      std::set<CmdSpec> specs = {{"add", "a", "", {}, ""}, {"teach", "ta", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);

      const auto all = ac.all("");

      VERIFY(all.empty(), caseLabel);
   }
}


void testAutoCompletionReset()
{
   {
      const std::string caseLabel =
         "AutoCompletion::reset for resetting the completion pattern";
      std::set<CmdSpec> specs = {{"add", "a", "", {}, ""},
                                 {"teach", "ta", "", {}, ""},
                                 {"bevel", "b", "", {}, ""},
                                 {"zap", "z", "", {}, ""},
                                 {"temp", "tp", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);

      auto all = ac.all("te");
      ac.reset();
      all = ac.all("b");

      VERIFY(all.size() == 1, caseLabel);
      VERIFY(std::find(all.begin(), all.end(), "bevel") != all.end(), caseLabel);
   }
   {
      const std::string caseLabel = "AutoCompletion::reset for clearing the completions";
      std::set<CmdSpec> specs = {{"add", "a", "", {}, ""},
                                 {"teach", "ta", "", {}, ""},
                                 {"bevel", "b", "", {}, ""},
                                 {"zap", "z", "", {}, ""},
                                 {"temp", "tp", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);

      const std::string firstCompletion = ac.next("te");
      ac.reset();

      VERIFY(ac.next("te") == firstCompletion, caseLabel);
   }
   {
      const std::string caseLabel = "AutoCompletion::reset for pattern does not reset "
                                    "unless reset is explicitly called";
      std::set<CmdSpec> specs = {{"add", "a", "", {}, ""},
                                 {"teach", "ta", "", {}, ""},
                                 {"bevel", "b", "", {}, ""},
                                 {"zap", "z", "", {}, ""},
                                 {"temp", "tp", "", {}, ""}};
      AutoCompletion ac;
      ac.setCmds(specs);

      const auto all1 = ac.all("te");
      const auto all2 = ac.all("b");

      VERIFY(all1 == all2, caseLabel);
   }
}

} // namespace


void testAutoCompletion()
{
   testAutoCompletionNext();
   testAutoCompletionAll();
   testAutoCompletionReset();
}
