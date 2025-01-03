#pragma once

#include "Cpu/cpu.h"

#include <Expressions/expression.h>

namespace Simulators
{
namespace RV32I
{

class Simulator
{
public:
   Simulator();

   void run(Assemblers::RV32I::ExecutableProgram& executable);

   std::uint32_t getNumberOfRanInstructions();
   std::uint32_t getNumberOfBitFlips();

private:
   CPU m_cpu;
};

}
}
