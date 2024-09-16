#include "converter.h"

#include "rv32ipseudotorv32ibase.h"
#include "rv32itorebel6.h"

#include <Expressions/all_expressions.h>
#include <Parsers/expressionparser.h>
#include <logger.h>

#include <iostream>

namespace
{
const Converters::RV32IPseudoToRV32IBase rv32i;
const Converters::RV32IToREBEL6 rebel6;

void resolveInstruction(const Expressions::Instruction* instr, Expressions::ExpressionList& out, const Converters::InstructionConverterBase& converter)
{
   const std::string& name = instr->getInstructionName();
   const std::vector<std::string>& operands = instr->getInstructionOperands();

   try
   {
      converter.at(name)(operands, out);
   }
   catch(const std::exception&e)
   {
      std::cerr << __PRETTY_FUNC__ << ": Failed to convert \"" << *instr << "\": " << e.what() << std::endl;
   }
}
}

namespace Converters
{
void Converter::Convert(const std::string& targetISAName, const Expressions::ExpressionList& from, Expressions::ExpressionList& to)
{
   const InstructionConverterBase* converter = nullptr;
   ExpressionParser parser(from);
   std::string isaName;
   isaName.resize(targetISAName.length());
   std::transform(targetISAName.begin(), targetISAName.end(), isaName.begin(), [](std::uint8_t c){ return std::tolower(c); });

   if(isaName == "rv32i")
   {
      converter = &rv32i;
   }
   else if((isaName == "rebel6") || (isaName == "rebel-6"))
   {
      converter = &rebel6;
   }
   else
   {
      std::cerr << __PRETTY_FUNC__ << "Trying to convert unsupported ISA named " << targetISAName << std::endl;
      abort();
   }

   const Expressions::Expression* expr = parser.nextExpression();
   while(expr != nullptr)
   {
      switch(expr->getExpressionType())
      {
      case Expressions::Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* dir = static_cast<const Expressions::Directive*>(expr);
         to.push_back(new Expressions::Directive(*dir));
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* label = static_cast<const Expressions::Label*>(expr);
         to.push_back(new Expressions::Label(*label));
         break;
      }
      case Expressions::Expression::ExpressionType::INSTRUCTION:
      {
         const Expressions::Instruction* instr = static_cast<const Expressions::Instruction*>(expr);
         resolveInstruction(instr, to, *converter);
         break;
      }
      case Expressions::Expression::ExpressionType::COMMENT:
         // Don't care
         break;

      default:
         // Don't care
         break;
      }

      expr = parser.nextExpression();
   }
}
}
