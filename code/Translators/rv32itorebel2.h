#pragma once

#include "instructiontranslatorbase.h"

namespace Translators
{
class RV32IToREBEL2 : public InstructionTranslatorBase
{
public:
   RV32IToREBEL2();
   virtual ~RV32IToREBEL2() = default;

private:
   void fillInstructionMap();
};
}
