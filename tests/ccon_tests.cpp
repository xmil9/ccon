//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "auto_completion_tests.h"
#include "blackboard_tests.h"
#include "cmd_depot_tests.h"
#include "cmd_parser_tests.h"
#include "cmd_spec_tests.h"
#include "console_util_tests.h"
#include "formatting_tests.h"
#include "preferences_tests.h"
#include <cstdlib>
#include <iostream>


int main()
{
   testAutoCompletion();
   testBlackboard();
   testCmdDepot();
   testCmdParser();
   testCmdSpec();
   testConsoleUtil();
   testFormatting();
   testPreferences();

   std::cout << "ccon tests finished.\n";
   return EXIT_SUCCESS;
}
