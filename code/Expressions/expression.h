#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

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


#endif // EXPRESSION_H
