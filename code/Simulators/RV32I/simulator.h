#pragma once

#include <Expressions/expression.h>


namespace Simulators
{
namespace RV32I
{

class ExecutableProgram;
class CPU;

class Simulator
{
public:
   Simulator();
   ~Simulator();

   void run(ExecutableProgram& executable);

private:
   CPU* m_cpu;
};

}
}
