#ifndef TERNARYOPERANDCONVERTER_H
#define TERNARYOPERANDCONVERTER_H

#include <Expressions/expression.h>

class ExpressionParser;

namespace Expressions
{
class Instruction;
}

class TernaryOperandConverter
{
public:
   TernaryOperandConverter(Expressions::ExpressionList& ternaryExpressions);
   ~TernaryOperandConverter();

   void convertOperands();

private:
   void convertRegister(const Expressions::Instruction* instruction);
   void convertImmediate(const Expressions::Instruction* instruction);

   ExpressionParser* m_parser;
};

#endif // TERNARYOPERANDCONVERTER_H
