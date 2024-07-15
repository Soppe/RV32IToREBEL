// Example parser for RISC-V assembly.
// Austin Henley
// 10/5/2020


#include "ternarymapper.h"
#include "ternaryoperandconverter.h"

#include "Converters/converter.h"

#include "Parsers/lexer.h"
#include "Parsers/token.h"
#include "Parsers/parser.h"

#include "Expressions/expression.h"
#include "Expressions/all_expressions.h"

#include "Simulators/RV32I/simulator.h"
#include "Simulators/RV32I/assemblerandlinker.h"
#include "Simulators/RV32I/executableprogram.h"

#include <iostream>
#include <fstream>
#include <filesystem>


namespace
{
void printOperands(const std::vector<std::string>& operands)
{
   for(int i = 0; i < operands.size(); ++i)
   {
      if(i != 0)
      {
         std::cout << ",";
      }

      std::cout << " " << operands[i];
   }
}

void printExpressions(const Expressions::ExpressionList& expressions)
{
   int pc = 0;
   for(const Expressions::Expression* e: expressions)
   {
      std::cout << std::hex << pc << std::dec << "\t";
      switch(e->getExpressionType())
      {
      case Expressions::Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* d = static_cast<const Expressions::Directive*>(e);
         std::cout << "\t";
         d->print();
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* l = static_cast<const Expressions::Label*>(e);
         l->print();
         break;
      }
      case Expressions::Expression::ExpressionType::INSTRUCTION:
      {
         const Expressions::Instruction* i = static_cast<const Expressions::Instruction*>(e);
         std::cout << "\t";
         i->print();
         pc += 4;
         break;
      }
      case Expressions::Expression::ExpressionType::COMMENT:
         //TODO: std::cout << e->getExpressionName();
         break;
      case Expressions::Expression::ExpressionType::UNDEFINED:
         // TODO: std::cerr << "Found undefined expression with name = " << e->getExpressionName() << std::endl;
         break;
      default:
         std::cerr << "Unsupported expression type with value = " << static_cast<int>(e->getExpressionType()) << std::endl;
      }

      /*if(peekNextExpression == comment && e->lineNumber == next->lineNumber)
      {
         Print comment
             m_helper->nextExpression(); // Skip comment
      }
      else
      {*/
      std::cout << std::endl;
      //}
   }
}
}

int main(int argc, char* argv[])
{
   if(argc != 2)
   {
      std::cout << "Needs one file path." << std::endl;
      return -1;
   }

   std::string inputPath = argv[1];

   std::ifstream inFile(inputPath);
   std::string source((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
   source += '\n';

   /*
    * ------------------ FRONTEND ------------------
    */

   Lexer* l = new Lexer(source);
   Parser p(l);
   Expressions::ExpressionList binaryExpressions;
   p.parse(binaryExpressions);
   std::cout << "Parsed successfully." << std::endl;

   /*
    * --------- INTERMEDIATE REPRESENTAITON ---------
    */
   // NA - The RV32I input is currently considered the IR code.


   /*
    * ------------------- BACKEND -------------------
    */

   // RV32I
   std::cout << "RV32I SIMULATION" << std::endl;
   Expressions::ExpressionList rv32iExpressions;
   Converters::Converter::Convert("rv32i", binaryExpressions, rv32iExpressions);

   Simulators::RV32I::ExecutableProgram rv32iExecutable;
   Simulators::RV32I::AssemblerAndLinker rv32iAssembler(rv32iExpressions, rv32iExecutable);
   rv32iAssembler.init();
   rv32iAssembler.run();

   std::string fileName = std::filesystem::path(inputPath).stem();
   std::cout << "Path name = " << inputPath << "; fileName = " << fileName << std::endl;
   //rv32iExecutable.generateAssemblyFileForMRCS(filename + ".mbo"); // Output as binary object file for MRCS. mbo = MRCS Binary Object file;

   Simulators::RV32I::Simulator rv32iSim;
   rv32iSim.run(rv32iExecutable);

   // REBEL-6
   Expressions::ExpressionList rebel2Expressions;
   //Converters::Converter::Convert("rebel-2", binaryExpressions, rebel2Expressions);

   TernaryMapper mapper(binaryExpressions);
   Expressions::ExpressionList ternaryExpressions;
   //mapper.mapExpressions(ternaryExpressions);


   // TODO: "Convert" immediates, as in check if the immediate can be converted by the single REBEL-2 instruction its converted to, or if that instruction itself need to be split up because of the value of
   // the constant being too large for ternary instructions. For this we need to know the size of pointers as this is basically what we us when using %hi/lo and %pcrel_hi/lo.
   // Include constants, ignore addresses, labels, and assembler modifiers for now. Do take into consideration that address size is defined, and that every occurence of using an address/label need to be resolved
   // based on its size and not value. Every occurence related to an address should be related to some assembler modifier instruction. Ergo, we need to decide beforehand if the paired assembler modifiers (hi/lo)
   // can be reused for ternary too, or if the value they combined can express exceeds what is supported by REBEL-2 and a potential third assembler modifier instruction has to be used. If so, this modifier
   // should be added before/after the use of one of the (hi) assembler modifiers. Alternatively we keep the existing ones and repurpose what they do, or even cut them down to only 1 modifier if REBEL-2
   // instructions can cover the full 32 bits of a RV32I value.
   //TernaryOperandConverter operandConverter(symbolTable);
   //operandConverter.convert(ternaryExpressions);

   // TODO: Convert registers

   // TODO: Redundancy check - can probably be skipped if the previous steps did not add any new instruction. That said, might look for possible uses for addi2.

   // TODO: Make the rest of the symbol table with ternary instruction size and ternary trit/tryte sizes as base. E.g. an array with 400 elements of 4 bytes each might have to become 400 elements of 3 trytes
   // or something.
   // .word, .byte, .set, .size, .half, .string, .ascii are words that need to be supported here to find size of constants. Turns out if .size is removed, their related labels get size 0
   // after assembling and checking using the readelf -s command. , .zero seems to be related to the amount of bytes that will be 0, and so to calculate elements one has to divide this by element size.
   // Chars are converted to .string, and it seems the .zero attribute is .size - defined elements - 1 (terminating zero), except if the array has no defined elements - .zero == .size then.
   // Strings, like char arrays with individual elements, are defined by the .string attribute where the value is the same as the string, and its size is the length of the string including terminating zero.
   // For .string and .ascii, each element takes 1 byte
   // All heap variables have a .size, as does functions, except auto-generated ones as they are part of functions. If .size can be converted to an int its a heap thing. If not its a label.. probably.
   //SymbolTable symbolTable;
   //symbolTable.resolveConstants(ternaryExpressions); // Find all .size variables and store the ones whose size can be converted to an int. They all go on the heap, meaning they can also be attributed an address.

   // TODO: Resolve addresses, labels, and assembler modifiers.

   // TODO: Since the size of an address/label/start location of an array is preset through address size, we don't have to redo any of the above steps

   // TODO: Ternary optimizer - god knows what to do... optimize?

   // "Assembly"
   // TODO: Ternary "assembler", formatting the ternary code to intended output. Handle labels and % commands here by creating a local symbol table, with each symbol taking 2 trits?

   std::cout << "BINARY EXPRESSIONS: " << std::endl;
   //printExpressions(binaryExpressions);

   std::cout << "TERNARY EXPRESSIONS: " << std::endl;
   //printExpressions(ternaryExpressions);

   binaryExpressions.remove_if([](const Expressions::Expression* expr) { delete expr; return true;});
   ternaryExpressions.remove_if([](const Expressions::Expression* expr) { delete expr; return true;});
   delete l;
   return 0;
}
