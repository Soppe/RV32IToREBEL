#include "simulator.h"

namespace Simulators
{
namespace REBEL6
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
