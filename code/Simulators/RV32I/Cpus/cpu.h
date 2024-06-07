#ifndef CPU_H
#define CPU_H

#include "registry.h"
#include <list>


class Expression;

namespace Simulators
{
namespace RV32I
{

class ExecutableProgram;

class CPU
{
public:
   CPU();
   ~CPU();

   void executeProgram(ExecutableProgram& program);
   int getAccumulatedCost() const;

protected:
   void initRegisters(int programSize);

   void executeRegister(const std::string& name, const std::string& rd, const std::string& rs1, const std::string& rs2);
   void executeImmediate(const std::string& name, const std::string& rd, const std::string& rs1, const std::string& imm);
   void executeUpper(const std::string& name, const std::string& rd, const std::string& imm);
   void executeBranch(const std::string& name, const std::string& rs1, const std::string& rs2, const std::string& offset);
   void executeJump(const std::string& name, const std::string& rd, const std::string& offset);
   void executeJumpRegister(const std::string& name, const std::string& rd, const std::string& target);
   void executeLoad(const std::string& name, const std::string& rd, const std::string& address, ExecutableProgram& program);
   void executeStore(const std::string& name, const std::string& rs, const std::string& address, ExecutableProgram& program);
   void executeSystem(const std::string& name);

   void resolve12ImmOffset(const std::string& offset, int& value);

   Registry m_registers;
   int m_PC;
   int m_accumulatedCost;
};

}
}

#endif // CPU_H
