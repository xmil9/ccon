//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "console_util.h"
#include "essentutils/string_util.h"


namespace ccon
{
///////////////////

std::string stripArgSeparators(const std::string& s)
{
   return sutil::trimLeft(s, '-');
}

} // namespace ccon
