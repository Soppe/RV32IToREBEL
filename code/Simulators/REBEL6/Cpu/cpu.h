#pragma once

#include "registry.h"

namespace Simulators
{
namespace REBEL6
{

class ExecutableProgram;

class CPU
{
 public:
   CPU();
   ~CPU();

   void executeProgram(ExecutableProgram& program);
   std::uint32_t getNumberOfRanInstructions() const;
   std::uint32_t getTritFlipCost() const;

 private:
   void initRegisters(std::int32_t programSizeTrytes);

   void executeRegister(const std::string& name, bool isBinary, const std::string& rd, const std::string& rs1, const std::string& rs2);
   void executeImmediate(const std::string& name, bool isBinary, const std::string& rd, const std::string& rs1, const std::string& imm);
   void executeLoadImmediate(const std::string& name, const std::string& rd, const std::string& imm);
   void executeBranch(const std::string& name, bool isBinary, std::uint8_t instructionSize, const std::string& rs1, const std::string& rs2, const std::string& offset);
   void executeJump(const std::string& name, std::uint8_t instructionSize, const std::string& rd, const std::string& offset);
   void executeJumpRegister(const std::string& name, std::uint8_t instructionSize, const std::string& rd, const std::string& target);
   void executeLoad(const std::string& name, bool isBinary, const std::string& rd, const std::string& address, ExecutableProgram& program);
   void executeStore(const std::string& name, bool isBinary, const std::string& rs, const std::string& address, ExecutableProgram& program);
   void executeSystem(const std::string& name);

   void resolveRsOffset(const std::string& offset, Tint& offseti, Tint& rs1);

   Registry m_registers;
   std::int32_t m_PC;
   std::uint32_t m_numberOfRanInstructions;
};

}
}
