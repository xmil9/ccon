//
// ccon tests
//
// Jul-2019, Michael Lindner
// MIT license
//
#pragma once
#include <string>


bool verify(bool cond, const std::string& label, const std::string& reason,
            const std::string& fileName, int lineNum);

#define VERIFY(cond, label) (verify(cond, label, #cond, __FILE__, __LINE__))
#define FAIL(reason, label) (verify(false, label, reason, __FILE__, __LINE__))
