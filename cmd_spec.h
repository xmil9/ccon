//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd.h"
#include <cstddef>
#include <limits>
#include <optional>
#include <string>
#include <utility>
#include <vector>


namespace ccon
{
///////////////////

class LabelWithAbbrev
{
 public:
   LabelWithAbbrev() = default;
   LabelWithAbbrev(const std::string& label, const std::string& abbrev);
   ~LabelWithAbbrev() = default;
   LabelWithAbbrev(const LabelWithAbbrev&) = default;
   LabelWithAbbrev(LabelWithAbbrev&&) = default;
   LabelWithAbbrev& operator=(const LabelWithAbbrev&) = default;
   LabelWithAbbrev& operator=(LabelWithAbbrev&&) = default;

   explicit operator bool() const;

   bool matches(const std::string& match) const;
   std::string label() const;
   bool haveAbbreviation() const;
   std::string abbreviation() const;

 private:
   std::string m_label;
   std::string m_abbrev;
};


///////////////////

// Specification for a command argument.
class ArgSpec
{
 public:
   static constexpr std::size_t ZeroOrMore = std::numeric_limits<std::size_t>::max();
   static constexpr std::size_t OneOrMore = std::numeric_limits<std::size_t>::max() - 1;

   // Creates a spec for a required, positional argument (one or more values).
   static ArgSpec makePositionalArg(std::size_t numValues,
                                    const std::string& description = "");
   // Creates a spec for an optional argument (a label followed by one or more values).
   static ArgSpec makeOptionalArg(const std::string& label, std::size_t numValues,
                                  const std::string& abbrev = "",
                                  const std::string& description = "");
   // Creates a spec for a flag argument (a special optional argument without any values).
   static ArgSpec makeFlagArg(const std::string& label, const std::string& abbrev = "",
                              const std::string& description = "");

   ArgSpec() = default;
   ~ArgSpec() = default;
   ArgSpec(const ArgSpec&) = default;
   ArgSpec(ArgSpec&&) = default;
   ArgSpec& operator=(const ArgSpec&) = default;
   ArgSpec& operator=(ArgSpec&&) = default;

   explicit operator bool() const;
   bool isRequired() const;
   bool hasLabel() const;
   std::string label() const;
   std::string help(const std::string& indent = {}) const;

   bool matchLabel(const std::string& argName) const;
   std::optional<VerifiedArg> match(CmdArgs::const_iterator& actualArgs,
                                    CmdArgs::const_iterator actualArgsEnd) const;

 private:
   ArgSpec(const std::string& label, const std::string& abbrev, std::size_t numValues,
           const std::string& description);

   std::optional<std::vector<std::string>>
   matchValues(CmdArgs::const_iterator& actualArgs,
               CmdArgs::const_iterator actualArgsEnd) const;
   bool canMatchMoreValues(std::size_t matchedValues) const;
   bool haveEnoughValues(std::size_t matchedValues) const;

 private:
   LabelWithAbbrev m_label;
   std::size_t m_numValues = 0;
   std::string m_description;
};


// Common help argument that can be used for all commands.
const ArgSpec HelpArgSpec = ArgSpec::makeFlagArg("help", "?");


///////////////////

// Specification for a command.
// Argument kinds:
// - Positional arguments:
//   - A sequence of values (numbers or text).
//   - Have to come before any optional arguments.
//   - Their order matters.
//   - They are required. Missing one will cause an error.
// - Optional arguments:
//   - A label (dash or double-dash followed by a name). Possibly followed by a
//     sequence of values (numbers or text).
//   - Have to come after any positional arguments.
//   - Their order can be changed.
//   - They are optional. Missing one will not cause an error.
class CmdSpec
{
 public:
   using ArgSpecIter_t = std::vector<ArgSpec>::const_iterator;

 public:
   CmdSpec() = default;
   CmdSpec(const std::string& name, const std::string& shortName,
           const std::string& description, std::vector<ArgSpec> argSpecs,
           const std::string& notes);
   ~CmdSpec() = default;
   CmdSpec(const CmdSpec&) = default;
   CmdSpec(CmdSpec&&) = default;
   CmdSpec& operator=(const CmdSpec&) = default;
   CmdSpec& operator=(CmdSpec&&) = default;

   explicit operator bool() const;
   bool matchesName(const std::string& cmdName) const;
   std::string name() const;
   std::string description() const;
   std::string help() const;
   bool hasArgSpec(const std::string& argLabel) const;

   ArgSpecIter_t begin() const { return m_argSpecs.begin(); }
   ArgSpecIter_t end() const { return m_argSpecs.end(); }
   ArgSpecIter_t firstPositionalArgument() const;
   ArgSpecIter_t firstOptionalArgument() const;

 private:
   LabelWithAbbrev m_name;
   std::string m_description;
   std::string m_notes;
   std::vector<ArgSpec> m_argSpecs;
};


inline bool operator==(const CmdSpec& a, const CmdSpec& b)
{
   return (a.name() == b.name());
}

inline bool operator!=(const CmdSpec& a, const CmdSpec& b)
{
   return !(a == b);
}

inline bool operator<(const CmdSpec& a, const CmdSpec& b)
{
   return (a.name() < b.name());
}

inline bool operator<=(const CmdSpec& a, const CmdSpec& b)
{
   return (a < b || a == b);
}

inline bool operator>(const CmdSpec& a, const CmdSpec& b)
{
   return !(a <= b);
}

inline bool operator>=(const CmdSpec& a, const CmdSpec& b)
{
   return !(a < b);
}

} // namespace ccon
