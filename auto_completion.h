//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "cmd_spec.h"
#include <set>
#include <string>
#include <vector>


namespace ccon
{
///////////////////

class AutoCompletion
{
public:
   void setCmds(const std::set<CmdSpec>& cmds);
   std::string next(const std::string& pattern);
   std::vector<std::string> all(const std::string& pattern);
   void reset();

private:
   void complete(const std::string& pattern);
   void completeIfNecessary(const std::string& pattern);
   std::string next();

private:
   std::set<CmdSpec> m_cmds;
   std::string m_completedPattern;
   std::vector<std::string> m_completions;
   std::vector<std::string>::const_iterator m_next;
};

} // namespace ccon
