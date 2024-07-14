#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType
{
   Symbol = 0,
   Lparen,
   Rparen,
   Plus,
   Minus,
   Dot,
   Colon,
   Comma,
   Newline,
   Comment,
   EndOfFile
};

static const char* const TokenTypeStrings[] = {"Symbol", "Left-Parenthesis", "Right-Parenthesis", "Plus", "Minus", "Dot", "Colon", "Comma", "Newline", "Comment", "End-Of-File"};

class Token
{
public:
   TokenType type;
   std::string literal;
};

#endif // TOKEN_H
