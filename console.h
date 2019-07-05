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

class Console : public ConsoleContent
{
public:
   Console();
   ~Console() = default;
   Console(const Console&) = delete;
   Console(Console&&) = default;
   Console& operator=(const Console&) = delete;
   Console& operator=(Console&&) = default;

   void setUI(ConsoleUI* ui);
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
   CmdOutput processRawInput(const std::string& rawInput) const;
   CmdOutput executeCommand(const VerifiedCmd& cmdInput) const;

private:
   CmdDepot m_cmds;
   Blackboard m_blackboard;
   AutoCompletion m_autoCompletion;
   ConsoleUI* m_ui = nullptr;
};


std::unique_ptr<Console> makeConsole();

} // namespace ccon
