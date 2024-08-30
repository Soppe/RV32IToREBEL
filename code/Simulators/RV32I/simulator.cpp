#include "simulator.h"

namespace Simulators
{
namespace RV32I
{

Simulator::Simulator()
{

}

void Simulator::run(ExecutableProgram& executable)
{
   m_cpu.executeProgram(executable);
}

}
}
