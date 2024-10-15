#include "Translators/translator.h"

#include "Parsers/lexer.h"
#include "Parsers/token.h"
#include "Parsers/parser.h"

#include "Expressions/expression.h"
#include "Expressions/all_expressions.h"

#include "Assemblers/RV32I/assembler.h"
#include "Assemblers/RV32I/assemblerutils.h"
#include "Assemblers/RV32I/executableprogram.h"
#include "Simulators/RV32I/simulator.h"

#include "Assemblers/REBEL6/assembler.h"
#include "Assemblers/REBEL6/executableprogram.h"
#include "Simulators/REBEL6/simulator.h"

#include <iostream>
#include <fstream>
#include <filesystem>


int main(int argc, char* argv[])
{
   if(argc != 2)
   {
      std::cout << "Needs one file path." << std::endl;
      return -1;
   }

   std::string inputPath = argv[1];

   std::ifstream inFile(inputPath);
   if(!inFile)
   {
      std::cout << "Failed to open file " << inputPath << std::endl;
      abort();
   }
   std::string source((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
   source += '\n';

   /*
    * ------------------ FRONTEND ------------------
    */

   Lexer* l = new Lexer(source);
   Parser p(l);
   Expressions::ExpressionList sourceExpressions;
   p.parse(sourceExpressions);
   std::cout << "Parsed successfully." << std::endl;

   /*
    * --------- INTERMEDIATE REPRESENTAITON ---------
    */
   // NA - The RV32I input is currently considered the IR code.


   /*
    * ------------------- BACKEND -------------------
    */

   // ------------ RV32I------------

   // Translation
   Expressions::ExpressionList rv32iExpressions;
   Translators::Translator::Translate("rv32i", sourceExpressions, rv32iExpressions);

   // Assembling
   Assemblers::RV32I::ExecutableProgram rv32iExecutable;
   Assemblers::RV32I::Assembler rv32iAssembler(rv32iExpressions, rv32iExecutable);

   // Generate binary assembly file
   std::string fileName = std::filesystem::path(inputPath).stem();
   rv32iAssembler.printExpressionsToFile(fileName);

   // Run assembler
   rv32iAssembler.init();
   rv32iAssembler.run();

   // Generate MRSC binary object file.
   Assemblers::RV32I::AssemblerUtils::generateAssemblyFileForMRCS(rv32iExecutable, fileName);

   // ------------ REBEL-6 ------------

   // Translation
   Expressions::ExpressionList rebel6Expressions;
   Translators::Translator::Translate("rebel-6", sourceExpressions, rebel6Expressions);

   // Assembling
   Assemblers::REBEL6::ExecutableProgram rebel6Executable;
   Assemblers::REBEL6::Assembler rebel6Assembler(rebel6Expressions, rebel6Executable);

   // Generate ternary assembly file
   rebel6Assembler.printExpressionsToFile(fileName);

   // Run assembler
   rebel6Assembler.init();
   rebel6Assembler.run();

   // Generate MRCS ternary file
   //Simulators::REBEL6::AssemblerUtils::generateAssemblyFileForMRCS(rebel6Executable, fileName);

   /*
    * ------------------ SIMULATION ------------------
    */

   // ------------ RV32I------------

   // Simulation
   std::cout << "RV32I SIMULATION" << std::endl;
   Simulators::RV32I::Simulator rv32iSim;
   rv32iSim.run(rv32iExecutable);

   // ------------ REBEL-6 ------------
   // Simulation
   std::cout << "REBEL-6 SIMULATION" << std::endl;
   Simulators::REBEL6::Simulator rebel6Sim;
   rebel6Sim.run(rebel6Executable);

   /*
    * ------------------ BENCHMARKS ------------------
    */
   std::cout << "Binary: Instructions ran count = " << rv32iSim.getNumberOfRanInstructions() << "; bitflip count = " << rv32iSim.getNumberOfBitFlips() << std::endl;
   std::cout << "Ternary: Instructions ran count = " << rebel6Sim.getNumberOfRanInstructions() << "; tritflip count = " << rebel6Sim.getNumberOfTritFlips() << std::endl;

   // Cleanup
   sourceExpressions.remove_if([](const Expressions::Expression* expr) { delete expr; return true;});
   delete l;
   return 0;
}
