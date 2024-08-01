#pragma once

#include <Expressions/expression.h>

namespace Converters
{
class InstructionConverterBase;
}

namespace Expressions
{
class Instruction;
}

class ExpressionParser;

class TernaryMapper
{
public:
   TernaryMapper(Expressions::ExpressionList& binaryExpressions);
   ~TernaryMapper();

   void mapExpressions(Expressions::ExpressionList& ternaryExpressions);

private:
   void mapInstruction(const Expressions::Instruction* instruction, Expressions::ExpressionList& ternaryExpressions);

   ExpressionParser* m_parser;
   const Converters::InstructionConverterBase* m_converter;
};
