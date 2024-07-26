#include "simulator.h"
#include "assemblerandlinker.h"
#include "executableprogram.h"
#include "Cpu/cpu.h"

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
   : m_cpu(new CPU())
{

}

Simulator::~Simulator()
{
   delete m_cpu;
}

void Simulator::run(ExecutableProgram& executable)
{
   m_cpu->executeProgram(executable);

   /*std::cout << "BINARY INSTRUCTIONS:" << std::endl;
   executable->printInstructions();

   std::cout << std::endl;

   std::cout << "BINARY SYMBOLS" << std::endl;
   executable->printSymbols();*/
}

}
}
