#pragma once

#include "lexer.h"

#include <Expressions/expression.h>

namespace Expressions
{
struct Label;
struct Directive;
struct Instruction;
}

class Parser
{
public:
   Parser(Lexer* l);
   ~Parser();
   // Helper functions.
   bool checkToken(TokenType type);
   bool checkPeek(TokenType type);
   void match(TokenType type);
   void nextToken();

   // Grammar functions.
   void parse(Expressions::ExpressionList& expressions);

private:
   Expressions::Label* getLabel(bool includeDot);
   Expressions::Directive* getDirective(bool includeDot);
   Expressions::Instruction* getInstruction();
   Expressions::Expression* getLabelOrDirective();
   void getOperand(std::string& operand);

   Lexer* m_l;
   Token* m_token;
   Token* m_peek;
};
