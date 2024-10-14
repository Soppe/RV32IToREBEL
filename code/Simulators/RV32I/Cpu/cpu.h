#pragma once

#include "registry.h"

namespace Assemblers
{
namespace RV32I
{
class ExecutableProgram;
}
}

namespace Simulators
{
namespace RV32I
{

class CPU
{
public:
   CPU();
   ~CPU();

   void executeProgram(Assemblers::RV32I::ExecutableProgram& program);
   std::uint32_t getNumberOfRanInstructions() const;
   std::uint32_t getBitFlipCost() const;

private:
   void initRegisters(std::int32_t programSizeBytes);

   void executeRegister(const std::string& name, const std::string& rd, const std::string& rs1, const std::string& rs2);
   void executeImmediate(const std::string& name, const std::string& rd, const std::string& rs1, const std::string& imm);
   void executeUpper(const std::string& name, const std::string& rd, const std::string& imm);
   void executeBranch(const std::string& name, std::uint8_t instructionSize, const std::string& rs1, const std::string& rs2, const std::string& offset);
   void executeJump(const std::string& name, std::uint8_t instructionSize, const std::string& rd, const std::string& offset);
   void executeJumpRegister(const std::string& name, std::uint8_t instructionSize, const std::string& rd, const std::string& target);
   void executeLoad(const std::string& name, const std::string& rd, const std::string& address, Assemblers::RV32I::ExecutableProgram& program);
   void executeStore(const std::string& name, const std::string& rs, const std::string& address, Assemblers::RV32I::ExecutableProgram& program);
   void executeSystem(const std::string& name);

   void resolveRsOffset(const std::string& offset, std::int32_t& offseti, std::int32_t& rs1);

   Registry m_registers;
   std::uint32_t m_PC;
   std::uint32_t m_numberOfRanInstructions;
};

}
}
