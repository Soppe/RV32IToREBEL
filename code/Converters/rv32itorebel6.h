#pragma once

#include "rv32ipseudotorv32ibase.h"

namespace Converters
{
class RV32IToREBEL6 : public RV32IPseudoToRV32IBase
{

#define REBEL6_TRITS_PER_TRYTE 6
#define REBEL6_TRITS_PER_INSTRUCTION 32
#define REBEL6_JUMPS_PER_INSTRUCTION 1

public:
   RV32IToREBEL6();
   virtual ~RV32IToREBEL6() = default;

private:
   void fillExpressionMap();
};
}
