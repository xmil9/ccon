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


namespace
{
///////////////////

void testLabelWithAbbrevCtor()
{
   {
      const std::string caseLabel = "LabelWithAbbrev ctor with label and abbreviation";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(label.label() == "test", caseLabel);
      VERIFY(label.abbreviation() == "t", caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev ctor with label only";
      LabelWithAbbrev label{"test", ""};
      VERIFY(label.label() == "test", caseLabel);
      VERIFY(label.abbreviation() == "", caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev ctor with abbreviation only";
      LabelWithAbbrev label{"", "t"};
      // Abbreviation is used as label.
      VERIFY(label.label() == "t", caseLabel);
      VERIFY(label.abbreviation() == "", caseLabel);
   }
}


void testLabelWithAbbrevConversionToBool()
{
   {
      const std::string caseLabel =
         "LabelWithAbbrev conversion to bool for label and abbreviation";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(label.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev conversion to bool for label only";
      LabelWithAbbrev label{"test", ""};
      VERIFY(label.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel =
         "LabelWithAbbrev conversion to bool for abbreviation only";
      LabelWithAbbrev label{"", "t"};
      VERIFY(label.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev conversion to bool for empty object";
      LabelWithAbbrev label;
      VERIFY(!label.operator bool(), caseLabel);
   }
}


void testLabelWithAbbrevMatches()
{
   {
      const std::string caseLabel = "LabelWithAbbrev::matches for matching label";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(label.matches("test"), caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev::matches for matching abbreviation";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(label.matches("t"), caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev::matches for no match";
      LabelWithAbbrev label{"test", "t"};
      VERIFY(!label.matches("abc"), caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev::matches for empty object";
      LabelWithAbbrev label;
      VERIFY(!label.matches("abc"), caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev::matches for matching empty string";
      LabelWithAbbrev label{"abc", "a"};
      VERIFY(!label.matches(""), caseLabel);
   }
   {
      const std::string caseLabel =
         "LabelWithAbbrev::matches for empty object matching empty string";
      LabelWithAbbrev label;
      VERIFY(label.matches(""), caseLabel);
   }
}


void testLabelWithAbbrevLabel()
{
   {
      const std::string caseLabel = "LabelWithAbbrev::label for populated label";
      LabelWithAbbrev label{"abc", "a"};
      VERIFY(label.label() == "abc", caseLabel);
   }
   {
      const std::string caseLabel = "LabelWithAbbrev::label for empty label";
      LabelWithAbbrev label;
      VERIFY(label.label() == "", caseLabel);
   }
}


void testLabelWithAbbrevHaveAbbreviation()
{
   {
      const std::string caseLabel =
         "LabelWithAbbrev::haveAbbreviation for populated abbreviation";
      LabelWithAbbrev label{"abc", "a"};
      VERIFY(label.haveAbbreviation(), caseLabel);
   }
   {
      const std::string caseLabel =
         "LabelWithAbbrev::haveAbbreviation for empty abbreviation";
      LabelWithAbbrev label{"abc", ""};
      VERIFY(!label.haveAbbreviation(), caseLabel);
   }
}


void testLabelWithAbbrevAbbreviation()
{
   {
      const std::string caseLabel =
         "LabelWithAbbrev::haveAbbreviation for populated abbreviation";
      LabelWithAbbrev label{"abc", "a"};
      VERIFY(label.abbreviation() == "a", caseLabel);
   }
   {
      const std::string caseLabel =
         "LabelWithAbbrev::haveAbbreviation for empty abbreviation";
      LabelWithAbbrev label{"abc", ""};
      VERIFY(label.abbreviation() == "", caseLabel);
   }
}


///////////////////

void testArgSpecDefaultCtor()
{
   {
      const std::string caseLabel = "ArgSpec default ctor";
      ArgSpec spec;
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
   }
}


void testArgSpecMakePositionalArg()
{
   {
      const std::string caseLabel = "ArgSpec::makePositionalArg for one value";

      ArgSpec spec = ArgSpec::makePositionalArg(1);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("1 value") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makePositionalArg for mutiple values";

      ArgSpec spec = ArgSpec::makePositionalArg(5);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("5 values") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makePositionalArg for zero or more values";

      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("zero or more") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makePositionalArg for one or more values";

      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("one or more") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::makePositionalArg for zero values. This is an empty spec object.";

      ArgSpec spec = ArgSpec::makePositionalArg(0);

      VERIFY(!spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help == "", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makePositionalArg with description";

      const std::string descr = "test_description";
      ArgSpec spec = ArgSpec::makePositionalArg(1, descr);

      const std::string help = spec.help();
      VERIFY(help.find(descr) != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makePositionalArg without description";

      ArgSpec spec = ArgSpec::makePositionalArg(5);

      const std::string help = spec.help();
      VERIFY(help.find("--") == std::string::npos, caseLabel);
   }
}


void testArgSpecMakeOptionalArg()
{
   {
      const std::string caseLabel = "ArgSpec::makeOptionalArg for one value";

      const std::string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("1 value") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeOptionalArg for multiple values";

      const std::string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 5);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("5 values") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeOptionalArg for zore or more values";

      const std::string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, ArgSpec::ZeroOrMore);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("zero or more") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeOptionalArg for one or more values";

      const std::string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, ArgSpec::OneOrMore);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("one or more") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::makeOptionalArg for zero values. This is a flag argument.";

      const std::string label = "first_arg";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 0);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("value") == std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeOptionalArg with uppercase label";

      const std::string label = "UPPER";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1);

      VERIFY(spec.label() == "upper", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeOptionalArg with abbreviation";

      const std::string label = "first_arg";
      const std::string abbrev = "qq";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1, abbrev);

      const std::string help = spec.help();
      VERIFY(help.find(abbrev) != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::makeOptionalArg with uppercase abbreviation";

      const std::string label = "first_arg";
      const std::string abbrev = "QQ";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1, abbrev);

      const std::string help = spec.help();
      VERIFY(help.find("qq") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeOptionalArg with description";

      const std::string label = "first_arg";
      const std::string descr = "test_description";
      ArgSpec spec = ArgSpec::makeOptionalArg(label, 1, "", descr);

      const std::string help = spec.help();
      VERIFY(help.find(descr) != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::makeOptionalArg without label. This is a positional argument.";

      ArgSpec spec = ArgSpec::makeOptionalArg("", 1);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("value") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::makeOptionalArg without label but abbreviation. "
         "The abbreviation becomes the label.";

      const std::string abbrev = "qq";
      ArgSpec spec = ArgSpec::makeOptionalArg("", 1, abbrev);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == abbrev, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("value") != std::string::npos, caseLabel);
      VERIFY(help.find(abbrev) != std::string::npos, caseLabel);
   }
}


void testArgSpecMakeFlagArg()
{
   {
      const std::string caseLabel = "ArgSpec::makeFlagArg";

      const std::string label = "first_arg";
      ArgSpec spec = ArgSpec::makeFlagArg(label);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == label, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find("value") == std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeFlagArg with uppercase label";

      const std::string label = "UPPER";
      ArgSpec spec = ArgSpec::makeFlagArg(label);

      VERIFY(spec.label() == "upper", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeFlagArg with abbreviation";

      const std::string label = "first_arg";
      const std::string abbrev = "qq";
      ArgSpec spec = ArgSpec::makeFlagArg(label, abbrev);

      const std::string help = spec.help();
      VERIFY(help.find(abbrev) != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeFlagArg with uppercase abbreviation";

      const std::string label = "first_arg";
      const std::string abbrev = "QQ";
      ArgSpec spec = ArgSpec::makeFlagArg(label, abbrev);

      const std::string help = spec.help();
      VERIFY(help.find("qq") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::makeFlagArg with description";

      const std::string label = "first_arg";
      const std::string descr = "test_description";
      ArgSpec spec = ArgSpec::makeFlagArg(label, "", descr);

      const std::string help = spec.help();
      VERIFY(help.find(descr) != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::makeFlagArg without label. This is an empty spec object.";

      ArgSpec spec = ArgSpec::makeFlagArg("");

      VERIFY(!spec.operator bool(), caseLabel);
      VERIFY(!spec.hasLabel(), caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      VERIFY(spec.help().empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::makeFlagArg without label but abbreviation. "
         "The abbreviation becomes the label.";

      const std::string abbrev = "qq";
      ArgSpec spec = ArgSpec::makeFlagArg("", abbrev);

      VERIFY(spec.operator bool(), caseLabel);
      VERIFY(spec.label() == abbrev, caseLabel);
      VERIFY(!spec.isRequired(), caseLabel);
      const std::string help = spec.help();
      VERIFY(help.find(abbrev) != std::string::npos, caseLabel);
   }
}


void testArgSpecConversionToBool()
{
   {
      const std::string caseLabel = "ArgSpec conversion to bool for empty object";
      ArgSpec spec;
      VERIFY(!spec.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec conversion to bool for empty object";
      ArgSpec spec;
      VERIFY(!spec.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec conversion to bool for spec with value";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(spec.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec conversion to bool for spec with label";
      ArgSpec spec = ArgSpec::makeFlagArg("label");
      VERIFY(spec.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec conversion to bool for spec with description only";
      ArgSpec spec = ArgSpec::makeFlagArg("", "", "descr");
      VERIFY(!spec.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec conversion to bool for spec with abbreviation only";
      ArgSpec spec = ArgSpec::makeFlagArg("", "abbreviation");
      // The abbreviation becomes the label.
      VERIFY(spec.operator bool(), caseLabel);
   }
}


void testArgSpecIsRequired()
{
   {
      const std::string caseLabel = "ArgSpec::isRequired for positional argument";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(spec.isRequired(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::isRequired for optional argument";
      ArgSpec spec = ArgSpec::makeOptionalArg("opt", 1);
      VERIFY(!spec.isRequired(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::isRequired for flag argument";
      ArgSpec spec = ArgSpec::makeFlagArg("opt");
      VERIFY(!spec.isRequired(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::isRequired for empty spec";
      ArgSpec spec;
      VERIFY(!spec.isRequired(), caseLabel);
   }
}


void testArgSpecHasLabel()
{
   {
      const std::string caseLabel = "ArgSpec::hasLabel for positional argument";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(!spec.hasLabel(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::hasLabel for optional argument";
      ArgSpec spec = ArgSpec::makeOptionalArg("opt", 1);
      VERIFY(spec.hasLabel(), caseLabel);
   }
}


void testArgSpecLabel()
{
   {
      const std::string caseLabel = "ArgSpec::label";
      ArgSpec spec = ArgSpec::makeOptionalArg("opt", 1);
      VERIFY(spec.label() == "opt", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::label for no label";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(spec.label().empty(), caseLabel);
   }
}


void testArgSpecHelp()
{
   {
      const std::string caseLabel = "ArgSpec::help for empty object";
      ArgSpec spec;
      VERIFY(spec.help() == "", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help for required argument";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      VERIFY(spec.help().find("Required") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help for optional argument";
      ArgSpec spec = ArgSpec::makeOptionalArg("foo", 1);
      VERIFY(spec.help().find("Optional") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help contains label";
      ArgSpec spec = ArgSpec::makeOptionalArg("abcdefg", 1);
      VERIFY(spec.help().find("abcdefg") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help contains abbreviation";
      ArgSpec spec = ArgSpec::makeOptionalArg("abcdefg", 1, "xyz");
      VERIFY(spec.help().find("xyz") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help contains the number of values";
      ArgSpec spec = ArgSpec::makePositionalArg(10);
      VERIFY(spec.help().find("10 values") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help contains phrase: zero or more";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore);
      VERIFY(spec.help().find("zero or more") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help contains phrase: one or more";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);
      VERIFY(spec.help().find("one or more") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help contains description";
      ArgSpec spec = ArgSpec::makePositionalArg(1, "thisisthedescription");
      VERIFY(spec.help().find("thisisthedescription") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::help indenting";
      ArgSpec spec = ArgSpec::makePositionalArg(1, "thisisthedescription");
      VERIFY(spec.help("  ").find("Required") == 2, caseLabel);
   }
}


void testArgSpecMatchLabel()
{
   {
      const std::string caseLabel = "ArgSpec::matchLabel for matching label";
      ArgSpec spec = ArgSpec::makeFlagArg("abcdefg");
      VERIFY(spec.matchLabel("abcdefg"), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::matchLabel for matching abbreviation";
      ArgSpec spec = ArgSpec::makeFlagArg("abcdefg", "ab");
      VERIFY(spec.matchLabel("ab"), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::matchLabel for no match";
      ArgSpec spec = ArgSpec::makeFlagArg("abcdefg");
      VERIFY(!spec.matchLabel("cd"), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::matchLabel for different case";
      ArgSpec spec = ArgSpec::makeFlagArg("abcd");
      VERIFY(spec.matchLabel("ABCD"), caseLabel);
   }
}


void testArgSpecMatch()
{
   {
      const std::string caseLabel =
         "ArgSpec::match label for matching actual flag argument";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      std::vector<std::string> actualArgs{"-myflag"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "myflag", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match label for matching actual flag argument with double-dash";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      std::vector<std::string> actualArgs{"--myflag"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "myflag", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match label for not matching actual flag argument";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      std::vector<std::string> actualArgs{"-other"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "-other", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match label for no actual arguments";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      std::vector<std::string> actualArgs;
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match label for not optional actual argument";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      std::vector<std::string> actualArgs{"myflag"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "myflag", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match label for multiple actual arguments";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      std::vector<std::string> actualArgs{"-myflag", "-other", "-more"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "myflag", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(*argIter == "-other", caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match label for different actual arguments with different case";
      ArgSpec spec = ArgSpec::makeFlagArg("myflag");
      std::vector<std::string> actualArgs{"-myFlag"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "myflag", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match value";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      std::vector<std::string> actualArgs{"first"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 1 && matched.value().values[0] == "first",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match multiple values";
      ArgSpec spec = ArgSpec::makePositionalArg(3);
      std::vector<std::string> actualArgs{"first", "1", "other"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 3 && matched.value().values[0] == "first" &&
                matched.value().values[1] == "1" && matched.value().values[2] == "other",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match multiple values with more actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(3);
      std::vector<std::string> actualArgs{"first", "1", "other", "more1", "more2"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 3 && matched.value().values[0] == "first" &&
                matched.value().values[1] == "1" && matched.value().values[2] == "other",
             caseLabel);
      VERIFY(*argIter == "more1", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match value with labeled actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      std::vector<std::string> actualArgs{"-labeled"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "-labeled", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match value with no actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(1);
      std::vector<std::string> actualArgs;
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match zero or more values with zero actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore);
      std::vector<std::string> actualArgs;
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match zero or more values with multiple actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore);
      std::vector<std::string> actualArgs{"1", "2", "3", "4", "-flag"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 4 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "2" && matched.value().values[2] == "3" &&
                matched.value().values[3] == "4",
             caseLabel);
      VERIFY(*argIter == "-flag", caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match one or more values with zero actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);
      std::vector<std::string> actualArgs;
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match one or more values with one actual arg";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);
      std::vector<std::string> actualArgs{"1"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 1 && matched.value().values[0] == "1",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match one or more values with multiple actual args";
      ArgSpec spec = ArgSpec::makePositionalArg(ArgSpec::OneOrMore);
      std::vector<std::string> actualArgs{"1", "2", "3", "4", "-flag"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label.empty(), caseLabel);
      VERIFY(matched.value().values.size() == 4 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "2" && matched.value().values[2] == "3" &&
                matched.value().values[3] == "4",
             caseLabel);
      VERIFY(*argIter == "-flag", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match optional value";
      ArgSpec spec = ArgSpec::makeOptionalArg("test", 1);
      std::vector<std::string> actualArgs{"-test", "1"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "test", caseLabel);
      VERIFY(matched.value().values.size() == 1 && matched.value().values[0] == "1",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match multiple optional values";
      ArgSpec spec = ArgSpec::makeOptionalArg("label", 3);
      std::vector<std::string> actualArgs{"-label", "1", "two", "3"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "label", caseLabel);
      VERIFY(matched.value().values.size() == 3 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "two" && matched.value().values[2] == "3",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match multiple optional values with more actual args";
      ArgSpec spec = ArgSpec::makeOptionalArg("label", 3);
      std::vector<std::string> actualArgs{"-label", "1", "two", "3", "more", "more2"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "label", caseLabel);
      VERIFY(matched.value().values.size() == 3 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "two" && matched.value().values[2] == "3",
             caseLabel);
      VERIFY(*argIter == "more", caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match optional value with missing actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", 1);
      std::vector<std::string> actualArgs{"-mylabel"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "-mylabel", caseLabel);
   }
   {
      const std::string caseLabel = "ArgSpec::match optional value with no actual args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", 1);
      std::vector<std::string> actualArgs;
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match optional value with too few actual args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", 3);
      std::vector<std::string> actualArgs{"-mylabel", "1", "2"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(*argIter == "-mylabel", caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match zero or more optional values with zero actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::ZeroOrMore);
      std::vector<std::string> actualArgs{"-mylabel"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "mylabel", caseLabel);
      VERIFY(matched.value().values.empty(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match zero or more optional values with multiple actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::ZeroOrMore);
      std::vector<std::string> actualArgs{"-mylabel", "1", "2", "3", "4", "-flag"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "mylabel", caseLabel);
      VERIFY(matched.value().values.size() == 4 && matched.value().values[0] == "1" &&
                matched.value().values[1] == "2" && matched.value().values[2] == "3" &&
                matched.value().values[3] == "4",
             caseLabel);
      VERIFY(*argIter == "-flag", caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match one or more optional values with zero actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::OneOrMore);
      std::vector<std::string> actualArgs;
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(!matched.has_value(), caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match one or more optional values with one actual value arg";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::OneOrMore);
      std::vector<std::string> actualArgs{"-mylabel", "1"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

      VERIFY(matched.has_value(), caseLabel);
      VERIFY(matched.value().label == "mylabel", caseLabel);
      VERIFY(matched.value().values.size() == 1 && matched.value().values[0] == "1",
             caseLabel);
      VERIFY(argIter == actualArgs.end(), caseLabel);
   }
   {
      const std::string caseLabel =
         "ArgSpec::match one or more optional values with multiple actual value args";
      ArgSpec spec = ArgSpec::makeOptionalArg("mylabel", ArgSpec::OneOrMore);
      std::vector<std::string> actualArgs{"-mylabel", "1", "2", "3", "4", "-flag"};
      std::vector<std::string>::const_iterator argIter = actualArgs.begin();

      const std::optional<VerifiedArg> matched = spec.match(argIter, actualArgs.end());

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
      const std::string caseLabel = "CmdSpec default ctor";
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
      const std::string caseLabel = "CmdSpec conversion to bool for populated spec";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec conversion to bool for empty spec";
      CmdSpec spec;
      VERIFY(!spec.operator bool(), caseLabel);
   }
}


void testCmdSpecMatchesName()
{
   {
      const std::string caseLabel = "CmdSpec::matchesName for matching name";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.matchesName("name"), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::matchesName for matching abbreviation";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.matchesName("n"), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::matchesName for no match";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(!spec.matchesName("other"), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::matchesName for empty object";
      CmdSpec spec;
      VERIFY(!spec.matchesName("other"), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::matchesName for matching empty string";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(!spec.matchesName(""), caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::matchesName for empty object matching empty string";
      CmdSpec spec;
      VERIFY(spec.matchesName(""), caseLabel);
   }
}


void testCmdSpecName()
{
   {
      const std::string caseLabel = "CmdSpec::name for named spec";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.name() == "name", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::name for empty spec";
      CmdSpec spec;
      VERIFY(spec.name().empty(), caseLabel);
   }
}


void testCmdSpecDescription()
{
   {
      const std::string caseLabel = "CmdSpec::description for spec with description";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(spec.description() == "descr", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::description for empty spec";
      CmdSpec spec;
      VERIFY(spec.description().empty(), caseLabel);
   }
}


void testCmdSpecHelp()
{
   {
      const std::string caseLabel = "CmdSpec::help for empty object";
      CmdSpec spec;
      VERIFY(spec.help().empty(), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::help contains name";
      CmdSpec spec{"searchablename", "n", "descr", {}, "notes"};
      VERIFY(spec.help().find("searchablename") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::help contains abbreviation";
      CmdSpec spec{"name", "qq", "descr", {}, "notes"};
      VERIFY(spec.help().find("qq") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::help contains description";
      CmdSpec spec{"name", "n", "searchableDescription", {}, "notes"};
      VERIFY(spec.help().find("searchableDescription") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::help contains arguments";
      CmdSpec spec{"name", "n", "descr", {ArgSpec::makeFlagArg("-myflag")}, "notes"};
      VERIFY(spec.help().find("myflag") != std::string::npos, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::help contains notes";
      CmdSpec spec{"name", "n", "descr", {}, "searchableNotes"};
      VERIFY(spec.help().find("searchableNotes") != std::string::npos, caseLabel);
   }
}


void testCmdSpecHasArgSpec()
{
   {
      const std::string caseLabel = "CmdSpec::hasArgSpec for existing arg spec";
      CmdSpec spec{"name", "n", "descr", {ArgSpec::makeFlagArg("myflag")}, "notes"};
      VERIFY(spec.hasArgSpec("myflag"), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::hasArgSpec for not existing arg spec";
      CmdSpec spec{"name",
                   "n",
                   "descr",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makeFlagArg("other")},
                   "notes"};
      VERIFY(!spec.hasArgSpec("myflag"), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::hasArgSpec for no arg specs";
      CmdSpec spec{"name", "n", "descr", {}, "notes"};
      VERIFY(!spec.hasArgSpec("myflag"), caseLabel);
   }
}


void testCmdSpecBegin()
{
   {
      const std::string caseLabel =
         "CmdSpec::begin for multiple arg specs with required arg first";
      CmdSpec spec{"name",
                   "n",
                   "descr",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makeFlagArg("other")},
                   "notes"};
      VERIFY(spec.begin()->isRequired(), caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::begin for multiple arg specs with optional arg first";
      CmdSpec spec{"name",
                   "n",
                   "descr",
                   {ArgSpec::makeFlagArg("first"), ArgSpec::makeFlagArg("other")},
                   "notes"};
      VERIFY(spec.begin()->label() == "first", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::begin for no arg specs";
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
      const std::string caseLabel = "CmdSpec::match - no match for empty spec";
      CmdSpec spec;
      VERIFY(!spec.match("help").isMatching, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match - no match for empty command";
      CmdSpec spec{"test", "t", "run a test", {}, ""};
      VERIFY(!spec.match("").isMatching, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match command name";
      CmdSpec spec{"test", "t", "run a test", {}, ""};

      CmdSpec::Match res = spec.match("test");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match - fail for incorrect command name";
      CmdSpec spec{"test", "t", "run a test", {}, ""};

      CmdSpec::Match res = spec.match("toast");

      VERIFY(!res.isMatching, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match command abbreviation";
      CmdSpec spec{"test", "t", "run a test", {}, ""};

      CmdSpec::Match res = spec.match("t");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match command name with different case";
      CmdSpec spec{"test", "t", "run a test", {}, ""};

      CmdSpec::Match res = spec.match("TEST");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match help argument";
      CmdSpec spec{"test", "t", "run a test", {}, ""};

      CmdSpec::Match res = spec.match("test -help");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
      VERIFY(res.matchedCmd.args.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[0].label == "help", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match help argument abbreviation";
      CmdSpec spec{"test", "t", "run a test", {}, ""};

      CmdSpec::Match res = spec.match("test -?");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
      VERIFY(res.matchedCmd.args.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[0].label == "help", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match positional args";
      CmdSpec spec{"test", "t", "run a test", {ArgSpec::makePositionalArg(2)}, ""};

      CmdSpec::Match res = spec.match("test abc 2");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
      VERIFY(res.matchedCmd.args.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[0].values.size() == 2, caseLabel);
      VERIFY(res.matchedCmd.args[0].values[0] == "abc", caseLabel);
      VERIFY(res.matchedCmd.args[0].values[1] == "2", caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match positional args specified in two arg specs";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(1), ArgSpec::makePositionalArg(1)},
                   ""};

      CmdSpec::Match res = spec.match("test abc 2");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
      VERIFY(res.matchedCmd.args.size() == 2, caseLabel);
      VERIFY(res.matchedCmd.args[0].values.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[0].values[0] == "abc", caseLabel);
      VERIFY(res.matchedCmd.args[1].values[0] == "2", caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match - fail for too few positional args";
      CmdSpec spec{"test", "t", "run a test", {ArgSpec::makePositionalArg(2)}, ""};

      CmdSpec::Match res = spec.match("test abc");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match - fail for too many positional args";
      CmdSpec spec{"test", "t", "run a test", {ArgSpec::makePositionalArg(2)}, ""};

      CmdSpec::Match res = spec.match("test abc 2 3");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test -size 10 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
      VERIFY(res.matchedCmd.args.size() == 2, caseLabel);
      VERIFY(res.matchedCmd.args[0].label == "size", caseLabel);
      VERIFY(res.matchedCmd.args[0].values.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[0].values[0] == "10", caseLabel);
      VERIFY(res.matchedCmd.args[1].label == "scan", caseLabel);
      VERIFY(res.matchedCmd.args[1].values.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match missing optional arg";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
      VERIFY(res.matchedCmd.args.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[0].label == "scan", caseLabel);
      VERIFY(res.matchedCmd.args[0].values.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match all optional args missing";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
      VERIFY(res.matchedCmd.args.empty(), caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match - fail for too many optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test -size 10 -scan -other");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
   }
   {
      const std::string caseLabel = "CmdSpec::match positional and optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makePositionalArg(1),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test abc 2 4.5 -size 10 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.name == "test", caseLabel);
      VERIFY(res.matchedCmd.args.size() == 4, caseLabel);
      VERIFY(res.matchedCmd.args[0].values.size() == 2, caseLabel);
      VERIFY(res.matchedCmd.args[0].values[0] == "abc", caseLabel);
      VERIFY(res.matchedCmd.args[0].values[1] == "2", caseLabel);
      VERIFY(res.matchedCmd.args[1].values.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[1].values[0] == "4.5", caseLabel);
      VERIFY(res.matchedCmd.args[2].label == "size", caseLabel);
      VERIFY(res.matchedCmd.args[2].values.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[2].values[0] == "10", caseLabel);
      VERIFY(res.matchedCmd.args[3].label == "scan", caseLabel);
      VERIFY(res.matchedCmd.args[3].values.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match - fail missing positional arg for mix of "
         "positional and optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makePositionalArg(1),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test abc 2 -size 10 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match - fail for too many positional args for mix of "
         "positional and optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makePositionalArg(1),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test abc 2 3 4 -size 10 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match missing optional arg for mix of positional and optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makePositionalArg(1),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test abc 2 3 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.args.size() == 3, caseLabel);
      VERIFY(res.matchedCmd.args[0].values.size() == 2, caseLabel);
      VERIFY(res.matchedCmd.args[0].values[0] == "abc", caseLabel);
      VERIFY(res.matchedCmd.args[0].values[1] == "2", caseLabel);
      VERIFY(res.matchedCmd.args[1].values.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[1].values[0] == "3", caseLabel);
      VERIFY(res.matchedCmd.args[2].label == "scan", caseLabel);
      VERIFY(res.matchedCmd.args[2].values.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match - fail for too many optional args for mix of "
         "positional and optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(2), ArgSpec::makePositionalArg(1),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test abc 2 3 -size 10 -scan -other");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match zero or more positional arg values for mix of "
         "positional and optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test 1 2 3 4 -size 10 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.args.size() == 3, caseLabel);
      VERIFY(res.matchedCmd.args[0].values.size() == 4, caseLabel);
      VERIFY(res.matchedCmd.args[1].label == "size", caseLabel);
      VERIFY(res.matchedCmd.args[1].values.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[1].values[0] == "10", caseLabel);
      VERIFY(res.matchedCmd.args[2].label == "scan", caseLabel);
      VERIFY(res.matchedCmd.args[2].values.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match zero positional arg values when zero or more are specified";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test -size 10 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.args.size() == 3, caseLabel);
      VERIFY(res.matchedCmd.args[0].values.empty(), caseLabel);
      VERIFY(res.matchedCmd.args[1].label == "size", caseLabel);
      VERIFY(res.matchedCmd.args[1].values.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[1].values[0] == "10", caseLabel);
      VERIFY(res.matchedCmd.args[2].label == "scan", caseLabel);
      VERIFY(res.matchedCmd.args[2].values.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match - fail for zero or more positional arg "
         "followed by another positional arg";
      CmdSpec spec{
         "test",
         "t",
         "run a test",
         {ArgSpec::makePositionalArg(ArgSpec::ZeroOrMore), ArgSpec::makePositionalArg(2)},
         ""};

      CmdSpec::Match res = spec.match("test 1 2 3 4");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match one or more positional arg values for mix of "
         "positional and optional args";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(ArgSpec::OneOrMore),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test 1 2 3 4 -size 10 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(res.areArgsValid, caseLabel);
      VERIFY(res.matchedCmd.args.size() == 3, caseLabel);
      VERIFY(res.matchedCmd.args[0].values.size() == 4, caseLabel);
      VERIFY(res.matchedCmd.args[1].label == "size", caseLabel);
      VERIFY(res.matchedCmd.args[1].values.size() == 1, caseLabel);
      VERIFY(res.matchedCmd.args[1].values[0] == "10", caseLabel);
      VERIFY(res.matchedCmd.args[2].label == "scan", caseLabel);
      VERIFY(res.matchedCmd.args[2].values.empty(), caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match - fail for zero positional arg values "
         "when one or more are specified";
      CmdSpec spec{"test",
                   "t",
                   "run a test",
                   {ArgSpec::makePositionalArg(ArgSpec::OneOrMore),
                    ArgSpec::makeOptionalArg("size", 1), ArgSpec::makeFlagArg("scan")},
                   ""};

      CmdSpec::Match res = spec.match("test -size 10 -scan");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
   }
   {
      const std::string caseLabel =
         "CmdSpec::match - fail for one or more positional arg "
         "followed by another positional arg";
      CmdSpec spec{
         "test",
         "t",
         "run a test",
         {ArgSpec::makePositionalArg(ArgSpec::OneOrMore), ArgSpec::makePositionalArg(2)},
         ""};

      CmdSpec::Match res = spec.match("test 1 2 3 4");

      VERIFY(res.isMatching, caseLabel);
      VERIFY(!res.areArgsValid, caseLabel);
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
