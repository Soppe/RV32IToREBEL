#include "instructionexecutor.h"

#include <Simulators/REBEL6/executableprogram.h>
#include <Simulators/RV32I/Cpu/instructionexecutor.h>
#include <logger.h>

#include <math.h>
#include <iostream>

namespace
{
std::int32_t TrytesToInt(const Trytes& trytes)
{
   std::int32_t retVal = 0;

   for(std::uint8_t i = 0; i < trytes.size(); ++i)
   {
      retVal = retVal | ((trytes[i] & 0xFF) << (8 * i));
   }

   return retVal;
}

}


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
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeAdd(rdi, rs1, rs2);

   rd = rdi;
}

void executeSub(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSub(rdi, rs1, rs2);

   rd = rdi;
}

void executeSll(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSll(rdi, rs1, rs2);

   rd = rdi;
}

void executeSrl(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSrl(rdi, rs1, rs2);

   rd = rdi;
}

void executeSra(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSra(rdi, rs1, rs2);

   rd = rdi;
}

void executeSltu(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSltu(rdi, rs1, rs2);

   rd = rdi;
}

void executeOr(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeOr(rdi, rs1, rs2);

   rd = rdi;
}

void executeXor(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeXor(rdi, rs1, rs2);

   rd = rdi;
}

void executeAnd(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeAnd(rdi, rs1, rs2);

   rd = rdi;
}

// Ternary
void executeAdd_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeSub_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeSl_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeSr_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeSlt_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   if(rs1 < rs2)
   {
      rd = 1;
   }
}

void executeOr_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeXor_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeAnd_t(Tint& rd, const Tint& rs1, const Tint& rs2)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

//======================================
// Immediate instructions
//======================================

// Binary
void executeAddi(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeAddi(rdi, rs1, imm);

   rd = rdi;
}

void executeSlli(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSlli(rdi, rs1, imm);

   rd = rdi;
}

void executeSrli(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSrli(rdi, rs1, imm);

   rd = rdi;
}

void executeSrai(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSrai(rdi, rs1, imm);

   rd = rdi;
}

void executeSltiu(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeSltiu(rdi, rs1, imm);

   rd = rdi;
}

void executeOri(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeOri(rdi, rs1, imm);

   rd = rdi;
}

void executeXori(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeXori(rdi, rs1, imm);

   rd = rdi;
}

void executeAndi(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::int32_t rdi = 0;
   Simulators::RV32I::InstructionExecutor::executeAndi(rdi, rs1, imm);

   rd = rdi;
}

// Ternary
const Tint maxImmediateValue = std::pow(3, 12) / 2; // TODO: Find out the size of the immediate
void executeAddi_t(Tint& rd, const Tint& rs1, const Tint& imm)
{
   if((imm > maxImmediateValue) || (imm < -maxImmediateValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   Tint imm12;
   TernaryLogic::parseImmediate(12, imm, imm12);

   rd = rs1 + imm12;
}

void executeSli_t(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeSri_t(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeSlti_t(Tint& rd, const Tint& rs1, const Tint& imm)
{
   if((imm > maxImmediateValue) || (imm < -maxImmediateValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   Tint imm12;
   TernaryLogic::parseImmediate(12, imm, imm12);


   if(rs1 < imm12)
   {
      rd = 1;
   }
}

void executeOri_t(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeXori_t(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeAndi_t(Tint& rd, const Tint& rs1, const Tint& imm)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

//======================================
// Load Immediate instructions
//======================================

// Ternary
void executeLi_t(Tint& rd, const Tint& imm)
{
   Tint immi = 0;
   TernaryLogic::parseImmediate(21, imm, immi);
   rd = immi;
}

void executeAipc_t(Tint& rd, const Tint& imm, const std::int32_t& pc)
{
   Tint immi = 0;
   TernaryLogic::parseImmediate(21, imm, immi);
   rd = immi + pc;
}

//======================================
// Branch instructions
//======================================

// Binary
void executeBgeu(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize)
{
   std::uint32_t pcui = pc;
   Simulators::RV32I::InstructionExecutor::executeBgeu(rs1, rs2, offset, pcui, instructionSize);

   pc = pcui;
}

void executeBltu(const Tint& rs1, const Tint& rs2, int32_t offset, int32_t& pc, uint8_t instructionSize)
{
   std::uint32_t pcui = pc;
   Simulators::RV32I::InstructionExecutor::executeBltu(rs1, rs2, offset, pcui, instructionSize);

   pc = pcui;
}

// Ternary
const Tint maxBranchOffset = std::pow(3, 16) / 2; // TODO: Find max branch range (remember that the exponent is the support number of trits -1, since pow don't consider 3^0)
void executeBeq_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize)
{
   if((offset > maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }


   if(rs1 == rs2)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

void executeBne_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize)
{
   if((offset > maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }


   if(rs1 != rs2)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

void executeBlt_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize)
{
   if((offset > maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }


   if(rs1 < rs2)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

void executeBge_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize)
{
   if((offset > maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }


   if(rs1 >= rs2)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

//======================================
// Jump instructions
//======================================

// Ternary
const Tint maxJumpOffset = std::pow(3, 16) / 2; // TODO: Find max branch range (remember that the exponent is the support number of trits -1, since pow don't consider 3^0)
void executeJal_t(Tint& rd, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize)
{
   if((offset > maxJumpOffset) || (offset < -maxJumpOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Tint offseti = 0;
   TernaryLogic::parseImmediate(16, offset, offseti);// TODO: Check offset number of trits
   rd = pc + instructionSize;
   pc = pc + offseti - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
}

//======================================
// Jump register instructions
//======================================

// Ternary
const Tint maxJalrOffset = std::pow(3, 16) / 2; // TODO: Find actual amount of trits
void executeJalr_t(Tint& rd, const Tint& offset, const Tint& rs1, std::int32_t& pc, std::uint8_t instructionSize)
{
   if((offset > maxJalrOffset) || (offset < -maxJalrOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   rd = pc + instructionSize;
   pc = offset + rs1 - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
}

//======================================
// Load instructions
//======================================

// Binary
const Tint maxLoadOffset = std::pow(3, 11); // TODO: Find the correct offset
void executeLw(Tint& rd, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   if((offset > maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Trytes trytes;
   program.loadFromHeap(offset + rs1, 4, trytes);
   std::int32_t rdi= TrytesToInt(trytes);
   rd = rdi;
}

void executeLh(Tint& rd, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   if((offset > maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Trytes trytes;
   program.loadFromHeap(offset + rs1, 2, trytes);
   std::int16_t rdi= TrytesToInt(trytes);
   rd = rdi;
}

void executeLb(Tint& rd, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   if((offset > maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Trytes trytes;
   program.loadFromHeap(offset + rs1, 1, trytes);
   std::int8_t rdi = TrytesToInt(trytes);
   rd = rdi;
}

void executeLhu(Tint& rd, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   if((offset > maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Trytes trytes;
   program.loadFromHeap(offset + rs1, 2, trytes);
   std::uint16_t rdi = TrytesToInt(trytes);
   rd = rdi;
}

void executeLbu(Tint& rd, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   if((offset > maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Trytes trytes;
   program.loadFromHeap(offset + rs1, 1, trytes);
   std::uint8_t rdi = TrytesToInt(trytes);
   rd = rdi;
}

// Ternary
void executeLw_t(Tint& rd, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeLh_t(Tint& rd, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeLt_t(Tint& rd, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

//======================================
// Store instructions
//======================================
const Tint maxStoreOffset = std::pow(3, 12); // TODO: Find the correct offset
// Binary
void executeSw(const Tint& rs2, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   if((offset > maxStoreOffset) || (offset < -maxStoreOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   // Who needs for-loops anyways...
   Tryte rs2t = rs2 & 0xFF;
   program.storeToHeap(offset + rs1, rs2t, 1);
   rs2t = (rs2 >> 8) & 0xFF;
   program.storeToHeap(offset + rs1 + 1, rs2t, 1);
   rs2t = (rs2 >> 16) & 0xFF;
   program.storeToHeap(offset + rs1 + 2, rs2t, 1);
   rs2t = (rs2 >> 24) & 0xFF;
   program.storeToHeap(offset + rs1 + 3, rs2t, 1);
}

void executeSh(const Tint& rs2, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   if((offset > maxStoreOffset) || (offset < -maxStoreOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Tryte rs2t = rs2 & 0xFF;
   program.storeToHeap(offset + rs1, rs2t, 1);
   rs2t = (rs2 >> 8) & 0xFF;
   program.storeToHeap(offset + rs1 + 1, rs2t, 1);
}

void executeSb(const Tint& rs2, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   if((offset > maxStoreOffset) || (offset < -maxStoreOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   Tryte rs2t = rs2 & 0xFF;
   program.storeToHeap(offset + rs1, rs2t, 1);
}

// Ternary
void executeSw_t(const Tint& rs2, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeSh_t(const Tint& rs2, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

void executeSb_t(const Tint& rs2, const Tint& offset, const Tint& rs1, ExecutableProgram& program)
{
   std::cerr << __PRETTY_FUNC__ << ": Unimplemented execution." << std::endl;
   abort();
}

}
}
}
