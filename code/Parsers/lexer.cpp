#include <string>
#include "lexer.h"
#include "token.h"

#define EOFConstant 3 // Using 0 is annoying for debugging.

Lexer::Lexer(const std::string& source)
   : m_source(source)
   , m_pos(0)
{
   nextChar(); // Initialize lexer with first character.
}

void Lexer::nextChar()
{
   if(m_pos >= m_source.length())
   {
      m_ch = EOFConstant;
   }
   else
   {
      m_ch = m_source[m_pos];
   }
   m_pos++;
}

char Lexer::peekChar()
{
   if(m_pos + 1 >= m_source.length())
   {
      return EOFConstant;
   }
   return m_source[m_pos + 1];
}

Token* Lexer::nextToken()
{
   Token *t = new Token();
   skipSpaces();
   t->literal = std::string(1, m_ch);

   switch(m_ch)
   {
   case '\r': case '\n': case ';':
      t->type = TokenType::Newline;
      // Reduce consecutive newlines to one (even if spaces are between them).
      while(isNewline() || isSpace())
         nextChar();
      break;
   case '+':
      t->type = TokenType::Plus;
      nextChar();
      break;
   case '-':
      t->type = TokenType::Minus;
      nextChar();
      break;
   case '(':
      t->type = TokenType::Lparen;
      nextChar();
      break;
   case ')':
      t->type = TokenType::Rparen;
      nextChar();
      break;
   case '.':
      t->type = TokenType::Dot;
      nextChar();
      break;
   case ',':
      t->type = TokenType::Comma;
      nextChar();
      break;
   case ':':
      t->type = TokenType::Colon;
      nextChar();
      break;
   case '#':
      t->type = TokenType::Comment;
      nextChar();
      while(!isNewline())
      { // Continue until newline.
         t->literal += m_ch;
         nextChar();
      }
      break;
   case EOFConstant: case 0:
      t->type = TokenType::EndOfFile;
      nextChar(); // Shouldn't matter.
      break;
   default:
      t->type = TokenType::Symbol;
      // Either an instruction name, directive name, string literal, or number.
      if(m_ch == '"')
      {
         // String literal.
         nextChar();
         while(m_ch != '"')
         {
            t->literal += m_ch;
            nextChar();
         }
         t->literal += m_ch; // Include ending quotation mark.
         nextChar();
      }
      else
      {
         nextChar();
         while(isAlpha() || isNum() || isPunct())
         {
            t->literal += m_ch;
            nextChar();
         }
      }
   }
   return t;
}

bool Lexer::isPunct()
{
   // Some assemblers allow $`._'
   return m_ch == 36 || m_ch == 39 || m_ch == 46 || m_ch == 95 || m_ch == 96;
}

bool Lexer::isAlpha()
{
   return (m_ch >= 65 && m_ch <= 90) || (m_ch >= 97 && m_ch <= 122);
}

bool Lexer::isNum()
{
   return m_ch >= 48 && m_ch <= 57;
}

bool Lexer::isNewline()
{
   return m_ch == '\r' || m_ch == '\n' || m_ch == ';';
}

bool Lexer::isSpace()
{
   return m_ch == ' ' || m_ch == '\t';
}

void Lexer::skipSpaces()
{
   while(isSpace())
   {
      nextChar();
   }
}
