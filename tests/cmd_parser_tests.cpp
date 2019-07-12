//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "cmd_parser_tests.h"
#include "cmd_parser.h"
#include "test_util.h"
#include "essentutils/color.h"
#include <string>

using namespace ccon;
using namespace std;
using namespace sutil;


namespace
{
///////////////////

void testContainsHelpParameterForCmdArg()
{
   {
      const std::string caseLabel =
         "containsHelpParameter(CmdArgs) for help arg as only arg";
      const CmdArgs args{"-help"};
      VERIFY(containsHelpParameter(args), caseLabel);
   }
   {
      const std::string caseLabel =
         "containsHelpParameter(CmdArgs) for help arg in multiple args";
      const CmdArgs args{"1", "2", "-help", "-flag"};
      VERIFY(containsHelpParameter(args), caseLabel);
   }
   {
      const std::string caseLabel =
         "containsHelpParameter(CmdArgs) for args without help arg";
      const CmdArgs args{"1", "2", "-flag"};
      VERIFY(!containsHelpParameter(args), caseLabel);
   }
   {
      const std::string caseLabel = "containsHelpParameter(CmdArgs) for no args";
      const CmdArgs args{};
      VERIFY(!containsHelpParameter(args), caseLabel);
   }
}


void testContainsHelpParameterForVerifiedArg()
{
   {
      const std::string caseLabel =
         "containsHelpParameter(VerifiedArgs) for help arg as only arg";
      const VerifiedArgs args{{"help"}};
      VERIFY(containsHelpParameter(args), caseLabel);
   }
   {
      const std::string caseLabel =
         "containsHelpParameter(VerifiedArgs) for help arg in multiple args";
      const VerifiedArgs args{{"", {"1", "2"}}, {"help"}, {"flag"}};
      VERIFY(containsHelpParameter(args), caseLabel);
   }
   {
      const std::string caseLabel =
         "containsHelpParameter(VerifiedArgs) for args without help arg";
      const VerifiedArgs args{{"", {"1", "2"}}, {"flag"}};
      VERIFY(!containsHelpParameter(args), caseLabel);
   }
   {
      const std::string caseLabel = "containsHelpParameter(VerifiedArgs) for no args";
      const VerifiedArgs args{};
      VERIFY(!containsHelpParameter(args), caseLabel);
   }
}


void testHaveArgWithLabel()
{
   {
      const std::string caseLabel = "haveArgWithLabel for target arg as only arg";
      const VerifiedArgs args{{"scan"}};
      VERIFY(haveArgWithLabel(args.begin(), args.end(), "scan"), caseLabel);
   }
   {
      const std::string caseLabel = "haveArgWithLabel for target arg and other args";
      const VerifiedArgs args{{"", {"1", "2"}}, {"scan"}, {"flag"}};
      VERIFY(haveArgWithLabel(args.begin(), args.end(), "scan"), caseLabel);
   }
   {
      const std::string caseLabel = "haveArgWithLabel for missing target arg";
      const VerifiedArgs args{{"", {"1", "2"}}, {"flag"}};
      VERIFY(!haveArgWithLabel(args.begin(), args.end(), "scan"), caseLabel);
   }
   {
      const std::string caseLabel = "haveArgWithLabel for no args";
      const VerifiedArgs args;
      VERIFY(!haveArgWithLabel(args.begin(), args.end(), "scan"), caseLabel);
   }
   {
      const std::string caseLabel =
         "haveArgWithLabel for target arg with dash in search pattern";
      const VerifiedArgs args{{"scan"}};
      VERIFY(haveArgWithLabel(args.begin(), args.end(), "-scan"), caseLabel);
   }
}


void testFindArgWithLabel()
{
   {
      const std::string caseLabel = "findArgWithLabel for target arg as only arg";
      const VerifiedArgs args{{"scan"}};
      VERIFY(findArgWithLabel(args.begin(), args.end(), "scan") == args.begin(),
             caseLabel);
   }
   {
      const std::string caseLabel = "findArgWithLabel for target arg and other args";
      const VerifiedArgs args{{"", {"1", "2"}}, {"scan"}, {"flag"}};
      VERIFY(findArgWithLabel(args.begin(), args.end(), "scan") == args.begin() + 1,
             caseLabel);
   }
   {
      const std::string caseLabel = "findArgWithLabel for missing target arg";
      const VerifiedArgs args{{"", {"1", "2"}}, {"flag"}};
      VERIFY(findArgWithLabel(args.begin(), args.end(), "scan") == args.end(), caseLabel);
   }
   {
      const std::string caseLabel = "findArgWithLabel for no args";
      const VerifiedArgs args;
      VERIFY(findArgWithLabel(args.begin(), args.end(), "scan") == args.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "findArgWithLabel for target arg with dash in search pattern";
      const VerifiedArgs args{{"scan"}};
      VERIFY(findArgWithLabel(args.begin(), args.end(), "-scan") == args.begin(),
             caseLabel);
   }
}


void testParseIntArg()
{
   {
      const std::string caseLabel = "parseIntArg for integer arg";
      const VerifiedArgs args{{"scan", {"101"}}};
      VERIFY(parseIntArg(args.begin(), args.end(), "scan", 999) == 101, caseLabel);
   }
   {
      const std::string caseLabel = "parseIntArg for missing integer arg";
      const VerifiedArgs args{{"other", {"101"}}};
      VERIFY(parseIntArg(args.begin(), args.end(), "scan", 999) == 999, caseLabel);
   }
   {
      const std::string caseLabel = "parseIntArg for negative integer arg";
      const VerifiedArgs args{{"scan", {"-101"}}};
      VERIFY(parseIntArg(args.begin(), args.end(), "scan", 999) == -101, caseLabel);
   }
   {
      const std::string caseLabel = "parseIntArg for no args";
      const VerifiedArgs args;
      VERIFY(parseIntArg(args.begin(), args.end(), "scan", 999) == 999, caseLabel);
   }
   {
      const std::string caseLabel = "parseIntArg for multiple args";
      const VerifiedArgs args{{"scan", {"101"}}, {"scan2", {"102"}}, {"scan3", {"103"}}};
      VERIFY(parseIntArg(args.begin(), args.end(), "scan2", 999) == 102, caseLabel);
   }
}


void testParseColorArg()
{
   {
      const std::string caseLabel = "parseColorArg for color arg";
      const VerifiedArgs args{{"bkg", {"050505"}}};
      const Rgb color = parseColorArg(args.begin(), args.end(), "bkg", {});
      VERIFY(color == Rgb(5, 5, 5), caseLabel);
   }
   {
      const std::string caseLabel = "parseColorArg for missing color arg";
      const VerifiedArgs args{{"other", {"050505"}}};
      const Rgb defaultColor = Rgb{9, 9, 9};
      const Rgb color = parseColorArg(args.begin(), args.end(), "bkg", defaultColor);
      VERIFY(color == defaultColor, caseLabel);
   }
   {
      const std::string caseLabel = "parseColorArg for no args";
      const VerifiedArgs args;
      const Rgb defaultColor = Rgb{9, 9, 9};
      const Rgb color = parseColorArg(args.begin(), args.end(), "bkg", defaultColor);
      VERIFY(color == defaultColor, caseLabel);
   }
   {
      const std::string caseLabel = "parseColorArg for multiple args";
      const VerifiedArgs args{{"scan", {"-101"}}, {"bkg", {"a7993c"}}, {"other"}};
      const Rgb color = parseColorArg(args.begin(), args.end(), "bkg", {});
      VERIFY(color == Rgb(167, 153, 60), caseLabel);
   }
   {
      const std::string caseLabel = "parseColorArg for invalid color (non-hex digit)";
      const VerifiedArgs args{{"bkg", {"11220w"}}};
      const Rgb defaultColor = Rgb{9, 9, 9};
      const Rgb color = parseColorArg(args.begin(), args.end(), "bkg", defaultColor);
      VERIFY(color == defaultColor, caseLabel);
   }
   {
      const std::string caseLabel = "parseColorArg for invalid color (too few digits)";
      const VerifiedArgs args{{"bkg", {"1122"}}};
      const Rgb defaultColor = Rgb{9, 9, 9};
      const Rgb color = parseColorArg(args.begin(), args.end(), "bkg", defaultColor);
      VERIFY(color == defaultColor, caseLabel);
   }
   {
      const std::string caseLabel = "parseColorArg for invalid color (too mant digits)";
      const VerifiedArgs args{{"bkg", {"11223344"}}};
      const Rgb defaultColor = Rgb{9, 9, 9};
      const Rgb color = parseColorArg(args.begin(), args.end(), "bkg", defaultColor);
      VERIFY(color == defaultColor, caseLabel);
   }
}

} // namespace


void testCmdParser()
{
   testContainsHelpParameterForCmdArg();
   testContainsHelpParameterForVerifiedArg();
   testHaveArgWithLabel();
   testFindArgWithLabel();
   testParseIntArg();
   testParseColorArg();
}
