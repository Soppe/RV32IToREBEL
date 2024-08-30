#pragma once

#include "Cpu/cpu.h"

#include <Expressions/expression.h>


namespace Simulators
{
namespace RV32I
{

class ExecutableProgram;

class Simulator
{
public:
   Simulator();

   void run(ExecutableProgram& executable);

private:
   CPU m_cpu;
};

}
}
