#include "simulator.h"
#include "assemblerandlinker.h"
#include "executableprogram.h"

#include <Converters/rv32ipseudotorv32ibase.h>
#include <Expressions/all_expressions.h>
#include <Parsers/expressionparser.h>

#include <iostream>
#include <iterator>

namespace Simulators
{
namespace RV32I
{

Simulator::Simulator()
    : m_executable(new ExecutableProgram())
    , m_cpu(new CPU())
{

}

Simulator::~Simulator()
{
   delete m_cpu;
   delete m_executable;
}

void Simulator::init(std::list<Expression*>& expressions)
{
   Converters::RV32IPseudoToRV32IBase converter;
   AssemblerAndLinker assLnk(expressions, converter, m_executable);

   assLnk.init();
   assLnk.run();
}

void Simulator::run()
{
   m_cpu->executeProgram(*m_executable);


   /*std::cout << "BINARY INSTRUCTIONS:" << std::endl;
   m_programMemory->printInstructions();

   std::cout << std::endl;

   std::cout << "BINARY SYMBOLS" << std::endl;
   m_programMemory->printSymbols();*/
}

}
}
