#ifndef SIMULATORUTILS_H
#define SIMULATORUTILS_H

#include <string>


namespace Simulators
{
namespace RV32I
{
class ExecutableProgram;

class SimulatorUtils
{
public:
   enum class InstructionType {BRANCH, IMMEDIATE, JUMP, JUMP_REGISTER, LOAD, REGISTER, STORE, UPPER, SYSTEM, UNDEFINED};

   static InstructionType getInstructionType(const std::string& instructionName);
   static void generateAssemblyFileForMRCS(const ExecutableProgram& program, const std::string& fileName);

};

}
}

#endif // SIMULATORUTILS_H
