#pragma once

#include <cstdint>
#include <vector>
#include <deque>

using Trit = std::int8_t; // Can be -1, 0, 1
using Tryte = std::int16_t; // Can be -3^6 - 3^6
using Tint = std::int64_t; // (-3^21, 3^21)
using Trits = std::deque<Trit>;

class TernaryLogic
{
 public:
   TernaryLogic() = delete;

   static void TintToTrits(Tint in, Trits& out);
   static void TritsToTint(const Trits& in, Tint& out);

   static void ParseImmediate(std::uint8_t immediateSizeTrits, const Tint& in, Tint& out);
};
