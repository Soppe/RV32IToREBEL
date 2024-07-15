#include "expressionparser.h"
#include <Expressions/all_expressions.h>

ExpressionParser::ExpressionParser(const Expressions::ExpressionList& expressions)
   : m_expressions(expressions)
   , m_iter(expressions.begin())
{
}

const Expressions::Expression* ExpressionParser::nextExpression()
{
   const Expressions::Expression* e = nullptr;

   if(m_iter != m_expressions.end())
   {
      e = *m_iter;
      m_iter++;
   }

   return e;
}

void ExpressionParser::insertAtCurrent(Expressions::Expression* expression)
{
   //m_expressions.insert(m_iter, expression);
}
