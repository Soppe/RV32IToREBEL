#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include <string>
#include <list>

namespace Expressions
{
class Instruction;
}
class Expression;

class ExpressionParser
{
public:

   ExpressionParser(const std::list<Expression*>& expressions);
   ~ExpressionParser() = default;

   const Expression* nextExpression();
   void insertAtCurrent(Expression* expression);

private:
   using ExpressionIterator = std::list<Expression*>::const_iterator;
   const std::list<Expression*>& m_expressions;
   ExpressionIterator m_iter;
};

#endif // EXPRESSIONPARSER_H
