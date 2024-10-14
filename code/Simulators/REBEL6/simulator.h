#pragma once

#include "Cpu/cpu.h"

#include <Expressions/expression.h>

namespace Simulators
{
namespace REBEL6
{

class Simulator
{
 public:
   Simulator();

   void run(Assemblers::REBEL6::ExecutableProgram& executable);

   std::uint32_t getNumberOfRanInstructions();
   std::uint32_t getNumberOfTritFlips();

 private:
   CPU m_cpu;
};

}
}
