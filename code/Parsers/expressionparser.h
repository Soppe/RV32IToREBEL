#pragma once

#include <Expressions/expression.h>

namespace Expressions
{
class Instruction;
}

class ExpressionParser
{
public:

   ExpressionParser(const Expressions::ExpressionList& expressions);
   ~ExpressionParser() = default;

   const Expressions::Expression* nextExpression();
   void reset();
   void insertAtCurrent(Expressions::Expression* expression);

private:
   using ExpressionIterator = Expressions::ExpressionList::const_iterator;
   const Expressions::ExpressionList& m_expressions;
   ExpressionIterator m_iter;
};
