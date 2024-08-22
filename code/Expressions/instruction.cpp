#include "instruction.h"

#include <iostream>

namespace Expressions
{

Instruction::Instruction(const std::string& instructionName, const std::vector<std::string>& instructionOperands)
    : m_operands(instructionOperands)
{
   m_name.resize(instructionName.length());
   std::transform(instructionName.begin(), instructionName.end(), m_name.begin(), [](std::uint8_t c){ return std::tolower(c); });
}

Expression::ExpressionType Instruction::getExpressionType() const
{
   return ExpressionType::INSTRUCTION;
}

std::ostream& Instruction::printToStream(std::ostream& stream) const
{
   stream << m_name << " ";

   for(std::int32_t i = 0; i < m_operands.size(); ++i)
   {
      if(i != 0)
      {
         stream << ",";
      }
      stream << m_operands[i];
   }

   return stream;
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
