//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "formatting_tests.h"
#include "formatting.h"
#include "test_util.h"
#include <array>
#include <set>
#include <string>
#include <vector>

using namespace ccon;


namespace
{
///////////////////

void testFormatIntOutput()
{
   {
      const std::string caseLabel = "formatIntOutput for positive int";
      VERIFY(formatIntOutput(100, "test", IntBase::Dec) == "test 100", caseLabel);
   }
   {
      const std::string caseLabel = "formatIntOutput for zero";
      VERIFY(formatIntOutput(0, "zero", IntBase::Dec) == "zero 0", caseLabel);
   }
   {
      const std::string caseLabel = "formatIntOutput for negative int";
      VERIFY(formatIntOutput(-348764, "negative", IntBase::Dec) == "negative -348764",
             caseLabel);
   }
   {
      const std::string caseLabel = "formatIntOutput for long";
      VERIFY(formatIntOutput(30L, "long", IntBase::Dec) == "long 30", caseLabel);
   }
   {
      const std::string caseLabel = "formatIntOutput for unsigned short";
      VERIFY(formatIntOutput<unsigned short>(23, "short", IntBase::Dec) == "short 23",
             caseLabel);
   }
   {
      const std::string caseLabel = "formatIntOutput for hex base";
      VERIFY(formatIntOutput(100, "hex", IntBase::Hex) == "hex 0x64", caseLabel);
   }
   {
      const std::string caseLabel = "formatIntOutput for octal base";
      VERIFY(formatIntOutput(100, "oct", IntBase::Oct) == "oct 0144", caseLabel);
   }
   {
      const std::string caseLabel = "formatIntOutput without label";
      VERIFY(formatIntOutput(100, "", IntBase::Dec) == "100", caseLabel);
   }
}


void testFormatIntArrayOutput()
{
   {
      const std::string caseLabel = "formatIntArrayOutput for multiple ints";
      const std::vector<int> ints{-1, 0, 1, 2};
      VERIFY(formatIntArrayOutput(ints.begin(), ints.end(), "array") ==
                "array [-1, 0, 1, 2]",
             caseLabel);
   }
   {
      const std::string caseLabel = "formatIntArrayOutput for set";
      const std::set<int> ints{-1, 0, 1, 2};
      VERIFY(formatIntArrayOutput(ints.begin(), ints.end(), "set") == "set [-1, 0, 1, 2]",
             caseLabel);
   }
   {
      const std::string caseLabel = "formatIntArrayOutput for C array";
      const int ints[] = {-1, 0, 1, 2};
      VERIFY(formatIntArrayOutput(std::begin(ints), std::end(ints), "C array") ==
                "C array [-1, 0, 1, 2]",
             caseLabel);
   }
   {
      const std::string caseLabel = "formatIntArrayOutput for multiple ints in set";
      const std::set<int> ints{-1, 0, 1, 2};
      VERIFY(formatIntArrayOutput(ints.begin(), ints.end(), "array") ==
                "array [-1, 0, 1, 2]",
             caseLabel);
   }
   {
      const std::string caseLabel = "formatIntArrayOutput for empty sequence";
      const std::vector<int> empty;
      VERIFY(formatIntArrayOutput(empty.begin(), empty.end(), "empty") == "empty []",
             caseLabel);
   }
   {
      const std::string caseLabel = "formatIntArrayOutput for int64s";
      const std::vector<int64_t> int64s{-1, 0, 1, 2};
      VERIFY(formatIntArrayOutput(int64s.begin(), int64s.end(), "array") ==
                "array [-1, 0, 1, 2]",
             caseLabel);
   }
   {
      const std::string caseLabel = "formatIntArrayOutput without label";
      const std::array<int, 4> arr{-1, 0, 1, 2};
      VERIFY(formatIntArrayOutput(arr.begin(), arr.end(), "") == "[-1, 0, 1, 2]",
             caseLabel);
   }
}


void testFormatStringOutput()
{
   {
      const std::string caseLabel = "formatStringOutput with label";
      VERIFY(formatStringOutput("value", "label") == "label value", caseLabel);
   }
   {
      const std::string caseLabel = "formatStringOutput without label";
      VERIFY(formatStringOutput("value", "") == "value", caseLabel);
   }
}

} // namespace


void testFormatting()
{
   testFormatIntOutput();
   testFormatIntArrayOutput();
   testFormatStringOutput();
}
