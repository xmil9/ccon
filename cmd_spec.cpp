//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "cmd.h"
#include "cmd_spec.h"
#include "cmd_parser.h"
#include "console_util.h"
#include "essentutils/string_util.h"
#include <algorithm>
#include <cassert>


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


std::optional<ccon::VerifiedArg> matchArgSpec(const ccon::ArgSpec& spec,
                                              ccon::CmdArgs::const_iterator& actualArgs,
                                              ccon::CmdArgs::const_iterator actualArgsEnd)
{
   ccon::CmdArgs::const_iterator iterCopy = actualArgs;
   const std::optional<ccon::VerifiedArg> match = spec.match(iterCopy, actualArgsEnd);
   if (!match.has_value())
      return std::nullopt;

   // Only advance the iterator over the actual arguments if we found a match.
   actualArgs = iterCopy;
   return match;
}


std::optional<ccon::VerifiedArgs> matchPositionalCmdArgs(
   ccon::CmdSpec::ArgSpecIter_t posSpec, ccon::CmdSpec::ArgSpecIter_t posSpecEnd,
   ccon::CmdArgs::const_iterator& actualArgs, ccon::CmdArgs::const_iterator actualArgsEnd)
{
   ccon::VerifiedArgs verifiedArgs;

   for (; posSpec != posSpecEnd; ++posSpec)
   {
      const std::optional<ccon::VerifiedArg> matchedArg =
         matchArgSpec(*posSpec, actualArgs, actualArgsEnd);
      if (!matchedArg.has_value())
         return std::nullopt;

      verifiedArgs.push_back(matchedArg.value());
   }

   return verifiedArgs;
}


std::optional<ccon::VerifiedArgs> matchOptionalCmdArgs(
   ccon::CmdSpec::ArgSpecIter_t optSpecBegin, ccon::CmdSpec::ArgSpecIter_t optSpecEnd,
   ccon::CmdArgs::const_iterator& actualArgs, ccon::CmdArgs::const_iterator actualArgsEnd)
{
   ccon::VerifiedArgs verifiedArgs;

   while (actualArgs != actualArgsEnd)
   {
      bool haveMatch = false;

      for (ccon::CmdSpec::ArgSpecIter_t optSpec = optSpecBegin; optSpec != optSpecEnd;
           ++optSpec)
      {
         const std::optional<ccon::VerifiedArg> matchedArg =
            matchArgSpec(*optSpec, actualArgs, actualArgsEnd);
         if (matchedArg.has_value())
         {
            verifiedArgs.push_back(matchedArg.value());
            haveMatch = true;
            break;
         }
      }

      // There is an actual arg that does not match any optional arg spec. Fail the
      // parsing.
      if (!haveMatch)
         return std::nullopt;
   }

   return verifiedArgs;
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

ArgSpec ArgSpec::makePositionalArg(std::size_t numValues, const std::string& description)
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
: m_label{sutil::lowercase(label), sutil::lowercase(abbrev)}, m_numValues{numValues},
  m_description{description}
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

   std::string help;

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
         help += std::to_string(m_numValues);
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
   return m_label.matches(sutil::lowercase(match));
}


std::optional<VerifiedArg> ArgSpec::match(CmdArgs::const_iterator& actualArgs,
                                          CmdArgs::const_iterator actualArgsEnd) const
{
   if (actualArgs == actualArgsEnd)
   {
      // Zero matching positional values might be valid for 'zero or more' specs.
      return (!hasLabel() && haveEnoughValues(0))
                ? std::optional<VerifiedArg>{VerifiedArg{}}
                : std::nullopt;
   }

   CmdArgs::const_iterator actualArgsBegin = actualArgs;
   std::string currArg = *actualArgs;
   std::string currArgWithoutSep = stripArgSeparators(currArg);

   if (hasLabel() && (!isArgLabel(currArg) || !matchLabel(currArgWithoutSep)))
      return std::nullopt;

   VerifiedArg matchedArg;
   if (hasLabel())
   {
      matchedArg.label = m_label.label();
      ++actualArgs;
   }

   const auto matchedValues = matchValues(actualArgs, actualArgsEnd);
   if (!matchedValues.has_value())
   {
      // Restore original position of actual arg iterator.
      actualArgs = actualArgsBegin;
      return std::nullopt;
   }
   matchedArg.values = matchedValues.value();

   return matchedArg;
}


std::optional<std::vector<std::string>>
ArgSpec::matchValues(CmdArgs::const_iterator& actualArgs,
                     CmdArgs::const_iterator actualArgsEnd) const
{
   std::vector<std::string> matchedValues;

   std::string currArg;
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
      return std::nullopt;
   return matchedValues;
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
: m_name{sutil::lowercase(name), sutil::lowercase(shortName)}, m_description{description},
  m_argSpecs{argSpecs}, m_notes{notes}
{
}


CmdSpec::operator bool() const
{
   return !!m_name;
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
   // Ignore empty spec.
   if (!*this)
      return "";

   const std::string Indent{"  "};
   const std::string Newline{"\n"};
   const std::string NotAvailable = Indent + std::string{"<none>"} + Newline;

   std::string help;
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
      const std::vector<std::string> noteLines = sutil::split(m_notes, "\n");
      for (const std::string& line : noteLines)
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


CmdSpec::Match CmdSpec::match(const std::string& cmd) const
{
   if (!*this)
      return {};

   std::vector<std::string> cmdPieces = sutil::split(cmd, " ");
   if (cmdPieces.empty())
      return {};

   if (!matchesName(sutil::lowercase(cmdPieces[0])))
      return {};

   VerifiedCmd verifiedCmd;
   verifiedCmd.name = name();

   cmdPieces.erase(cmdPieces.begin());

   const std::optional<VerifiedArgs> args = matchCmdArgs(cmdPieces);
   if (!args.has_value())
      return {true, false, {}};
   verifiedCmd.args = args.value();

   return {true, true, verifiedCmd};
}


std::optional<VerifiedArgs> CmdSpec::matchCmdArgs(const CmdArgs& args) const
{
   if (containsHelpParameter(args))
      return VerifiedArgs{VerifiedArg{HelpArgSpec.label()}};

   auto posSpecs = firstPositionalArgument();
   auto posSpecsEnd = firstOptionalArgument();
   auto optSpecs = posSpecsEnd;
   auto optSpecsEnd = end();

   auto actualArgs = std::begin(args);
   auto actualArgsEnd = std::end(args);

   VerifiedArgs verifiedArgs;
   const std::optional<VerifiedArgs> parsedArgs =
      matchPositionalCmdArgs(posSpecs, posSpecsEnd, actualArgs, actualArgsEnd);
   if (!parsedArgs.has_value())
      return std::nullopt;
   verifiedArgs = parsedArgs.value();

   const std::optional<VerifiedArgs> parsedOptArgs =
      matchOptionalCmdArgs(optSpecs, optSpecsEnd, actualArgs, actualArgsEnd);
   if (!parsedOptArgs.has_value())
      return std::nullopt;

   const bool haveUnmacthedArgs = (actualArgs != std::end(args));
   if (haveUnmacthedArgs)
      return std::nullopt;

   std::copy(parsedOptArgs.value().begin(), parsedOptArgs.value().end(),
             std::back_inserter(verifiedArgs));
   return verifiedArgs;
}


CmdSpec::ArgSpecIter_t CmdSpec::firstPositionalArgument() const
{
   auto pos = find_if(begin(), end(),
                      [](const ArgSpec& argSpec) { return argSpec.isRequired(); });
   if (pos != end())
      return pos;
   // The end iterator for the positional arguments is the iterator of the first optional
   // argument.
   return firstOptionalArgument();
}


CmdSpec::ArgSpecIter_t CmdSpec::firstOptionalArgument() const
{
   return find_if(begin(), end(),
                  [](const ArgSpec& argSpec) { return !argSpec.isRequired(); });
}

} // namespace ccon
