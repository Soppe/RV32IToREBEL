#include "directive.h"

#include <iostream>

namespace Expressions
{

Directive::Directive(const std::string& directiveName, const std::vector<std::string>& directiveParameters)
    : m_name(directiveName)
    , m_parameters(directiveParameters)
{
}

Expression::ExpressionType Directive::getExpressionType() const
{
   return ExpressionType::DIRECTIVE;
}

void Directive::print() const
{
   std::cout << m_name << "\t";

   for(int i = 0; i < m_parameters.size(); ++i)
   {
      if(i != 0)
      {
         std::cout << ",";
      }
      std::cout << m_parameters[i];
   }
}

const std::string& Directive::getDirectiveName() const
{
   return m_name;
}

const std::vector<std::string>& Directive::getDirectiveParameters() const
{
   return m_parameters;
}

}