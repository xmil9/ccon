//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "blackboard_tests.h"
#include "blackboard.h"
#include "essentutils/string_util.h"
#include "test_util.h"
#include <string>

using namespace ccon;


namespace
{
///////////////////

void testBlackboardDefaultCtor()
{
   {
      const std::string caseLabel = "Blackboard default ctor";
      Blackboard board;
      VERIFY(board.countLines() == 1, caseLabel);
   }
}


void testBlackboardCountLines()
{
   {
      const std::string caseLabel = "Blackboard::countLines for initial line";
      Blackboard board;
      VERIFY(board.countLines() == 1, caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::countLines for multiple lines";
      Blackboard board;
      board.appendLine("line 1");
      board.appendLine("line 2");
      VERIFY(board.countLines() == 3, caseLabel);
   }
}


void testBlackboardLineText()
{
   {
      const std::string caseLabel = "Blackboard::lineText for initial line";
      Blackboard board;
      board.setInputLine("my text");
      VERIFY(board.lineText(0) == "my text", caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::lineText for multiple lines";
      Blackboard board;
      board.appendLine("line 1");
      board.appendLine("line 2");
      VERIFY(!board.lineText(0).empty(), caseLabel);
      VERIFY(board.lineText(1) == "line 1", caseLabel);
      VERIFY(board.lineText(2) == "line 2", caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::lineText for invalid index";
      Blackboard board;
      board.appendLine("line 1");
      VERIFY(board.lineText(10).empty(), caseLabel);
   }
}


void testBlackboardIsEnteredLine()
{
   {
      const std::string caseLabel = "Blackboard::isEnteredLine for initial line";
      Blackboard board;
      VERIFY(board.isEnteredLine(0), caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::isEnteredLine for multiple lines";
      Blackboard board;
      board.appendLine("line 1");
      board.appendLine("line 2");
      VERIFY(board.isEnteredLine(0), caseLabel);
      VERIFY(!board.isEnteredLine(1), caseLabel);
      VERIFY(!board.isEnteredLine(2), caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::isEnteredLine for invalid index";
      Blackboard board;
      board.appendLine("line 1");
      VERIFY(!board.isEnteredLine(10), caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::isEnteredLine for input line";
      Blackboard board;
      VERIFY(board.isEnteredLine(0), caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::isEnteredLine for appended line";
      Blackboard board;
      board.appendLine("line 1");
      VERIFY(!board.isEnteredLine(1), caseLabel);
   }
}


void testBlackboardAppendLine()
{
   {
      const std::string caseLabel = "Blackboard::appendLine";
      Blackboard board;
      board.appendLine("line 1");
      board.appendLine("line 2");
      VERIFY(board.countLines() == 3, caseLabel);
      VERIFY(board.lineText(1) == "line 1", caseLabel);
      VERIFY(board.lineText(2) == "line 2", caseLabel);
   }
}


void testBlackboardInputLineText()
{
   {
      const std::string caseLabel = "Blackboard::inputLineText for existing input text";
      Blackboard board;
      board.setInputLine("my input");
      VERIFY(board.inputLineText() == "my input", caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::inputLineText for no input text";
      Blackboard board;
      VERIFY(board.inputLineText().size() == board.promptLength(), caseLabel);
   }
}


void testBlackboardEnteredInputText()
{
   {
      const std::string caseLabel = "Blackboard::enteredInputText for entered input text";
      Blackboard board;
      board.setEnteredInputText("my input");
      VERIFY(board.enteredInputText() == "my input", caseLabel);
   }
   {
      const std::string caseLabel =
         "Blackboard::enteredInputText for no entered input text";
      Blackboard board;
      VERIFY(board.enteredInputText().empty(), caseLabel);
   }
}


void testBlackboardSetInputLine()
{
   {
      const std::string caseLabel = "Blackboard::setInputLine";
      Blackboard board;
      board.setInputLine("my input");
      VERIFY(board.inputLineText() == "my input", caseLabel);
   }
}


void testBlackboardSetEnteredInputText()
{
   {
      const std::string caseLabel = "Blackboard::setEnteredInputText";
      Blackboard board;
      board.setEnteredInputText("my input");
      VERIFY(board.enteredInputText() == "my input", caseLabel);
   }
   {
      const std::string caseLabel =
         "Blackboard::setEnteredInputText does not overwrite the prompt";
      Blackboard board;
      board.setEnteredInputText("my input");
      VERIFY(board.inputLineText() != "my input", caseLabel);
      VERIFY(sutil::endsWith(board.inputLineText(), "my input"), caseLabel);
   }
}


void testBlackboardStartNewInputLine()
{
   {
      const std::string caseLabel = "Blackboard::startNewInputLine adds a line";
      Blackboard board;
      board.appendLine("line 1");
      const std::size_t prevNumLines = board.countLines();

      board.startNewInputLine();

      VERIFY(board.countLines() == prevNumLines + 1, caseLabel);
   }
   {
      const std::string caseLabel =
         "Blackboard::startNewInputLine adds prompt but no other text";
      Blackboard board;
      board.appendLine("line 1");

      board.startNewInputLine();

      VERIFY(board.inputLineText().size() == board.promptLength(), caseLabel);
      VERIFY(board.enteredInputText().empty(), caseLabel);
   }
}


void testBlackboardCommitInputLine()
{
   {
      const std::string caseLabel =
         "Blackboard::commitInputLine adds input line to history";
      Blackboard board;
      board.setInputLine("line 1");
      board.commitInputLine();
      board.setInputLine("line 2");
      board.commitInputLine();
      board.startNewInputLine();

      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 2", caseLabel);
   }
}


void testBlackboardPromptLength()
{
   {
      const std::string caseLabel = "Blackboard::promptLength";
      Blackboard board;
      VERIFY(board.promptLength() > 0, caseLabel);
   }
}


void testBlackboardGoToPreviousInput()
{
   {
      const std::string caseLabel =
         "Blackboard::goToPreviousInput moves backwards in history";
      Blackboard board;
      board.setInputLine("line 1");
      board.commitInputLine();
      board.setInputLine("line 2");
      board.commitInputLine();
      board.setInputLine("line 3");
      board.commitInputLine();
      board.startNewInputLine();

      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 3", caseLabel);
      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 2", caseLabel);
      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 1", caseLabel);
   }
   {
      const std::string caseLabel =
         "Blackboard::goToPreviousInput does nothing for empty history";
      Blackboard board;
      board.setInputLine("line 1");
      board.appendLine("line 2");
      board.appendLine("line 3");

      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 3", caseLabel);
      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 3", caseLabel);
   }
   {
      const std::string caseLabel = "Blackboard::goToPreviousInput does not moves past "
                                    "the oldest line in the history";
      Blackboard board;
      board.setInputLine("line 1");
      board.commitInputLine();
      board.setInputLine("line 2");
      board.commitInputLine();
      board.startNewInputLine();

      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 2", caseLabel);
      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 1", caseLabel);
      board.goToPreviousInput();
      VERIFY(board.inputLineText() == "line 1", caseLabel);
   }
}


void testBlackboardGoToNextInput()
{
   {
      const std::string caseLabel = "Blackboard::goToNextInput moves forward in history";
      Blackboard board;
      board.setInputLine("line 1");
      board.commitInputLine();
      board.setInputLine("line 2");
      board.commitInputLine();
      board.setInputLine("line 3");
      board.commitInputLine();
      board.startNewInputLine();
      board.goToPreviousInput();
      board.goToPreviousInput();
      board.goToPreviousInput();

      board.goToNextInput();
      VERIFY(board.inputLineText() == "line 2", caseLabel);
      board.goToNextInput();
      VERIFY(board.inputLineText() == "line 3", caseLabel);
   }
   {
      const std::string caseLabel =
         "Blackboard::goToNextInput does nothing for empty history";
      Blackboard board;
      board.setInputLine("line 1");
      board.appendLine("line 2");
      board.appendLine("line 3");

      board.goToNextInput();
      VERIFY(board.inputLineText() == "line 3", caseLabel);
      board.goToNextInput();
      VERIFY(board.inputLineText() == "line 3", caseLabel);
   }
   {
      const std::string caseLabel =
         "Blackboard::goToNextInput sets input line to prompt what moving past "
         "the newest line in the history";
      Blackboard board;
      board.setInputLine("line 1");
      board.commitInputLine();
      board.setInputLine("line 2");
      board.commitInputLine();
      board.startNewInputLine();
      board.goToPreviousInput();
      board.goToPreviousInput();

      board.goToNextInput();
      VERIFY(board.inputLineText() == "line 2", caseLabel);
      board.goToNextInput();
      VERIFY(board.inputLineText().size() == board.promptLength(), caseLabel);
      VERIFY(board.enteredInputText().empty(), caseLabel);
   }
}

} // namespace


void testBlackboard()
{
   testBlackboardDefaultCtor();
   testBlackboardCountLines();
   testBlackboardLineText();
   testBlackboardIsEnteredLine();
   testBlackboardAppendLine();
   testBlackboardInputLineText();
   testBlackboardEnteredInputText();
   testBlackboardSetInputLine();
   testBlackboardSetEnteredInputText();
   testBlackboardStartNewInputLine();
   testBlackboardCommitInputLine();
   testBlackboardPromptLength();
   testBlackboardGoToPreviousInput();
   testBlackboardGoToNextInput();
}
