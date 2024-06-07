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

   virtual void initMap();
};
}
#endif // RV32ITOREBEL2_H
