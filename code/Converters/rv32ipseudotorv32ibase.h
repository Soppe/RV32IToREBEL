#pragma once

#include "instructionconverterbase.h"

namespace Converters
{
class RV32IPseudoToRV32IBase : public InstructionConverterBase
{
public:
   RV32IPseudoToRV32IBase();
   virtual ~RV32IPseudoToRV32IBase() = default;

private:
   void fillExpressionMap();
};
}
