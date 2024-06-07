#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "expression.h"

#include <vector>

namespace Expressions
{
class Instruction : public Expression
{
public:
   Instruction(const std::string& instructionName, const std::vector<std::string>& instructionOperands);
   virtual ~Instruction() = default;

   virtual ExpressionType getExpressionType() const;
   virtual void print() const;

   const std::string& getInstructionName() const;
   const std::vector<std::string>& getInstructionOperands()const ;
   std::vector<std::string>& getInstructionOperands();

private:

   std::string m_name;
   std::vector<std::string> m_operands;
};
}

#endif // INSTRUCTION_H
