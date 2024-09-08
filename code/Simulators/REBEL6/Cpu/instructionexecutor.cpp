#include "instructionexecutor.h"

#include <Simulators/RV32I/Cpu/instructionexecutor.h>
#include <logger.h>

#include <math.h>
#include <iostream>

namespace Simulators
{
namespace REBEL6
{
namespace InstructionExecutor
{

//======================================
// Register instructions
//======================================

// Binary
void executeAdd(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   // Converting from Tint to int32 means we implicitly handle any possible overflow
   std::int32_t rdi = 0;
   std::int32_t rs1i = rs1;
   std::int32_t rs2i = rs2;
   Simulators::RV32I::InstructionExecutor::executeAdd(rdi, rs1i, rs2i);

   rd = rdi;
}

void executeSub(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   // Converting from Tint to int32 means we implicitly handle any possible overflow
   std::int32_t rdi = 0;
   std::int32_t rs1i = rs1;
   std::int32_t rs2i = rs2;
   Simulators::RV32I::InstructionExecutor::executeSub(rdi, rs1i, rs2i);

   rd = rdi;
}

void executeSll(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeSrl(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeSra(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeOr(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeXor(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeAnd(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   // Converting from Tint to int32 means we implicitly handle any possible overflow
   std::int32_t rdi = 0;
   std::int32_t rs1i = rs1;
   std::int32_t rs2i = rs2;

   Simulators::RV32I::InstructionExecutor::executeAnd(rdi, rs1i, rs2i);

   rd = rdi;
}

// Ternary
void executeAdd_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
}

void executeSub_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeSl_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeSr_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeSlt_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeOr_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeXor_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

void executeAnd_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{

}

//======================================
// Immediate instructions
//======================================

// Binary
void executeAddi(Tint& rd, const Tint& rs1, const Tint& imm)
{
   // Converting from Tint to int32 means we implicitly handle any possible overflow
   std::int32_t rdi = 0;
   std::int32_t rs1i = rs1;
   std::int32_t immi = imm;
   Simulators::RV32I::InstructionExecutor::executeAddi(rdi, rs1i, immi);

   rd = rdi;
}

void executeSlli(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeSrli(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeSrai(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeOri(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeXori(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeAndi(Tint& rd, const Tint& rs1, const Tint& imm)
{
   // Converting from Tint to int32 means we implicitly handle any possible overflow
   std::int32_t rdi = 0;
   std::int32_t rs1i = rs1;
   std::int32_t immi = imm;
   Simulators::RV32I::InstructionExecutor::executeAndi(rdi, rs1i, immi);

   rd = rdi;
}

// Ternary
const Tint immediateImmMaxValue = std::pow(3, 11); //TODO: Find out the size of the immediate
void executeAddi_t(Tint& rd, const Tint& rs1, const Tint& imm)
{
   if((imm > immediateImmMaxValue) || (imm < -immediateImmMaxValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   Tint imm12;
   TernaryLogic::ParseImmediate(12, imm, imm12);

   rd = rs1 + imm12;
}

void executeSli_t(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeSri_t(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeSlti_t(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeOri_t(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeXori_t(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

void executeAndi_t(Tint& rd, const Tint& rs1, const Tint& imm)
{

}

//======================================
// Load Immediate instructions
//======================================

// Ternary
void executeLi_t(Tint& rd, const Tint& imm)
{
   Tint immi = 0;
   TernaryLogic::ParseImmediate(21, imm, immi);
   rd = immi;
}

void executeAipc_t(Tint& rd, const Tint& imm, const std::int32_t& pc)
{
   Tint immi = 0;
   TernaryLogic::ParseImmediate(21, imm, immi);
   rd = immi + pc;
}

//======================================
// Branch instructions
//======================================

// Ternary
const Tint maxBranchOffset = std::pow(3, 15); // TODO: Find max branch range (remember that the exponent is the support number of trits -1, since pow don't consider 3^0)
void executeBeq_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc)
{

}

void executeBne_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc)
{
   if((offset > maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }


   if(rs1 != rs2)
   {
      pc = pc + offset - 32; // Subtract 32 since simulator automatically adds 32 to PC after each instruction call
   }
}

void executeBlt_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc)
{

}

void executeBge_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc)
{

}

//======================================
// Jump instructions
//======================================

// Ternary
const Tint maxJumpOffset = std::pow(3, 15); // TODO: Find max branch range (remember that the exponent is the support number of trits -1, since pow don't consider 3^0)
void executeJal_t(Tint& rd, std::int32_t offset, std::int32_t& pc)
{
   if((offset > maxJumpOffset) || (offset < -maxJumpOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Tint offseti = 0;
   TernaryLogic::ParseImmediate(16, offset, offseti);// TODO: Check offset number of trits
   rd = pc + 32;
   pc = pc + offseti - 32; // Subtract 32 since simulator automatically adds 32 to PC after each instruction call
}

//======================================
// Jump register instructions
//======================================

// Ternary
void executeJalr_t(Tint& rd, std::int32_t target, std::int32_t& pc)
{

}

//======================================
// Load instructions
//======================================

// Binary
void executeLw(Tint& rd, std::int32_t srcAddress, ExecutableProgram& program)
{

}

void executeLh(Tint& rd, std::int32_t srcAddress, ExecutableProgram& program)
{

}

void executeLb(Tint& rd, std::int32_t srcAddress, ExecutableProgram& program)
{

}

void executeLhu(Tint& rd, std::int32_t srcAddress, ExecutableProgram& program)
{

}

void executeLbu(Tint& rd, std::int32_t srcAddress, ExecutableProgram& program)
{

}

// Ternary
void executeLw_t(Tint& rd, std::int32_t srcAddress, ExecutableProgram& program)
{

}

void executeLh_t(Tint& rd, std::int32_t srcAddress, ExecutableProgram& program)
{

}

void executeLb_t(Tint& rd, std::int32_t srcAddress, ExecutableProgram& program)
{

}

//======================================
// Store instructions
//======================================

// Binary
void executeSw(const Tint& rs, std::int32_t targetAddress, ExecutableProgram& program)
{

}

void executeSh(const Tint& rs, std::int32_t targetAddress, ExecutableProgram& program)
{

}

void executeSb(const Tint& rs, std::int32_t targetAddress, ExecutableProgram& program)
{

}

// Ternary
void executeSw_t(const Tint& rs, std::int32_t targetAddress, ExecutableProgram& program)
{

}

void executeSh_t(const Tint& rs, std::int32_t targetAddress, ExecutableProgram& program)
{

}

void executeSb_t(const Tint& rs, std::int32_t targetAddress, ExecutableProgram& program)
{

}

}
}
}
