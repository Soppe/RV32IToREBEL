#pragma once

#include "expression.h"

#include <vector>

namespace Expressions
{
class Directive : public Expression
{
public:

   Directive(const std::string& directiveName, const std::vector<std::string>& directiveParameters);
   virtual ~Directive() = default;

   virtual ExpressionType getExpressionType() const;
   virtual std::ostream& printToStream(std::ostream& stream) const;

   const std::string& getDirectiveName() const;
   const std::vector<std::string>& getDirectiveParameters() const;

private:

   std::string m_name;
   std::vector<std::string> m_parameters;
};
}
