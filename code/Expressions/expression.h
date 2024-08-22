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
   virtual std::ostream& printToStream(std::ostream& stream) const = 0;

protected:
};


using ExpressionList = std::list<Expression*>;
}

inline std::ostream& operator<<(std::ostream& file, const Expressions::Expression& expr)
{
   return expr.printToStream(file);
}
