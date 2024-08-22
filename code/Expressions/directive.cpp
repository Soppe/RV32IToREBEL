#include "directive.h"

#include <iostream>

namespace Expressions
{

Directive::Directive(const std::string& directiveName, const std::vector<std::string>& directiveParameters)
    : m_parameters(directiveParameters)
{
   m_name.resize(directiveName.length());
   std::transform(directiveName.begin(), directiveName.end(), m_name.begin(), [](std::uint8_t c){ return std::tolower(c); });
}

Expression::ExpressionType Directive::getExpressionType() const
{
   return ExpressionType::DIRECTIVE;
}

std::ostream& Directive::printToStream(std::ostream& stream) const
{
   stream << m_name << " ";

   for(std::int32_t i = 0; i < m_parameters.size(); ++i)
   {
      if(i != 0)
      {
         stream << ", ";
      }
      stream << m_parameters[i];
   }

   return stream;
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
