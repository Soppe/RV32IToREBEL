#include "instruction.h"

#include <iostream>

namespace Expressions
{

Instruction::Instruction(const std::string& instructionName, const std::vector<std::string>& instructionOperands)
    : m_operands(instructionOperands)
{
   m_name.resize(instructionName.length());
   std::transform(instructionName.begin(), instructionName.end(), m_name.begin(), [](unsigned char c){ return std::tolower(c); });
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
