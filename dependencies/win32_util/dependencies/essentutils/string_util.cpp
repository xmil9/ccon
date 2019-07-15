//
// essentutils
// String utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "string_util.h"
#include <algorithm>
#include <array>
#include <clocale>
#include <cstddef>
#include <cstdlib>


namespace
{
///////////////////

// RAII class for setting the global locale to UTF8 for the lifetime of
// an instance.
class Utf8Locale
{
 public:
   Utf8Locale();
   ~Utf8Locale();
   Utf8Locale(const Utf8Locale&) = delete;
   Utf8Locale(Utf8Locale&&) = default;
   Utf8Locale& operator=(const Utf8Locale&) = delete;
   Utf8Locale& operator=(Utf8Locale&&) = default;

 private:
   std::string m_prevLocale;
};


Utf8Locale::Utf8Locale()
{
   char* prevLocale = std::setlocale(LC_ALL, "en_US.utf8");
   if (prevLocale)
      m_prevLocale = prevLocale;
}

Utf8Locale::~Utf8Locale()
{
   if (!m_prevLocale.empty())
      std::setlocale(LC_ALL, m_prevLocale.c_str());
}

} // namespace


namespace sutil
{
///////////////////

std::string utf8(const std::string& s)
{
   // Nothing to do but it's convenient to have the overload available to
   // reduce checks in calling code.
   return s;
}


std::string utf8(const std::wstring& s)
{
   if (s.empty())
      return "";

   Utf8Locale utf8Locale;

   std::vector<char> buffer(s.size() * 4, 0);
   const size_t numBytesWritten = wcstombs(buffer.data(), s.c_str(), buffer.size());
   if (numBytesWritten == -1)
      return "";

   return buffer.data();
}


std::wstring utf16(const std::string& s)
{
   if (s.empty())
      return L"";

   Utf8Locale utf8Locale;

   std::vector<wchar_t> buffer(s.size() * 4, 0);
   const size_t numBytesWritten = mbstowcs(buffer.data(), s.c_str(), buffer.size());
   if (numBytesWritten == -1)
      return L"";

   return buffer.data();
}


std::wstring utf16(const std::wstring& s)
{
   // Nothing to do but it's convenient to have the overload available to
   // reduce checks in calling code.
   return s;
}


std::string utf8(char ch)
{
   // Nothing to do but it's convenient to have the overload available to
   // reduce checks in calling code.
   return std::string{ch};
}


std::string utf8(wchar_t ch)
{
   Utf8Locale utf8Locale;

   std::mbstate_t state{};
   std::array<char, MB_LEN_MAX + 1> buffer;
   buffer.fill('\0');
   const std::size_t bytesWritten = wcrtomb(buffer.data(), ch, &state);
   if (bytesWritten == -1)
      return {};

   return buffer.data();
}


wchar_t utf16(const char* ch, std::size_t len)
{
   Utf8Locale utf8Locale;

   std::mbstate_t state{};
   wchar_t wch;
   if (mbrtowc(&wch, ch, len, &state) < 0)
      return L'\0';

   return wch;
}


wchar_t utf16(const wchar_t* ch, std::size_t len)
{
   // Nothing to do but it's convenient to have the overload available to
   // reduce checks in calling code.
   if (len > 0)
      return *ch;
   return L'\0';
}

} // namespace sutil
