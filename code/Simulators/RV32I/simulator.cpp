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

std::uint32_t Simulator::getNumberOfRanInstructions()
{
   return m_cpu.getNumberOfRanInstructions();
}

std::uint32_t Simulator::getNumberOfBitFlips()
{
   return m_cpu.getBitFlipCost();
}

}
}
