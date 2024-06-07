#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <list>
#include "Cpus/cpu.h"


namespace Converters
{
class RV32IPseudoToRV32IBase;
}

class Expression;
class ExecutableProgram;

namespace Simulators
{
namespace RV32I
{

class Simulator
{
public:
   Simulator();
   ~Simulator();

   void init(std::list<Expression*>& expressions);
   void run();

private:
   ExecutableProgram* m_executable;
   CPU* m_cpu;
};

}
}
#endif // SIMULATOR_H
