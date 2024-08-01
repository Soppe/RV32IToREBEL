#pragma once

#include "rv32ipseudotorv32ibase.h"

namespace Converters
{
class RV32IToREBEL6 : public RV32IPseudoToRV32IBase
{
public:
   RV32IToREBEL6();
   virtual ~RV32IToREBEL6() = default;

private:
   void fillExpressionMap();
};
}
