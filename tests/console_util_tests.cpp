//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "console_util_tests.h"
#include "console_util.h"
#include "test_util.h"
#include <string>

using namespace ccon;


namespace
{
///////////////////

void testStripArgSeparators()
{
   {
      const std::string caseLabel = "testStripArgSeparators for single separator";
      VERIFY(stripArgSeparators("-arg") == "arg", caseLabel);
   }
   {
      const std::string caseLabel = "testStripArgSeparators for double separator";
      VERIFY(stripArgSeparators("--arg") == "arg", caseLabel);
   }
   {
      const std::string caseLabel = "testStripArgSeparators for no separator";
      VERIFY(stripArgSeparators("arg") == "arg", caseLabel);
   }
   {
      const std::string caseLabel = "testStripArgSeparators for separator in middle";
      VERIFY(stripArgSeparators("arg-more") == "arg-more", caseLabel);
   }
   {
      const std::string caseLabel = "testStripArgSeparators for separator only";
      VERIFY(stripArgSeparators("-") == "", caseLabel);
   }
   {
      const std::string caseLabel = "testStripArgSeparators for empty string";
      VERIFY(stripArgSeparators("") == "", caseLabel);
   }
}

} // namespace


void testConsoleUtil()
{
   testStripArgSeparators();
}
