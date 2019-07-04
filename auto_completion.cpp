//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "auto_completion.h"
#include "cmd_spec.h"
#include "console_util.h"
#include "essentutils/string_util.h"
#include <set>

using namespace ccon;
using namespace std;
using namespace sutil;


namespace
{
///////////////////

std::vector<std::string> autoCompleteCmdName(const std::string& input,
                                             const std::set<CmdSpec>& cmds)
{
   vector<string> candidates;
   for (const CmdSpec& spec : cmds)
      if (startsWith(spec.name(), input))
         candidates.push_back(spec.name());
   return candidates;
}


std::vector<std::string> autoCompleteCmdArg(const std::string& cmdName,
                                            const std::string& inputArg,
                                            const std::set<CmdSpec>& cmds)
{
   const string inputLabel = stripArgSeparators(inputArg);
   const string strippedSeps = inputArg.substr(0, inputArg.size() - inputLabel.size());

   vector<string> candidates;

   for (const CmdSpec& cmdSpec : cmds)
   {
      if (cmdSpec.matchesName(cmdName))
      {
         for (const ArgSpec& argSpec : cmdSpec)
         {
            if (argSpec.hasLabel() && startsWith(argSpec.label(), inputLabel))
               candidates.push_back(strippedSeps + argSpec.label());
         }

         break;
      }
   }

   return candidates;
}


std::vector<std::string> autoComplete(const std::string& input,
                                      const std::set<CmdSpec>& cmds)
{
   vector<string> cmdPieces = split(input, " ");
   if (cmdPieces.empty())
      return {};

   if (cmdPieces.size() == 1)
   {
      return autoCompleteCmdName(lowercase(cmdPieces[0]), cmds);
   }
   else
   {
      const vector<string> argCompletions = autoCompleteCmdArg(
         lowercase(cmdPieces[0]), lowercase(*cmdPieces.rbegin()), cmds);

      const string inputFront =
         join(cmdPieces.begin(), (cmdPieces.rbegin() + 1).base(), " ") + ' ';

      vector<string> inputCompletions;
      for (const string& completedArg : argCompletions)
         inputCompletions.push_back(inputFront + completedArg);

      return inputCompletions;
   }
}

} // namespace


namespace ccon
{
///////////////////

void AutoCompletion::setCmds(const std::set<CmdSpec>& cmds)
{
   m_cmds = cmds;
}


std::string AutoCompletion::next(const std::string& pattern)
{
   completeIfNecessary(pattern);
   return next();
}


std::vector<std::string> AutoCompletion::all(const std::string& pattern)
{
   completeIfNecessary(pattern);
   return m_completions;
}


void AutoCompletion::reset()
{
   m_completedPattern = {};
   m_completions.clear();
   m_next = m_completions.end();
}


void AutoCompletion::complete(const std::string& pattern)
{
   m_completedPattern = pattern;
   m_completions = autoComplete(pattern, m_cmds);
   m_next = m_completions.begin();
}


void AutoCompletion::completeIfNecessary(const std::string& pattern)
{
   // Check if the completions got reset and we need to calculate new
   // ones. Note that the passed pattern might be different froms
   // the completed pattern but we still don't want to re-calculate
   // the completions because the pattern can be a previously returned
   // completion itself that gets passed back as new pattern into this
   // function! That's the reason why we need an explicit reset call
   // with which client code can control when a pattern reset should
   // happen.
   if (m_completedPattern.empty() && !pattern.empty())
      complete(pattern);
}


std::string AutoCompletion::next()
{
   if (m_next == m_completions.end())
      m_next = m_completions.begin();
   return (m_next != m_completions.end()) ? *m_next++ : "";
}

} // namespace ccon
