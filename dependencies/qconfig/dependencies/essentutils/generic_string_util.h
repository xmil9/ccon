//
// essentutils
// Generic versions of the string utilities.
// The generic utilities can be used directly but template parameter deduction for
// string literals fails because the deduced type is a character array. Specifying
// the template parameter type works, e.g. startsWith<std::string>("aaa", "aa"), but
// is less convenient to use than the functions in string_util.h/cpp.
// The fully generic string utility functions basically serve as implementation for
// the "partially specified" overloads.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>


namespace sutil
{
namespace genstr
{
///////////////////

// Interface

template <typename Str> bool startsWith(const Str& s, const Str& start);
template <typename Str> bool endsWith(const Str& s, const Str& tail);
template <typename Str> Str trimLeft(const Str& s, typename Str::value_type ch);
template <typename Str> Str trimRight(const Str& s, typename Str::value_type ch);
template <typename Str> Str trim(const Str& s, typename Str::value_type ch);
template <typename Str> std::vector<Str> split(const Str& s, const Str& separator);
template <typename Iter, typename Str> Str join(Iter it, Iter end, const Str& separator);
template <typename Int, typename Str> Int intFromStrThrow(const Str& s);
template <typename Int, typename Str> Int intFromStr(const Str& s, Int defaultValue) noexcept;
template <typename Int, typename Str> std::optional<Int> intFromStr(const Str& s) noexcept;
template <typename FP, typename Str> FP fpFromStrThrow(const Str& s);
template <typename FP, typename Str> FP fpFromStr(const Str& s, FP defaultValue) noexcept;
template <typename FP, typename Str> std::optional<FP> fpFromStr(const Str& s) noexcept;


///////////////////

// Implementation

template <typename Str>
inline constexpr bool isStdString =
   std::is_same_v<Str, std::string> || std::is_same_v<Str, std::wstring> ||
   std::is_same_v<Str, std::u16string> || std::is_same_v<Str, std::u32string>;


template <typename Str> bool startsWith(const Str& s, const Str& start)
{
   static_assert(isStdString<Str>);
   const std::size_t pos = s.find(start);
   return (pos == 0);
}


template <typename Str> bool endsWith(const Str& s, const Str& tail)
{
   static_assert(isStdString<Str>);
   const std::size_t pos = s.rfind(tail);
   return (pos + tail.size() == s.size());
}


template <typename Str> Str trimLeft(const Str& s, typename Str::value_type ch)
{
   static_assert(isStdString<Str>);
   const std::size_t pos = s.find_first_not_of(ch);
   if (pos != Str::npos)
      return s.substr(pos);
   return {};
}


template <typename Str> Str trimRight(const Str& s, typename Str::value_type ch)
{
   static_assert(isStdString<Str>);
   const std::size_t pos = s.find_last_not_of(ch);
   if (pos != Str::npos)
      return s.substr(0, pos + 1);
   return {};
}


template <typename Str> Str trim(const Str& s, typename Str::value_type ch)
{
   static_assert(isStdString<Str>);
   return trimRight(trimLeft(s, ch), ch);
}


template <typename Str> std::vector<Str> split(const Str& s, const Str& separator)
{
   static_assert(isStdString<Str>);
   std::vector<Str> split;

   const std::size_t sepLen = separator.size();
   std::size_t pos = 0;
   std::size_t next = s.find(separator, pos);
   while (next != -1)
   {
      split.push_back(s.substr(pos, next - pos));
      pos = next + sepLen;
      next = s.find(separator, pos);
   }

   split.push_back(s.substr(pos));
   return split;
}


template <typename Iter, typename Str> Str join(Iter it, Iter end, const Str& separator)
{
   static_assert(isStdString<Str>);
   static_assert(std::is_same_v<std::iterator_traits<Iter>::value_type, Str>);

   Str joined;
   for (; it != end; ++it)
   {
      if (!joined.empty())
         joined += separator;
      joined += *it;
   }
   return joined;
}


template <typename Int, typename Str> Int intFromStrThrow(const Str& s)
{
   static_assert(isStdString<Str>);
   static_assert(std::is_integral_v<Int>);

   if constexpr (std::is_same_v<Int, long>)
      return std::stol(s.c_str());
   else if constexpr (std::is_same_v<Int, long long>)
      return std::stoll(s.c_str());
   else if constexpr (std::is_same_v<Int, unsigned long>)
      return std::stoul(s.c_str());
   else if constexpr (std::is_same_v<Int, unsigned long long>)
      return std::stoull(s.c_str());
   else
      return static_cast<Int>(std::stoi(s.c_str()));
}


template <typename Int, typename Str> Int intFromStr(const Str& s, Int defaultValue) noexcept
{
   static_assert(isStdString<Str>);
   static_assert(std::is_integral_v<Int>);

   try
   {
      return intFromStrThrow<Int, Str>(s);
   }
   catch (...)
   {
      return defaultValue;
   }
}


template <typename Int, typename Str> std::optional<Int> intFromStr(const Str& s) noexcept
{
   static_assert(isStdString<Str>);
   static_assert(std::is_integral_v<Int>);

   try
   {
      return intFromStrThrow<Int, Str>(s);
   }
   catch (...)
   {
      return std::nullopt;
   }
}


template <typename FP, typename Str> FP fpFromStrThrow(const Str& s)
{
   static_assert(isStdString<Str>);
   static_assert(std::is_floating_point_v<FP>);

   if constexpr (std::is_same_v<FP, double>)
      return std::stod(s.c_str());
   else if constexpr (std::is_same_v<FP, long double>)
      return std::stold(s.c_str());
   else
      return std::stof(s.c_str());
}


template <typename FP, typename Str> FP fpFromStr(const Str& s, FP defaultValue) noexcept
{
   static_assert(isStdString<Str>);
   static_assert(std::is_floating_point_v<FP>);

   try
   {
      return fpFromStrThrow<FP>(s);
   }
   catch (...)
   {
      return defaultValue;
   }
}


template <typename FP, typename Str> std::optional<FP> fpFromStr(const Str& s) noexcept
{
   static_assert(isStdString<Str>);
   static_assert(std::is_floating_point_v<FP>);

   try
   {
      return fpFromStrThrow<FP>(s);
   }
   catch (...)
   {
      return {};
   }
}

} // namespace genstr
} // namespace sutil
