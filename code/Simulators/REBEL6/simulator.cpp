#include "simulator.h"

namespace Simulators
{
namespace REBEL6
{
Simulator::Simulator()
{

}

void Simulator::run(Assemblers::REBEL6::ExecutableProgram& executable)
{
   m_cpu.executeProgram(executable);
}

std::uint32_t Simulator::getNumberOfRanInstructions()
{
   return m_cpu.getNumberOfRanInstructions();
}

std::uint32_t Simulator::getNumberOfTritFlips()
{
   return m_cpu.getTritFlipCost();
}

}
}
