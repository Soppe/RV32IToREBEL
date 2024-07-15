#ifndef RV32IPSEUDOTORV32IBASE_H
#define RV32IPSEUDOTORV32IBASE_H

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

#endif // RV32IPSEUDOTORV32IBASE_H
