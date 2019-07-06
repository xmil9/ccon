//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "test_util.h"
#include <iostream>
#include <string>


static std::string errorMessage(const std::string& label, const std::string& reason,
                                const std::string& fileName, int lineNum)
{
   std::string msg;
   msg += "Failed - ";
   msg += label;
   msg += ": '";
   msg += reason;
   msg += "'.";
   msg += " (file: ";
   msg += fileName;
   msg += ", line: ";
   msg += std::to_string(lineNum);
   msg += ").";
   return msg;
}


bool verify(bool cond, const std::string& label, const std::string& reason,
            const std::string& fileName, int lineNum)
{
   if (!cond)
      std::cout << errorMessage(label, reason, fileName, lineNum) << "\n";
   return cond;
}
