//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "cmd.h"
#include "cmd_spec.h"
#include "console_util.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <cassert>

using namespace std;
using namespace sutil;


namespace
{
///////////////////

// Checks if a passed string is an argument label.
bool isArgLabel(const std::string& val)
{
   if (val.empty())
      return false;

   auto charIter = std::begin(val);
   auto charEnd = std::end(val);
   while (charIter != charEnd && *charIter == ' ')
      ++charIter;

   std::size_t numDashes = 0;
   while (charIter != charEnd && *charIter == '-')
   {
      ++numDashes;
      ++charIter;
   }

   return numDashes == 1 || numDashes == 2;
}

} // namespace


namespace ccon
{
///////////////////

LabelWithAbbrev::LabelWithAbbrev(const std::string& label, const std::string& abbrev)
: m_label{label}, m_abbrev{abbrev}
{
   const bool haveOnlyAbbrev = (m_label.empty() && !m_abbrev.empty());
   if (haveOnlyAbbrev)
      std::swap(m_label, m_abbrev);
}


LabelWithAbbrev::operator bool() const
{
   return !m_label.empty();
}


bool LabelWithAbbrev::matches(const std::string& match) const
{
   if (match.empty())
      return m_label.empty();
   return (match == m_label || match == m_abbrev);
}


std::string LabelWithAbbrev::label() const
{
   return m_label;
}


bool LabelWithAbbrev::haveAbbreviation() const
{
   return !m_abbrev.empty();
}


std::string LabelWithAbbrev::abbreviation() const
{
   return m_abbrev;
}


///////////////////

ArgSpec ArgSpec::makePositionalArg(std::size_t numValues,
                                    const std::string& description)
{
   return ArgSpec("", "", numValues, description);
}


ArgSpec ArgSpec::makeOptionalArg(const std::string& label, std::size_t numValues,
                                 const std::string& abbrev,
                                 const std::string& description)
{
   return ArgSpec(label, abbrev, numValues, description);
}


ArgSpec ArgSpec::makeFlagArg(const std::string& label, const std::string& abbrev,
                              const std::string& description)
{
   return ArgSpec(label, abbrev, 0, description);
}


ArgSpec::ArgSpec(const std::string& label, const std::string& abbrev,
                 std::size_t numValues, const std::string& description)
: m_label{lowercase(label), lowercase(abbrev)}, m_numValues{numValues}, m_description{
                                                                           description}
{
}


ArgSpec::operator bool() const
{
   return hasLabel() || m_numValues > 0;
}


bool ArgSpec::isRequired() const
{
   return !hasLabel() && m_numValues > 0;
}


bool ArgSpec::hasLabel() const
{
   return !!m_label;
}


std::string ArgSpec::label() const
{
   return m_label.label();
}


std::string ArgSpec::help(const std::string& indent) const
{
   // Ignore empty spec.
   if (!*this)
      return "";

   string help;

   help += indent;
   help += isRequired() ? "Required:" : "Optional:";

   if (hasLabel())
   {
      help += " -";
      help += m_label.label();
      if (m_label.haveAbbreviation())
      {
         help += "/-";
         help += m_label.abbreviation();
      }
   }

   if (m_numValues > 0)
   {
      if (hasLabel())
         help += " +";
      help += " ";
      if (m_numValues == ZeroOrMore)
         help += "zero or more";
      else if (m_numValues == OneOrMore)
         help += "one or more";
      else
         help += to_string(m_numValues);
      help += (m_numValues == 1) ? " value" : " values";
   }

   if (!m_description.empty())
   {
      help += " -- ";
      help += m_description;
   }

   return help;
}


bool ArgSpec::matchLabel(const std::string& match) const
{
   return m_label.matches(lowercase(match));
}


std::pair<bool, VerifiedArg> ArgSpec::match(CmdArgs::const_iterator& actualArgs,
                                            CmdArgs::const_iterator actualArgsEnd) const
{
   if (actualArgs == actualArgsEnd)
      return {false, {}};

   string currArg = *actualArgs;
   string currArgWithoutSep = stripArgSeparators(currArg);

   if (hasLabel() && (!isArgLabel(currArg) || !matchLabel(currArgWithoutSep)))
      return {false, {}};

   VerifiedArg matchedArg;
   if (hasLabel())
   {
      matchedArg.label = m_label.label();
      ++actualArgs;
   }

   bool haveValidValues = false;
   tie(haveValidValues, matchedArg.values) = matchValues(actualArgs, actualArgsEnd);
   if (!haveValidValues)
      return {false, {}};

   return {true, matchedArg};
}


std::pair<bool, std::vector<std::string>>
ArgSpec::matchValues(CmdArgs::const_iterator& actualArgs,
                     CmdArgs::const_iterator actualArgsEnd) const
{
   vector<string> matchedValues;

   string currArg;
   while (actualArgs != actualArgsEnd && canMatchMoreValues(matchedValues.size()))
   {
      currArg = *actualArgs;
      // Stop if we reach the next label.
      if (isArgLabel(currArg))
         break;

      matchedValues.push_back(stripArgSeparators(currArg));
      ++actualArgs;
   }

   if (!haveEnoughValues(matchedValues.size()))
      return {false, {}};
   return {true, matchedValues};
}


bool ArgSpec::canMatchMoreValues(std::size_t numMatchedValues) const
{
   if (m_numValues == OneOrMore || m_numValues == ZeroOrMore)
      return true;
   return (numMatchedValues < m_numValues);
}


bool ArgSpec::haveEnoughValues(std::size_t numMatchedValues) const
{
   if (m_numValues == ZeroOrMore)
      return (numMatchedValues >= 0);
   if (m_numValues == OneOrMore)
      return (numMatchedValues >= 1);
   return (numMatchedValues == m_numValues);
}


///////////////////

CmdSpec::CmdSpec(const std::string& name, const std::string& shortName,
                 const std::string& description, std::vector<ArgSpec> argSpecs,
                 const std::string& notes)
: m_name{lowercase(name), lowercase(shortName)}, m_description{description},
  m_argSpecs{argSpecs}, m_notes{notes}
{
}


bool CmdSpec::matchesName(const std::string& name) const
{
   return m_name.matches(name);
}


std::string CmdSpec::name() const
{
   return m_name.label();
}


std::string CmdSpec::description() const
{
   return m_description;
}


std::string CmdSpec::help() const
{
   const string Indent{"  "};
   const string Newline{"\n"};
   const string NotAvailable = Indent + string{"<none>"} + Newline;

   string help;
   help += "Name:\n";
   help += Indent + m_name.label() + Newline;

   help += "Abbreviation:\n";
   if (m_name.haveAbbreviation())
      help += Indent + m_name.abbreviation() + Newline;
   else
      help += NotAvailable;

   help += "Description:\n";
   if (!m_description.empty())
      help += Indent + m_description + Newline;
   else
      help += NotAvailable;

   help += "Arguments:\n";
   if (!m_argSpecs.empty())
   {
      for (const ArgSpec& argSpec : m_argSpecs)
         help += argSpec.help(Indent) + Newline;
   }
   else
   {
      help += NotAvailable;
   }

   help += "Notes:\n";
   if (!m_notes.empty())
   {
      const vector<string> noteLines = split(m_notes, "\n");
      for (const string& line : noteLines)
         help += Indent + line + Newline;
   }
   else
   {
      help += NotAvailable;
   }

   return help;
}


bool CmdSpec::hasArgSpec(const std::string& argLabel) const
{
   auto endPos = std::end(m_argSpecs);
   return (endPos !=
           find_if(std::begin(m_argSpecs), endPos,
                   [&argLabel](const auto& spec) { return spec.matchLabel(argLabel); }));
}


CmdSpec::ArgSpecIter_t CmdSpec::firstPositionalArgument() const
{
   return begin();
}


CmdSpec::ArgSpecIter_t CmdSpec::firstOptionalArgument() const
{
   return find_if(begin(), end(),
                  [](const ArgSpec& argSpec) { return !argSpec.isRequired(); });
}

} // namespace ccon
