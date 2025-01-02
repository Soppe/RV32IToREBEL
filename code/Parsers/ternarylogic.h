#pragma once

#include <cstdint>
#include <vector>
#include <deque>

using Trit = std::int8_t;   // Can be -1, 0, 1
using Tryte = std::int16_t; // Can be between -+ (3^6 - 1) / 2
using Tint = std::int64_t;  // Can be between -+ (3^24 - 1) / 2
using Trits = std::deque<Trit>;
using Trytes = std::vector<Tryte>;

class TernaryLogic
{
public:
  TernaryLogic() = delete;

  static void tintToTrits(Tint in, Trits &out);
  static void tritsToTint(const Trits &in, Tint &out);

  static void parseImmediate(std::uint8_t immediateSizeTrits, const Tint &in, Tint &out);
};
