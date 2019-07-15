//
// ccon
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <algorithm>
#include <iterator>
#include <locale>
#include <sstream>
#include <string>


namespace ccon
{
///////////////////

inline std::string formatLabel(const std::string& label)
{
   return label + " ";
}


enum class IntBase
{
   Dec,
   Hex,
   Oct
};

template <typename Int>
std::string formatIntOutput(Int val, const std::string& label, IntBase base)
{
   static_assert(std::is_integral_v<Int>);

   std::string out;
   if (!label.empty())
      out += formatLabel(label);

   std::ios_base::fmtflags baseFlag = std::ios_base::dec;
   switch (base)
   {
   case IntBase::Hex:
      baseFlag = std::ios_base::hex;
      break;
   case IntBase::Oct:
      baseFlag = std::ios_base::oct;
      break;
   }

   std::stringstream strStream;
   // Use classic locale to get rid of thousand-separator for hex numbers.
   strStream.imbue(std::locale::classic());
   strStream.setf(baseFlag, std::ios_base::basefield);
   strStream.setf(std::ios_base::showbase);
   strStream << val;
   out += strStream.str();

   return out;
}


template <typename Iter>
std::string formatIntArrayOutput(Iter begin, Iter end, const std::string& label)
{
   using value_type = typename std::iterator_traits<Iter>::value_type;

   std::string out;
   if (!label.empty())
      out += formatLabel(label);
   out += "[";
   // Handle first element differently because of element separators.
   if (begin != end)
      out += std::to_string(*(begin++));
   std::for_each(begin, end, [&out](value_type val) {
      out += ", ";
      out += std::to_string(val);
   });
   out += "]";
   return out;
}


inline std::string formatStringOutput(const std::string& str, const std::string& label)
{
   std::string out;
   if (!label.empty())
      out += formatLabel(label);
   out += str;
   return out;
}

} // namespace ccon
