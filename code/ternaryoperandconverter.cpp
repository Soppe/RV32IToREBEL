#include "ternaryoperandconverter.h"

#include "expressionparser.h"

#include <Expressions/all_expressions.h>

TernaryOperandConverter::TernaryOperandConverter(std::list<Expression*>& ternaryExpressions)
    : m_parser(new ExpressionParser(ternaryExpressions))
{

}

TernaryOperandConverter::~TernaryOperandConverter()
{
   delete m_parser;
}

void TernaryOperandConverter::convertOperands()
{
   const Expression* expression = m_parser->nextExpression();

   while(expression != nullptr)
   {
      if(expression->getExpressionType() == Expression::ExpressionType::INSTRUCTION)
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
