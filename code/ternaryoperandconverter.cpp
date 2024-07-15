#include "ternaryoperandconverter.h"

#include <Parsers/expressionparser.h>
#include <Expressions/all_expressions.h>

TernaryOperandConverter::TernaryOperandConverter(Expressions::ExpressionList& ternaryExpressions)
    : m_parser(new ExpressionParser(ternaryExpressions))
{

}

TernaryOperandConverter::~TernaryOperandConverter()
{
   delete m_parser;
}

void TernaryOperandConverter::convertOperands()
{
   const Expressions::Expression* expression = m_parser->nextExpression();

   while(expression != nullptr)
   {
      if(expression->getExpressionType() == Expressions::Expression::ExpressionType::INSTRUCTION)
      {
         const Expressions::Instruction* instruction = static_cast<const Expressions::Instruction*>(expression);

         if(instruction->getInstructionName() == "addi")
         {

         }
         else if(instruction->getInstructionName() == "addi2")
         {

         }
         else if(instruction->getInstructionName() == "jal")
         {

         }
      }

      expression = m_parser->nextExpression();
   }
}

void TernaryOperandConverter::convertRegister(const Expressions::Instruction* instruction)
{

}
