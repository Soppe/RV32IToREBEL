#pragma once

#include "expression.h"

namespace Expressions
{
class Label : public Expression
{
public:

   Label(const std::string& labelName);
   virtual ~Label() = default;

   virtual ExpressionType getExpressionType() const;
   virtual std::ostream& printToStream(std::ostream& stream) const;

   const std::string& getLabelName() const;

private:
   std::string m_name;
};
}
