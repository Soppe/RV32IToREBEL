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

#include "Simulators/RV32I/assemblerandlinker.h"
#include "Simulators/RV32I/assemblerutils.h"
#include "Simulators/RV32I/executableprogram.h"
#include "Simulators/RV32I/simulator.h"

#include "Simulators/REBEL6/executableprogram.h"

#include <iostream>
#include <fstream>
#include <filesystem>


namespace
{
void printOperands(const std::vector<std::string>& operands)
{
   for(std::uint32_t i = 0; i < operands.size(); ++i)
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
   std::uint32_t pc = 0;
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
         std::cerr << "Unsupported expression type with value = " << static_cast<std::int32_t>(e->getExpressionType()) << std::endl;
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

   // ------------ RV32I------------

   // Conversion
   Expressions::ExpressionList rv32iExpressions;
   Converters::Converter::Convert("rv32i", binaryExpressions, rv32iExpressions);

   // Assembling
   Simulators::RV32I::ExecutableProgram rv32iExecutable;
   Simulators::RV32I::AssemblerAndLinker rv32iAssembler(rv32iExpressions, rv32iExecutable);
   rv32iAssembler.init();
   rv32iAssembler.run();

   // Simulation
   std::cout << "RV32I SIMULATION" << std::endl;
   Simulators::RV32I::Simulator rv32iSim;
   rv32iSim.run(rv32iExecutable);

   //
   std::string fileName = std::filesystem::path(inputPath).stem();
   std::cout << "Path name = " << inputPath << "; fileName = " << fileName << std::endl;
   // Generate MRSC binary object file.
   Simulators::RV32I::AssemblerUtils::generateAssemblyFileForMRCS(rv32iExecutable, fileName);

   // ------------ REBEL-6 ------------

   // Conversion
   Expressions::ExpressionList rebel6Expressions;
   Converters::Converter::Convert("rebel-6", binaryExpressions, rebel6Expressions);

   // Assembling
   Simulators::REBEL6::ExecutableProgram rebel6Executable;
   //Simulators::REBEL6::AssemblerAndLinker rebel6Assembler(rebel6Expressions, rebel6Executable);

   // Simulation
   std::cout << "REBEL-6 SIMULATION" << std::endl;
   //Simulators::REBEL6::Simulator rebel6Sim;
   //rebel6Sim.run(rebel6Sim);

   // Generating MRSC ternary file
   //Simulators::REBEL6::AssemblerUtils::generateAssemblyFileForMRCS(rebel6Executable, fileName);




   TernaryMapper mapper(binaryExpressions);
   Expressions::ExpressionList ternaryExpressions;
   //mapper.mapExpressions(ternaryExpressions);


   std::cout << "BINARY EXPRESSIONS: " << std::endl;
   //printExpressions(binaryExpressions);

   std::cout << "TERNARY EXPRESSIONS: " << std::endl;
   //printExpressions(ternaryExpressions);

   // Cleanup
   binaryExpressions.remove_if([](const Expressions::Expression* expr) { delete expr; return true;});
   ternaryExpressions.remove_if([](const Expressions::Expression* expr) { delete expr; return true;});
   delete l;
   return 0;
}
