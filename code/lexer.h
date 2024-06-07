#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "token.h"

class Lexer
{
public:
   Lexer(const std::string& source);
   Token* nextToken();

private:
   const std::string& m_source;
   char m_ch; // Current character.
   int m_pos; // Position of next character to be read.
   void nextChar();
   char peekChar();
   bool isPunct();
   bool isAlpha();
   bool isNum();
   bool isNewline();
   bool isSpace();
   void skipSpaces();
};

#endif //LEXER_H
