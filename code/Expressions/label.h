#ifndef LABEL_H
#define LABEL_H

#include "expression.h"

namespace Expressions
{
class Label : public Expression
{
public:

   Label(const std::string& labelName);
   virtual ~Label() = default;

   virtual ExpressionType getExpressionType() const;
   virtual void print() const;

   const std::string& getLabelName() const;

private:
   std::string m_name;
};
}

#endif // LABEL_H
