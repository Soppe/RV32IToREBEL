#pragma once

#include <string>
#include <list>

namespace Expressions
{

class Expression
{
public:
   enum class ExpressionType{DIRECTIVE, INSTRUCTION, LABEL, COMMENT, UNDEFINED};

   Expression() = default;
   virtual ~Expression() = default;

   virtual ExpressionType getExpressionType() const = 0;
   virtual void print() const = 0;

protected:
};

using ExpressionList = std::list<Expression*>;
}
