#ifndef CONVERTER_H
#define CONVERTER_H

#include "instructionconverterbase.h"
#include <Expressions/instruction.h>

namespace Converters
{
class Converter
{
public:
   static void Convert(const std::string& targetISAName, const ExpressionList& from, ExpressionList& to);

private:
   static void resolveInstruction(const Expressions::Instruction* instr, std::list<Expression*>& out, Converters::InstructionConverterBase& converter);
};

}

#endif // CONVERTER_H
