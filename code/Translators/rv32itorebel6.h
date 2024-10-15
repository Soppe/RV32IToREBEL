#pragma once

#include "rv32itorv32ibase.h"

namespace Translators
{
class RV32IToREBEL6 : public RV32IToRV32IBase
{

#define REBEL6_TRITS_PER_TRYTE 6
#define REBEL6_TRITS_PER_INSTRUCTION 32
#define REBEL6_JUMPS_PER_INSTRUCTION 1

public:
   RV32IToREBEL6();
   virtual ~RV32IToREBEL6() = default;

private:
   void fillInstructionMap();
};
}
