#ifndef TERNARYLOGIC_H
#define TERNARYLOGIC_H

#include <cstdint>

namespace Simulators
{
namespace REBEL6
{

using Trit = std::int8_t; // Can be -1, 0, 1
using Tint = std::int64_t; // (-3^21, 3^21)

#define TRITS_PER_TRYTE 6
#define TRITS_PER_THALFWORD 11
#define TRITS_PER_TWORD 21
#define TRITS_PER_INSTRUCTION 32

class TernaryLogic
{
public:
   TernaryLogic() = delete;

};

}
}

#endif // TERNARYLOGIC_H
