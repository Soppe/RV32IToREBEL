#include "instruction.h"

#include <iostream>
#include <unordered_map>

namespace Expressions
{

Instruction::Instruction(const std::string& instructionName, const std::vector<std::string>& instructionOperands)
    : m_name(instructionName)
    , m_operands(instructionOperands)
{
}

Expression::ExpressionType Instruction::getExpressionType() const
{
   return ExpressionType::INSTRUCTION;
}

void Instruction::print() const
{
   std::cout << m_name << "\t";

   for(int i = 0; i < m_operands.size(); ++i)
   {
      if(i != 0)
      {
         std::cout << ",";
      }
      std::cout << m_operands[i];
   }
}

const std::string& Instruction::getInstructionName() const
{
   return m_name;
}

const std::vector<std::string>& Instruction::getInstructionOperands() const
{
   return m_operands;
}

std::vector<std::string>& Instruction::getInstructionOperands()
{
   return m_operands;
}
}
