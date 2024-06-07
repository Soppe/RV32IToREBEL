#include "expressionparser.h"
#include <Expressions/all_expressions.h>

ExpressionParser::ExpressionParser(std::list<Expression*>& expressions)
   : m_expressions(expressions)
   , m_iter(expressions.begin())
{
}

Expression* ExpressionParser::nextExpression()
{
   Expression* e = nullptr;

   if(m_iter != m_expressions.end())
   {
      e = *m_iter;
      m_iter++;
   }

   return e;
}

void ExpressionParser::insertAtCurrent(Expression* expression)
{
   m_expressions.insert(m_iter, expression);
}
