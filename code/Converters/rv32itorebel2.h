#ifndef RV32ITOREBEL2_H
#define RV32ITOREBEL2_H

#include "instructionconverterbase.h"

namespace Converters
{
class RV32IToREBEL2 : public InstructionConverterBase
{
public:
   RV32IToREBEL2();
   virtual ~RV32IToREBEL2() = default;

private:
   virtual void fillExpressionMap();
};
}
#endif // RV32ITOREBEL2_H
