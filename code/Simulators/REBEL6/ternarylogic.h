#pragma once

#include <cstdint>
#include <vector>
#include <deque>

namespace Simulators
{
namespace REBEL6
{

using Trit = std::int8_t; // Can be -1, 0, 1
using Trits = std::deque<Trit>;
using Tint = std::int64_t; // (-3^21, 3^21)

#define TRITS_PER_TRYTE 6
#define TRITS_PER_THALFWORD 12
#define TRITS_PER_TWORD 24
#define TRITS_PER_INSTRUCTION 32

class TernaryLogic
{
 public:
   TernaryLogic() = delete;

   static void TintToTrits(Tint in, Trits& out);
   static void TritsToTint(const Trits& in, Tint& out);

   static void ParseImmediate(std::uint8_t immediateSizeTrits, const Tint& in, Tint& out);
};

}
}
