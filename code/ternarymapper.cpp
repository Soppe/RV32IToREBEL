#include "ternarymapper.h"
#include "expressionparser.h"

#include <Expressions/expression.h>
#include <Expressions/all_expressions.h>
#include <Converters/rv32itorebel2.h>

#include <iostream>
#include <iterator>

TernaryMapper::TernaryMapper(std::list<Expression*>& binaryExpressions)
   : m_parser(new ExpressionParser(binaryExpressions))
   , m_converter(new Converters::RV32IToREBEL2())
{

}

TernaryMapper::~TernaryMapper()
{
   delete m_parser;
   delete m_converter;
}

void TernaryMapper::mapExpressions(std::list<Expression*>& ternaryExpressions)
{
   Expression* binaryExpression = m_parser->nextExpression();

   while(binaryExpression != nullptr)
   {
      switch(binaryExpression->getExpressionType())
      {
      case Expression::ExpressionType::DIRECTIVE:
      {
         Expressions::Directive* d = static_cast<Expressions::Directive*>(binaryExpression);
         ternaryExpressions.push_back(new Expressions::Directive(*d));
         break;
      }
      case Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* l = static_cast<const Expressions::Label*>(binaryExpression);
         ternaryExpressions.push_back(new Expressions::Label(*l));
         break;
      }
      case Expression::ExpressionType::INSTRUCTION:
      {
         const Expressions::Instruction* i = static_cast<const Expressions::Instruction*>(binaryExpression);
         mapInstruction(i, ternaryExpressions);
         break;
      }
      case Expression::ExpressionType::COMMENT:
         //TODO: std::cout << e->getExpressionName();
         break;
      case Expression::ExpressionType::UNDEFINED:
         // TODO: std::cerr << "Found undefined expression with name = " << e->getExpressionName() << std::endl;
         break;
      default:
         std::cerr << "Unsupported expression type with value = " << static_cast<int>(binaryExpression->getExpressionType()) << std::endl;
         break;
      }

      /*if(peekNextExpression == comment && e->lineNumber == next->lineNumber)
      {
         Print comment
             m_helper->nextExpression(); // Skip comment
      }
      else
      {*/
      //}

      binaryExpression = m_parser->nextExpression();
   }
}


void TernaryMapper::mapInstruction(const Expressions::Instruction* binaryInstruction, std::list<Expression*>& ternaryExpressions)
{
   const std::string& name = binaryInstruction->getInstructionName();
   const std::vector<std::string>& operands = binaryInstruction->getInstructionOperands();

   //TODO: REMEMBER TO DEAL WITH SIGN EXTENSIONS FOR COMMANDS LIKE LUI AND THE USE OF IMMEDIATE OFFSET VALUES.
   //TODO: REMEMBER TO TAKE SIGNED VALUES INTO ACCOUNT.
   // For both, see https://www.youtube.com/watch?v=ROkjMXuR2u8 around minute 10+.

   try
   {
      m_converter->at(name)(operands, ternaryExpressions);
   }
   catch(const std::exception&e)
   {
      std::cerr << __PRETTY_FUNCTION__ << " Instruction (" << name << " ";
      std::copy(operands.begin(), operands.end(), std::ostream_iterator<const std::string&>(std::cerr, " "));
      std::cerr << ") failed to convert because " << e.what() << std::endl;
   }
}
