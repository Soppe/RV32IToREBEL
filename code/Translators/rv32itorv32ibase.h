#pragma once

#include "instructiontranslatorbase.h"

namespace Translators
{
class RV32IToRV32IBase : public InstructionTranslatorBase
{
public:
   RV32IToRV32IBase();
   virtual ~RV32IToRV32IBase() = default;

private:
   void fillInstructionMap();
};
}
