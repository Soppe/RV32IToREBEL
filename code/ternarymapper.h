#ifndef TERNARYMAPPER_H
#define TERNARYMAPPER_H

#include <list>

namespace Expressions
{
class Instruction;
}

namespace Converters
{
class InstructionConverterBase;
}

class Expression;
class ExpressionParser;

class TernaryMapper
{
public:
   TernaryMapper(std::list<Expression*>& binaryExpressions);
   ~TernaryMapper();

   void mapExpressions(std::list<Expression*>& ternaryExpressions);

private:
   void mapInstruction(const Expressions::Instruction* instruction, std::list<Expression*>& ternaryExpressions);

   ExpressionParser* m_parser;
   const Converters::InstructionConverterBase* m_converter;
};

#endif // TERNARYMAPPER_H
