//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "auto_completion.h"
#include "blackboard.h"
#include "cmd.h"
#include "cmd_depot.h"
#include "console_content.h"
#include <cstddef>
#include <memory>
#include <string>

namespace ccon
{
struct ConsoleUI;
}


namespace ccon
{
///////////////////

const std::string DefaultPrompt = "> ";


///////////////////

class Console : public ConsoleContent
{
public:
   Console(ConsoleUI& ui, const std::string& prompt = DefaultPrompt);
   ~Console() = default;
   Console(const Console&) = delete;
   Console(Console&&) = delete;
   Console& operator=(const Console&) = delete;
   Console& operator=(Console&&) = delete;

   void addCommand(const CmdSpec& spec, CmdFactoryFn factoryFn);
   std::size_t countLines() const override;
   std::string lineText(std::size_t lineIdx) const override;
   bool isEnteredLine(std::size_t lineIdx) const override;
   std::size_t minInputCursorPosition() const override;
   std::string inputLineText() const override;
   void setInputLine(const std::string& text) override;
   void processInputLine() override;
   void goToPreviousInput() override;
   void goToNextInput() override;
   void nextAutoCompletion() override;

private:
   void initCommands();
   CmdOutput processRawInput(const std::string& rawInput) const;
   CmdOutput executeCommand(const VerifiedCmd& cmdInput) const;

private:
   ConsoleUI& m_ui;
   CmdDepot m_cmds;
   Blackboard m_blackboard;
   AutoCompletion m_autoCompletion;
};

} // namespace ccon
