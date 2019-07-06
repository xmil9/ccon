//
// essentutils
// Color utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <cstdint>
#include <initializer_list>
#include <utility>


namespace sutil
{
///////////////////

struct Rgb
{
   Rgb() = default;
   Rgb(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_);
   Rgb(const Rgb&) = default;
   Rgb(Rgb&&) = default;
   Rgb& operator=(const Rgb&) = default;
   Rgb& operator=(Rgb&&) = default;

   friend void swap(Rgb& a, Rgb& b) noexcept;

   std::uint8_t r = 0;
   std::uint8_t g = 0;
   std::uint8_t b = 0;
};


inline Rgb::Rgb(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_) : r{r_}, g{g_}, b{b_}
{
}

inline void swap(Rgb& a, Rgb& b) noexcept
{
   std::swap(a.r, b.r);
   std::swap(a.g, b.g);
   std::swap(a.b, b.b);
}


// Comparision

inline bool operator==(const Rgb& a, const Rgb& b)
{
   return (a.r == b.r && a.g == b.g && a.b == b.b);
}

inline bool operator!=(const Rgb& a, const Rgb& b)
{
   return !(a == b);
}

} // namespace util
