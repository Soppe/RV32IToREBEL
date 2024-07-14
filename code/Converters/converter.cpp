#include "converter.h"

#include "rv32ipseudotorv32ibase.h"
#include "rv32itorebel2.h"

#include <Expressions/all_expressions.h>
#include <Parsers/expressionparser.h>

#include <iostream>

namespace Converters
{
void Converter::Convert(const std::string& targetISAName, const ExpressionList& from, ExpressionList& to)
{
   InstructionConverterBase* converter = nullptr;
   ExpressionParser parser(from);
   std::string isaName;
   std::transform(targetISAName.begin(), targetISAName.end(), isaName.begin(), [](unsigned char c){ return std::tolower(c); });


   if(isaName == "rv32i")
   {
      converter = new RV32IPseudoToRV32IBase();
   }
   else if((isaName == "rebel2") || (isaName == "rebel-2"))
   {
      converter = new RV32IToREBEL2();
   }
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << "Trying to convert unsupported ISA named " << targetISAName << std::endl;
      abort();
   }

   converter->initMap();

   const Expression* expr = parser.nextExpression();
   while(expr != nullptr)
   {
      switch(expr->getExpressionType())
      {
      case Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* dir = static_cast<const Expressions::Directive*>(expr);
         to.push_back(new Expressions::Directive(*dir));
         break;
      }
      case Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* label = static_cast<const Expressions::Label*>(expr);
         to.push_back(new Expressions::Label(*label));
         break;
      }
      case Expression::ExpressionType::INSTRUCTION:
      {
         const Expressions::Instruction* instr = static_cast<const Expressions::Instruction*>(expr);
         resolveInstruction(instr, to, *converter);
         break;
      }

      default:
         // Don't care
         break;
      }

      expr = parser.nextExpression();
   }
}

void Converter::resolveInstruction(const Expressions::Instruction* instr, std::list<Expression*>& out, Converters::InstructionConverterBase& converter)
{
   const std::string& name = instr->getInstructionName();
   const std::vector<std::string>& operands = instr->getInstructionOperands();

   try
   {
      /*std::cout << "Before conversion: ";
      instr->print();
      std::cout << std::endl;*/
      converter.at(name)(operands, out);
      /*std::cout << "After conversion: ";
      out.back()->print();
      std::cout << std::endl;*/
   }
   catch(const std::exception&e)
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Failed to convert \"";
      instr->print();
      std::cerr << "\": " << e.what() << std::endl;
   }
}
}
