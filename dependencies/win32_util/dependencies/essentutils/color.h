//
// essentutils
// Color utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include <cstdint>


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

   std::uint8_t r = 0;
   std::uint8_t g = 0;
   std::uint8_t b = 0;
};


inline Rgb::Rgb(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_) : r{r_}, g{g_}, b{b_}
{
}

} // namespace util
