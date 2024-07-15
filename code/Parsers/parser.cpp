#include <iostream>
#include <cstdlib>
#include "parser.h"
#include "lexer.h"
#include "Expressions/all_expressions.h"

Parser::Parser(Lexer* l)
   : m_l(l)
   , m_token(nullptr)
   , m_peek(nullptr)

{
   nextToken(); // Call twice to initialize token and peek.
   nextToken();
}

Parser::~Parser()
{
   delete m_token;
   delete m_peek;
}

bool Parser::checkToken(TokenType type)
{
   return m_token->type == type;
}

bool Parser::checkPeek(TokenType type)
{
   return m_peek->type == type;
}

void Parser::match(TokenType type)
{
   if(!checkToken(type))
   {
      std::cout << "Expected " << TokenTypeStrings[static_cast<int>(type)] << " but found " << m_token->literal << ".\n";
      abort();
   }
   nextToken();
}

void Parser::nextToken()
{
   delete m_token;
   m_token = m_peek;
   m_peek = m_l->nextToken();
}

//TODO: Update this according to actual documentation found at https://sourceware.org/binutils/docs/as/ (chapter 5) and https://github.com/riscv-non-isa/riscv-asm-manual/blob/master/riscv-asm.md
void Parser::parse(std::list<Expressions::Expression*>& expressions)
{
   while(!checkToken(TokenType::EndOfFile))
   {
      // Is it a label?
      if(checkPeek(TokenType::Colon))
      {
         std::cout << "Checking label = " << m_token->literal << std::endl;
         Expressions::Label* l = getLabel(false);
         expressions.push_back(l);
      }

      // Is it a directive or an auto-generated label?
      else if(checkToken(TokenType::Dot))
      {
         std::cout << "Checking label or directive" << std::endl;
         Expressions::Expression* e = getLabelOrDirective();
         expressions.push_back(e);
      }

      else if(checkToken(TokenType::Symbol))
      {
         std::cout << "Checking symbol = " << m_token->literal << std::endl;
         Expressions::Instruction* instruction = getInstruction();
         /*if(instruction->getInstructionType() == Expressions::Instruction::InstructionType::PSEUDO)
         {
            resolvePseudoInstruction(instruction, expressions);
         }
         else
         {*/
            expressions.push_back(instruction);
         //}
      }

      // Is it a comment?
      else if(checkToken(TokenType::Comment))
      {
         //std::cout << "Checking comment = " << m_token->literal << std::endl;
         nextToken();
      }

      //std::cout << "Checking newline = " << m_token->literal << std::endl;
      match(TokenType::Newline);
   }
}

Expressions::Label* Parser::getLabel(bool includeDot)
{
   std::string labelName;
   if(includeDot)
   {
      labelName += ".";
   }
   labelName += m_token->literal;

   Expressions::Label* l = new Expressions::Label(labelName);

   match(TokenType::Symbol);
   nextToken(); // Bypass the colon

   return l;
}

Expressions::Directive* Parser::getDirective(bool includeDot)
{
   std::string directiveName;

   if(checkToken(TokenType::Dot))
   {
      directiveName += m_token->literal;
      std::cout << "Printing dot token = " << m_token->literal << std::endl;;
      nextToken(); // Consume the leading dot.
   }
   else if(includeDot)
   {
      directiveName += ".";
   }

   directiveName += m_token->literal;

   //std::cout << "Printing attrivute name = " << m_token->literal << std::endl;
   match(TokenType::Symbol);

   std::vector<std::string> operands;
   // Is there at least one operand?
   if(!checkToken(TokenType::Newline) && !checkToken(TokenType::Comment))
   {
      std::string operand;
      getOperand(operand);
      // Does the first operand have a Dot, Plus, or minus?
      /*while(CheckToken(TokenType::Dot) || CheckToken(TokenType::Plus) || CheckToken(TokenType::Minus))
      {
         operand += m_token->literal;
         NextToken();
      }
      operand += m_token->literal;
      Match(TokenType::Symbol);*/
      operands.push_back(operand);
      // Zero or more operands.
      while(checkToken(TokenType::Comma))
      {
         nextToken(); // Bypass comma

         getOperand(operand);
         operands.push_back(operand);
         /*NextToken();
         // Is there a dot, plus, or minus?
         while(CheckToken(TokenType::Dot) || CheckToken(TokenType::Plus) || CheckToken(TokenType::Minus))
         {
            NextToken();
         }
         Match(TokenType::Symbol);*/
      }
   }

   return new Expressions::Directive(directiveName, operands);
;
}

Expressions::Expression* Parser::getLabelOrDirective()
{
   Expressions::Expression* retval = nullptr;
   std::string str = m_token->literal;

   nextToken(); // Bypass the dot
   if(checkPeek(TokenType::Colon))
   {
      Expressions::Label* l = getLabel(true);
      retval = l;
   }
   else
   {
      Expressions::Directive* d = getDirective(true);
      retval = d;
   }

   return retval;
}

Expressions::Instruction* Parser::getInstruction()
{
   Expressions::Instruction* instr = nullptr;

   const std::string instructionName = m_token->literal;
   nextToken();

   std::vector<std::string> operands;
   std::string operand;

   // Is there at least one operand?
   if(!checkToken(TokenType::Newline) && !checkToken(TokenType::Comment))
   {
      getOperand(operand);
      operands.push_back(operand);
      // Zero or more operands.
      while(checkToken(TokenType::Comma))
      {
         operand = "";
         nextToken();
         getOperand(operand);
         operands.push_back(operand);
      }
   }

   instr = new Expressions::Instruction(instructionName, operands);

   return instr;
}

void Parser::getOperand(std::string& operand) {
   // Optional sign, or dot referring to a label.
   while(checkToken(TokenType::Dot) || checkToken(TokenType::Plus) || checkToken(TokenType::Minus))
   {
      operand += m_token->literal;
      // std::cout << "Found first operand = " << m_token->literal << std::endl;
      nextToken();
   }

   // Only a symbol.
   if(checkToken(TokenType::Symbol) && !checkPeek(TokenType::Lparen))
   {
      operand += m_token->literal;
      // std::cout << "Found second operand = " << m_token->literal << std::endl;
      nextToken();
   }
   // symbol '(' symbol ')'
   else
   {
      // std::cout << "Found third operand = " << m_token->literal << std::endl;
      operand += m_token->literal;
      match(TokenType::Symbol);
      operand += m_token->literal;
      match(TokenType::Lparen);
      if(checkToken(TokenType::Dot)) // Sometimes the label can be one auto-generated by GCC, e.g. %pcrel_hi(.L0)
      {
         operand += m_token->literal;
         match(TokenType::Dot);
      }
      operand += m_token->literal;
      match(TokenType::Symbol);
      operand += m_token->literal;
      match(TokenType::Rparen);

      if(checkToken(TokenType::Lparen)) // In the case where the offset itself comes from a label, e.g. %hi(LABEL_AS_ADDRESS_OFFSET)(a0)
      {
         operand += m_token->literal;
         match(TokenType::Lparen);
         operand += m_token->literal;
         match(TokenType::Symbol);
         operand += m_token->literal;
         match(TokenType::Rparen);
      }
   }
}

/*void Parser::resolvePseudoInstruction(Expressions::Instruction* i, std::list<const Expression*>& expressions)
{
   Expressions::Instruction* first = nullptr;
   Expressions::Instruction* second = nullptr;
   Expressions::Instruction::resolvePseudoInstruction(i, first, second);

   if(first != nullptr)
   {
      expressions.push_back(first);

      if(second != nullptr)
      {
         expressions.push_back(second);
      }
   }
}*/
