//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "console_util.h"
#include "essentutils/string_util.h"

using namespace std;
using namespace sutil;


namespace ccon
{
///////////////////

std::string stripArgSeparators(const std::string& s)
{
   return trimLeft(s, '-');
}

} // namespace ccon
