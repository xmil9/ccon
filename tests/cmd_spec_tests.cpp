//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "cmd_spec_tests.h"
#include "cmd_spec.h"
#include "test_util.h"
#include <string>

using namespace ccon;
using namespace std;


namespace
{
///////////////////

void testLabelWithAbbrevCtor()
{
   {
      const string caseLabel = "LabelWithAbbrev ctor with label and abbreviation";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(label.label() == "test", caseLabel);
      VERIFY(label.abbreviation() == "t", caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev ctor with label only";
      LabelWithAbbrev label{"test", ""};
      VERIFY(label.label() == "test", caseLabel);
      VERIFY(label.abbreviation() == "", caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev ctor with abbreviation only";
      LabelWithAbbrev label{"", "t"};
      // Abbreviation is used as label.
      VERIFY(label.label() == "t", caseLabel);
      VERIFY(label.abbreviation() == "", caseLabel);
   }
}


void testLabelWithAbbrevConversionToBool()
{
   {
      const string caseLabel =
         "LabelWithAbbrev conversion to bool for label and abbreviation";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(label.operator bool(), caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev conversion to bool for label only";
      LabelWithAbbrev label{"test", ""};
      VERIFY(label.operator bool(), caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev conversion to bool for abbreviation only";
      LabelWithAbbrev label{"", "t"};
      VERIFY(label.operator bool(), caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev conversion to bool for empty object";
      LabelWithAbbrev label;
      VERIFY(!label.operator bool(), caseLabel);
   }
}


void testLabelWithAbbrevMatches()
{
   {
      const string caseLabel = "LabelWithAbbrev::matches for matching label";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(label.matches("test"), caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev::matches for matching abbreviation";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(label.matches("t"), caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev::matches for no match";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(!label.matches("abc"), caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev::matches for empty object";
      LabelWithAbbrev label;
      VERIFY(!label.matches("abc"), caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev::matches for matching empty string";
      LabelWithAbbrev label{"abc", "a"};
      VERIFY(!label.matches(""), caseLabel);
   }
   {
      const string caseLabel =
         "LabelWithAbbrev::matches for empty object matching empty string";
      LabelWithAbbrev label;
      VERIFY(label.matches(""), caseLabel);
   }
}


void testLabelWithAbbrevLabel()
{
   {
      const string caseLabel = "LabelWithAbbrev::label for populated label";
      LabelWithAbbrev label{"abc", "a"};
      VERIFY(label.label() == "abc", caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev::label for empty label";
      LabelWithAbbrev label;
      VERIFY(label.label() == "", caseLabel);
   }
}


void testLabelWithAbbrevHaveAbbreviation()
{
   {
      const string caseLabel =
         "LabelWithAbbrev::haveAbbreviation for populated abbreviation";
      LabelWithAbbrev label{"abc", "a"};
      VERIFY(label.haveAbbreviation(), caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev::haveAbbreviation for empty abbreviation";
      LabelWithAbbrev label{"abc", ""};
      VERIFY(!label.haveAbbreviation(), caseLabel);
   }
}


void testLabelWithAbbrevAbbreviation()
{
   {
      const string caseLabel =
         "LabelWithAbbrev::haveAbbreviation for populated abbreviation";
      LabelWithAbbrev label{"abc", "a"};
      VERIFY(label.abbreviation() == "a", caseLabel);
   }
   {
      const string caseLabel = "LabelWithAbbrev::haveAbbreviation for empty abbreviation";
      LabelWithAbbrev label{"abc", ""};
      VERIFY(label.abbreviation() == "", caseLabel);
   }
}


///////////////////

void testArgSpecDefaultCtor()
{
   {
      const string caseLabel = "ArgSpec default ctor";
      ArgSpec spec;
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
   }
}


void testArgSpecMakePositionalArg()
{
   {
      const string caseLabel = "ArgSpec::makePositionalArg for one value";

      ArgSpec spec = ArgSpec::makePositionalArg(1);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("1 value") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makePositionalArg for mutiple values";

      ArgSpec spec = ArgSpec::makePositionalArg(5);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("5 values") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makePositionalArg for zero or more values";

      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("zero or more") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makePositionalArg for one or more values";

      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("one or more") != string::npos, caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::makePositionalArg for zero values. This is an empty spec object.";

      ArgSpec spec = ArgSpec::makePositionalArg(0);

      VERIFY(!spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help == "", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makePositionalArg with description";

      const string descr = "test_description";
      ArgSpec spec = ArgSpec::makePositionalArg(1, descr);

      const string help = spec.help();
      VERIFY(help.find(descr) != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makePositionalArg without description";

      ArgSpec spec = ArgSpec::makePositionalArg(5);

      const string help = spec.help();
      VERIFY(help.find("--") == string::npos, caseLabel);
   }
}


void testArgSpecMakeOptionalArg()
{
   {
      const string caseLabel = "ArgSpec::makeOptionalArg for one value";

      const string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("1 value") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeOptionalArg for multiple values";

      const string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 5);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("5 values") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeOptionalArg for zore or more values";

      const string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, ArgSpec::ZeroOrMore);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("zero or more") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeOptionalArg for one or more values";

      const string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, ArgSpec::OneOrMore);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("one or more") != string::npos, caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::makeOptionalArg for zero values. This is a flag argument.";

      const string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 0);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("value") == string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeOptionalArg with uppercase label";

      const string label = "UPPER";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1);

      VERIFY(spec.label() == "upper", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeOptionalArg with abbreviation";

      const string label = "first_arg";
      const string abbrev = "qq";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1, abbrev);

      const string help = spec.help();
      VERIFY(help.find(abbrev) != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeOptionalArg with uppercase abbreviation";

      const string label = "first_arg";
      const string abbrev = "QQ";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1, abbrev);

      const string help = spec.help();
      VERIFY(help.find("qq") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeOptionalArg with description";

      const string label = "first_arg";
      const string descr = "test_description";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1, "", descr);

      const string help = spec.help();
      VERIFY(help.find(descr) != string::npos, caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::makeOptionalArg without label. This is a positional argument.";

      ArgSpec spec = ArgSpec::makeOptionalArg("", 1);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("value") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeOptionalArg without label but abbreviation. "
                               "The abbreviation becomes the label.";

      const string abbrev = "qq";
      ArgSpec spec = ArgSpec::makeOptionalArg("", 1, abbrev);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == abbrev, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("value") != string::npos, caseLabel);
      VERIFY(help.find(abbrev) != string::npos, caseLabel);
   }
}


void testArgSpecMakeFlagArg()
{
   {
      const string caseLabel = "ArgSpec::makeFlagArg";

      const string label = "first_arg";
      ArgSpec spec = ArgSpec::makeFlagArg(label);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find("value") == string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeFlagArg with uppercase label";

      const string label = "UPPER";
      ArgSpec spec = ArgSpec::makeFlagArg(label);

      VERIFY(spec.label() == "upper", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeFlagArg with abbreviation";

      const string label = "first_arg";
      const string abbrev = "qq";
      ArgSpec spec = ArgSpec::makeFlagArg(label, abbrev);

      const string help = spec.help();
      VERIFY(help.find(abbrev) != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeFlagArg with uppercase abbreviation";

      const string label = "first_arg";
      const string abbrev = "QQ";
      ArgSpec spec = ArgSpec::makeFlagArg(label, abbrev);

      const string help = spec.help();
      VERIFY(help.find("qq") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeFlagArg with description";

      const string label = "first_arg";
      const string descr = "test_description";
      ArgSpec spec = ArgSpec::makeFlagArg(label, "", descr);

      const string help = spec.help();
      VERIFY(help.find(descr) != string::npos, caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::makeFlagArg without label. This is an empty spec object.";

      ArgSpec spec = ArgSpec::makeFlagArg("");

      VERIFY(!spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      VERIFY(spec.help().empty(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::makeFlagArg without label but abbreviation. "
                               "The abbreviation becomes the label.";

      const string abbrev = "qq";
      ArgSpec spec = ArgSpec::makeFlagArg("", abbrev);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == abbrev, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const string help = spec.help();
      VERIFY(help.find(abbrev) != string::npos, caseLabel);
   }
}


void testArgSpecConversionToBool()
{
   {
      const string caseLabel = "ArgSpec conversion to bool for empty object";
      ArgSpec spec;
      VERIFY(!spec.operator bool(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec conversion to bool for empty object";
      ArgSpec spec;
      VERIFY(!spec.operator bool(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec conversion to bool for spec with value";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(spec.operator bool(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec conversion to bool for spec with label";
      ArgSpec spec = ArgSpec::makeFlagArg("label");
      VERIFY(spec.operator bool(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec conversion to bool for spec with description only";
      ArgSpec spec = ArgSpec::makeFlagArg("", "", "descr");
      VERIFY(!spec.operator bool(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec conversion to bool for spec with abbreviation only";
      ArgSpec spec = ArgSpec::makeFlagArg("", "abbreviation");
      // The abbreviation becomes the label.
      VERIFY(spec.operator bool(), caseLabel);
   }
}


void testArgSpecIsRequired()
{
   {
      const string caseLabel = "ArgSpec::isRequired for positional argument";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(spec.isRequired(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::isRequired for optional argument";
      ArgSpec spec = ArgSpec::makeOptionalArg("opt", 1);
      VERIFY(!spec.isRequired(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::isRequired for flag argument";
      ArgSpec spec = ArgSpec::makeFlagArg("opt");
      VERIFY(!spec.isRequired(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::isRequired for empty spec";
      ArgSpec spec;
      VERIFY(!spec.isRequired(), caseLabel);
   }
}


void testArgSpecHasLabel()
{
   {
      const string caseLabel = "ArgSpec::hasLabel for positional argument";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(!spec.hasLabel(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::hasLabel for optional argument";
      ArgSpec spec = ArgSpec::makeOptionalArg("opt", 1);
      VERIFY(spec.hasLabel(), caseLabel);
   }
}


void testArgSpecLabel()
{
   {
      const string caseLabel = "ArgSpec::label";
      ArgSpec spec = ArgSpec::makeOptionalArg("opt", 1);
      VERIFY(spec.label() == "opt", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::label for no label";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(spec.label().empty(), caseLabel);
   }
}


void testArgSpecHelp()
{
   {
      const string caseLabel = "ArgSpec::help for empty object";
      ArgSpec spec;
      VERIFY(spec.help() == "", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help for required argument";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(spec.help().find("Required") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help for optional argument";
      ArgSpec spec = ArgSpec::makeOptionalArg("foo", 1);
      VERIFY(spec.help().find("Optional") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help contains label";
      ArgSpec spec = ArgSpec::makeOptionalArg("abcdefg", 1);
      VERIFY(spec.help().find("abcdefg") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help contains abbreviation";
      ArgSpec spec = ArgSpec::makeOptionalArg("abcdefg", 1, "xyz");
      VERIFY(spec.help().find("xyz") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help contains the number of values";
      ArgSpec spec = ArgSpec::makePositionalArg(10);
      VERIFY(spec.help().find("10 values") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help contains phrase: zero or more";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore);
      VERIFY(spec.help().find("zero or more") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help contains phrase: one or more";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);
      VERIFY(spec.help().find("one or more") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help contains description";
      ArgSpec spec = ArgSpec::makePositionalArg(1, "thisisthedescription");
      VERIFY(spec.help().find("thisisthedescription") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::help indenting";
      ArgSpec spec = ArgSpec::makePositionalArg(1, "thisisthedescription");
      VERIFY(spec.help("  ").find("Required") == 2, caseLabel);
   }
}


void testArgSpecMatchLabel()
{
   {
      const string caseLabel = "ArgSpec::matchLabel for matching label";
      ArgSpec spec = ArgSpec::makeFlagArg("abcdefg");
      VERIFY(spec.matchLabel("abcdefg"), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::matchLabel for matching abbreviation";
      ArgSpec spec = ArgSpec::makeFlagArg("abcdefg", "ab");
      VERIFY(spec.matchLabel("ab"), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::matchLabel for no match";
      ArgSpec spec = ArgSpec::makeFlagArg("abcdefg");
      VERIFY(!spec.matchLabel("cd"), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::matchLabel for different case";
      ArgSpec spec = ArgSpec::makeFlagArg("abcd");
      VERIFY(spec.matchLabel("ABCD"), caseLabel);
   }
}


void testArgSpecMatch()
{
   {
      const string caseLabel = "ArgSpec::match label for matching actual flag argument";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      vector<string> actualArgs{"-myflag"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "myflag", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match label for matching actual flag argument with double-dash";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      vector<string> actualArgs{"--myflag"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "myflag", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match label for not matching actual flag argument";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      vector<string> actualArgs{"-other"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "-other", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match label for no actual arguments";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      vector<string> actualArgs;
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match label for not optional actual argument";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      vector<string> actualArgs{"myflag"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "myflag", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match label for multiple actual arguments";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      vector<string> actualArgs{"-myflag", "-other", "-more"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "myflag", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(*argIter == "-other", caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match label for different actual arguments with different case";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      vector<string> actualArgs{"-myFlag"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "myflag", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match value";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      vector<string> actualArgs{"first"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 1 && matched.value().values[0] == "first",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match multiple values";
      ArgSpec spec = ArgSpec::makePositionalArg(3);
      vector<string> actualArgs{"first", "1", "other"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 3 && matched.value().values[0] == "first" &&
                matched.value().values[1] == "1" && matched.value().values[2] == "other",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match multiple values with more actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(3);
      vector<string> actualArgs{"first", "1", "other", "more1", "more2"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 3 && matched.value().values[0] == "first" &&
                matched.value().values[1] == "1" && matched.value().values[2] == "other",
             caseLabel);
      VERIFY(*argIter == "more1", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match value with labeled actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      vector<string> actualArgs{"-labeled"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "-labeled", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match value with no actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      vector<string> actualArgs;
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match zero or more values with zero actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore);
      vector<string> actualArgs;
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match zero or more values with multiple actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore);
      vector<string> actualArgs{"1", "2", "3", "4", "-flag"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 4 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "2" && matched.value().values[2] == "3" &&
                matched.value().values[3] == "4",
             caseLabel);
      VERIFY(*argIter == "-flag", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match one or more values with zero actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);
      vector<string> actualArgs;
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match one or more values with one actual arg";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);
      vector<string> actualArgs{"1"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 1 && matched.value().values[0] == "1",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match one or more values with multiple actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);
      vector<string> actualArgs{"1", "2", "3", "4", "-flag"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 4 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "2" && matched.value().values[2] == "3" &&
                matched.value().values[3] == "4",
             caseLabel);
      VERIFY(*argIter == "-flag", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match optional value";
      ArgSpec spec = ArgSpec::makeOptionalArg("test", 1);
      vector<string> actualArgs{"-test", "1"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "test", caseLabel);
      VERIFY(matched.value().values.size() == 1 && matched.value().values[0] == "1",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match multiple optional values";
      ArgSpec spec = ArgSpec::makeOptionalArg("label", 3);
      vector<string> actualArgs{"-label", "1", "two", "3"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "label", caseLabel);
      VERIFY(matched.value().values.size() == 3 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "two" && matched.value().values[2] == "3",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match multiple optional values with more actual args";
      ArgSpec spec = ArgSpec::makeOptionalArg("label", 3);
      vector<string> actualArgs{"-label", "1", "two", "3", "more", "more2"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "label", caseLabel);
      VERIFY(matched.value().values.size() == 3 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "two" && matched.value().values[2] == "3",
             caseLabel);
      VERIFY(*argIter == "more", caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match optional value with missing actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", 1);
      vector<string> actualArgs{"-mylabel"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "-mylabel", caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match optional value with no actual args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", 1);
      vector<string> actualArgs;
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel = "ArgSpec::match optional value with too few actual args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", 3);
      vector<string> actualArgs{"-mylabel", "1", "2"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "-mylabel", caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match zero or more optional values with zero actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::ZeroOrMore);
      vector<string> actualArgs{"-mylabel"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "mylabel", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match zero or more optional values with multiple actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::ZeroOrMore);
      vector<string> actualArgs{"-mylabel", "1", "2", "3", "4", "-flag"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "mylabel", caseLabel);
      VERIFY(matched.value().values.size() == 4 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "2" && matched.value().values[2] == "3" &&
                matched.value().values[3] == "4",
             caseLabel);
      VERIFY(*argIter == "-flag", caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match one or more optional values with zero actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::OneOrMore);
      vector<string> actualArgs;
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match one or more optional values with one actual value arg";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::OneOrMore);
      vector<string> actualArgs{"-mylabel", "1"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "mylabel", caseLabel);
      VERIFY(matched.value().values.size() == 1 && matched.value().values[0] == "1",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const string caseLabel =
         "ArgSpec::match one or more optional values with multiple actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::OneOrMore);
      vector<string> actualArgs{"-mylabel", "1", "2", "3", "4", "-flag"};
      vector<string>::const_iterator argIter = actualArgs.begin();

      const optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "mylabel", caseLabel);
      VERIFY(matched.value().values.size() == 4 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "2" && matched.value().values[2] == "3" &&
                matched.value().values[3] == "4",
             caseLabel);
      VERIFY(*argIter == "-flag", caseLabel);
   }
}


void testCmdSpecDefaultCtor()
{
   {
      const string caseLabel = "CmdSpec default ctor";
      CmdSpec spec;
      VERIFY(!spec, caseLabel);
      VERIFY(spec.name().empty(), caseLabel);
      VERIFY(spec.description().empty(), caseLabel);
      VERIFY(spec.help().empty(), caseLabel);
   }
}


void testCmdSpecConversionToBool()
{
   {
      const string caseLabel = "CmdSpec conversion to bool for populated spec";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.operator bool(), caseLabel);
   }
   {
      const string caseLabel = "CmdSpec conversion to bool for empty spec";
      CmdSpec spec;
      VERIFY(!spec.operator bool(), caseLabel);
   }
}


void testCmdSpecMatchesName()
{
   {
      const string caseLabel = "CmdSpec::matchesName for matching name";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.matchesName("name"), caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::matchesName for matching abbreviation";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.matchesName("n"), caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::matchesName for no match";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(!spec.matchesName("other"), caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::matchesName for empty object";
      CmdSpec spec;
      VERIFY(!spec.matchesName("other"), caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::matchesName for matching empty string";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(!spec.matchesName(""), caseLabel);
   }
   {
      const string caseLabel =
         "CmdSpec::matchesName for empty object matching empty string";
      CmdSpec spec;
      VERIFY(spec.matchesName(""), caseLabel);
   }
}


void testCmdSpecName()
{
   {
      const string caseLabel = "CmdSpec::name for named spec";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.name() == "name", caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::name for empty spec";
      CmdSpec spec;
      VERIFY(spec.name().empty(), caseLabel);
   }
}


void testCmdSpecDescription()
{
   {
      const string caseLabel = "CmdSpec::description for spec with description";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.description() == "descr", caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::description for empty spec";
      CmdSpec spec;
      VERIFY(spec.description().empty(), caseLabel);
   }
}


void testCmdSpecHelp()
{
   {
      const string caseLabel = "CmdSpec::help for empty object";
      CmdSpec spec;
      VERIFY(spec.help().empty(), caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::help contains name";
      CmdSpec spec{"searchablename", "n", "descr", {}, "notes"};
      VERIFY(spec.help().find("searchablename") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::help contains abbreviation";
      CmdSpec spec{"name", "qq", "descr", {}, "notes"};
      VERIFY(spec.help().find("qq") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::help contains description";
      CmdSpec spec{"name", "n", "searchableDescription", {}, "notes"};
      VERIFY(spec.help().find("searchableDescription") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::help contains arguments";
      CmdSpec spec{"name", "n", "descr", {ArgSpec::makeFlagArg("-myflag")}, "notes"};
      VERIFY(spec.help().find("myflag") != string::npos, caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::help contains notes";
      CmdSpec spec{"name", "n", "descr", {}, "searchableNotes"};
      VERIFY(spec.help().find("searchableNotes") != string::npos, caseLabel);
   }
}


void testCmdSpecHasArgSpec()
{
   {
      const string caseLabel = "CmdSpec::hasArgSpec for existing arg spec";
      CmdSpec spec{"name", "n", "descr", {ArgSpec::makeFlagArg("myflag")}, "notes"};
      VERIFY(spec.hasArgSpec("myflag"), caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::hasArgSpec for not existing arg spec";
      CmdSpec spec{"name",
                   "n",
                   "descr",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makeFlagArg("other")},
                   "notes"};
      VERIFY(!spec.hasArgSpec("myflag"), caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::hasArgSpec for no arg specs";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(!spec.hasArgSpec("myflag"), caseLabel);
   }
}


void testCmdSpecBegin()
{
   {
      const string caseLabel =
         "CmdSpec::begin for multiple arg specs with required arg first";
      CmdSpec spec{"name",
                   "n",
                   "descr",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makeFlagArg("other")},
                   "notes"};
      VERIFY(spec.begin()->isRequired(), caseLabel);
   }
   {
      const string caseLabel =
         "CmdSpec::begin for multiple arg specs with optional arg first";
      CmdSpec spec{"name",
                   "n",
                   "descr",
                   {ArgSpec::makeFlagArg("first"), ArgSpec::makeFlagArg("other")},
                   "notes"};
      VERIFY(spec.begin()->label() == "first", caseLabel);
   }
   {
      const string caseLabel = "CmdSpec::begin for no arg specs";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.begin() == spec.end(), caseLabel);
   }
}


void testCmdSpecEnd()
{
   // No meaningful test.
}


void testCmdSpecMatch()
{
   {
   }
}

} // namespace


void testCmdSpec()
{
   testLabelWithAbbrevCtor();
   testLabelWithAbbrevConversionToBool();
   testLabelWithAbbrevMatches();
   testLabelWithAbbrevLabel();
   testLabelWithAbbrevHaveAbbreviation();
   testLabelWithAbbrevAbbreviation();

   testArgSpecDefaultCtor();
   testArgSpecMakePositionalArg();
   testArgSpecMakeOptionalArg();
   testArgSpecMakeFlagArg();
   testArgSpecConversionToBool();
   testArgSpecIsRequired();
   testArgSpecHasLabel();
   testArgSpecLabel();
   testArgSpecHelp();
   testArgSpecMatchLabel();
   testArgSpecMatch();

   testCmdSpecDefaultCtor();
   testCmdSpecConversionToBool();
   testCmdSpecMatchesName();
   testCmdSpecName();
   testCmdSpecDescription();
   testCmdSpecHelp();
   testCmdSpecHasArgSpec();
   testCmdSpecBegin();
   testCmdSpecEnd();
   testCmdSpecMatch();
}
