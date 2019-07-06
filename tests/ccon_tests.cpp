//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "console_util_tests.h"
#include "formatting_tests.h"
#include "preferences_tests.h"
#include <cstdlib>
#include <iostream>


int main()
{
   testConsoleUtil();
   testFormatting();
   testPreferences();

   std::cout << "ccon tests finished.\n";
   return EXIT_SUCCESS;
}
