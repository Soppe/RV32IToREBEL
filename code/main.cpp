#include "Converters/converter.h"

#include "Parsers/lexer.h"
#include "Parsers/token.h"
#include "Parsers/parser.h"

#include "Expressions/expression.h"
#include "Expressions/all_expressions.h"

#include "Assemblers/RV32I/assemblerandlinker.h"
#include "Assemblers/RV32I/assemblerutils.h"
#include "Assemblers/RV32I/executableprogram.h"
#include "Simulators/RV32I/simulator.h"

#include "Assemblers/REBEL6/assemblerandlinker.h"
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
   Assemblers::RV32I::ExecutableProgram rv32iExecutable;
   Assemblers::RV32I::AssemblerAndLinker rv32iAssembler(rv32iExpressions, rv32iExecutable);

   // Generate binary assembly file
   std::string fileName = std::filesystem::path(inputPath).stem();
   rv32iAssembler.printExpressionsToFile(fileName);

   rv32iAssembler.init();
   rv32iAssembler.run();

   // Simulation
   std::cout << "RV32I SIMULATION" << std::endl;
   Simulators::RV32I::Simulator rv32iSim;
   rv32iSim.run(rv32iExecutable);

   // Generate MRSC binary object file.
   Assemblers::RV32I::AssemblerUtils::generateAssemblyFileForMRCS(rv32iExecutable, fileName);

   // ------------ REBEL-6 ------------

   // Conversion
   Expressions::ExpressionList rebel6Expressions;
   Converters::Converter::Convert("rebel-6", binaryExpressions, rebel6Expressions);

   // Assembling
   Assemblers::REBEL6::ExecutableProgram rebel6Executable;
   Assemblers::REBEL6::AssemblerAndLinker rebel6Assembler(rebel6Expressions, rebel6Executable);

   // Generate ternary assembly file
   rebel6Assembler.printExpressionsToFile(fileName);

   rebel6Assembler.init();
   rebel6Assembler.run();

   // Simulation
   std::cout << "REBEL-6 SIMULATION" << std::endl;
   Simulators::REBEL6::Simulator rebel6Sim;
   rebel6Sim.run(rebel6Executable);

   std::cout << "Binary: Instructions ran count = " << rv32iSim.getNumberOfRanInstructions() << "; bitflip count = " << rv32iSim.getNumberOfBitFlips() << std::endl;
   std::cout << "Ternary: Instructions ran count = " << rebel6Sim.getNumberOfRanInstructions() << "; tritflip count = " << rebel6Sim.getNumberOfTritFlips() << std::endl;

   // Generating MRSC ternary file
   //Simulators::REBEL6::AssemblerUtils::generateAssemblyFileForMRCS(rebel6Executable, fileName);

   // Cleanup
   binaryExpressions.remove_if([](const Expressions::Expression* expr) { delete expr; return true;});
   delete l;
   return 0;
}
