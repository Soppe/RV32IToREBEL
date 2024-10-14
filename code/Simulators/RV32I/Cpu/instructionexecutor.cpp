#include "instructionexecutor.h"

#include <Assemblers/RV32I/executableprogram.h>
#include <logger.h>
#include <Parsers/parseutils.h>

#include <math.h>
#include <iostream>

namespace Simulators
{
namespace RV32I
{
namespace InstructionExecutor
{

//======================================
// Register instructions
//======================================
void executeAdd(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   rd = rs1 + rs2;
}

void executeSub(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   rd = rs1 - rs2;
}

void executeSll(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   rs2 = rs2 & 0x1f; // Use only the 5 lowest bits as defined by the standard

   if(rs2 == 0)
   {
      rd = rs1;
   }
   else
   {
      rd = rs1 << rs2;
   }
}

void executeSrl(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   rs2 = rs2 & 0x1f; // Use only the 5 lowest bits as defined by the standard

   if(rs2 == 0)
   {
      rd = rs1;
   }
   else
   {
      rd = rs1 >> rs2;

      // Since c++ 20 the right shift is defined to be arithmetic. Before c++20 it seems to have been implementation defined, but normally arithmetic for negative numbers
      // Since the >> operand therefore most likely sign extends the number, we have to simulate shifting 0's into the upper bits
      std::int32_t mask = 0x7fffffff;
      mask = mask >> (rs2 - 1);
      rd = rd & mask;
   }
}

void executeSra(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   rs2 = rs2 & 0x1f; // Use only the 5 lowest bits as defined by the standard

   if(rs2 == 0)
   {
      rd = rs1;
   }
   else
   {
      rd = rs1 >> rs2;
   }
}

void executeSlt(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   if(rs1 < rs2)
   {
      rd = 1;
   }
}

void executeSltu(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   std::uint32_t rs1u = rs1;
   std::uint32_t rs2u = rs2;

   if(rs1u < rs2u)
   {
      rd = 1;
   }
}

void executeOr(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   rd = rs1 | rs2;
}

void executeXor(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   rd = rs1 ^ rs2;
}

void executeAnd(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2)
{
   rd = rs1 & rs2;
}

//======================================
// Immediate instructions
//======================================
const std::int32_t maxImmediateValue = std::pow(2, 11);
void executeAddi(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   if((imm >= maxImmediateValue) || (imm < -maxImmediateValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   std::int32_t imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   rd = rs1 + imm12;
}

void executeSlli(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   // This is based on tests with the GCC assembler using values represented with more than 5 bits, and negative numbers, where errors related to negative numbers inferred they had been converted
   // to signed values first - a low negative number resulted in an error referring to close to the max value of a 64 bit integer
   std::uint32_t immu = imm;
   if(immu > 0x1f)
   {
      std::cerr << __PRETTY_FUNC__ << "Value too large: " << immu << std::endl;
      abort();
   }

   rd = rs1 << immu;
}

void executeSrli(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   // This is based on tests with the GCC assembler using values represented with more than 5 bits, and negative numbers, where errors related to negative numbers inferred they had been converted
   // to signed values first - a low negative number resulted in an error referring to close to the max value of a 64 bit integer
   std::uint32_t immu = imm;
   if(immu > 0x1f)
   {
      std::cerr << __PRETTY_FUNC__ << "Value too large: " << immu << std::endl;
      abort();
   }

   if(immu == 0)
   {
      rd = rs1;
   }
   else
   {
      rd = rs1 >> immu;

      // Since the >> operand in c++ functions as sra we have to simulate shifting 0's into the upper bits
      std::int32_t mask = 0x7fffffff;
      mask = mask >> (immu - 1);
      rd = rd & mask;
   }
}

void executeSrai(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   // This is based on tests with the GCC assembler using values represented with more than 5 bits, and negative numbers, where errors related to negative numbers inferred they had been converted
   // to signed values first - a low negative number resulted in an error referring to close to the max value of a 64 bit integer
   std::uint32_t immu = imm;
   if(immu > 0x1f)
   {
      std::cerr << __PRETTY_FUNC__ << "Value too large: " << immu << std::endl;
      abort();
   }

   rd = rs1 >> immu;
}

void executeSlti(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   if((imm >= maxImmediateValue) || (imm < -maxImmediateValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   std::int32_t imm12;
   ParseUtils::parseImmediate(12, imm, imm12);


   if(rs1 < imm12)
   {
      rd = 1;
   }
}

// The standard says the imm value is to first be sign-extended, then converted to an unsigned value. Ergo, the limits for sltiu are the same as for slti.
void executeSltiu(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   if((imm >= maxImmediateValue) || (imm < -maxImmediateValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   std::int32_t imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   std::uint32_t rs1u = rs1;
   std::uint32_t immu12 = imm12;

   if(rs1u < immu12)
   {
      rd = 1;
   }
}

void executeOri(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   if((imm >= maxImmediateValue) || (imm < -maxImmediateValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   std::int32_t imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   rd = rs1 | imm12;
}

void executeXori(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   if((imm >= maxImmediateValue) || (imm < -maxImmediateValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   std::int32_t imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   rd = rs1 ^ imm12;
}

void executeAndi(std::int32_t& rd, std::int32_t rs1, std::int32_t imm)
{
   if((imm >= maxImmediateValue) || (imm < -maxImmediateValue))
   {
      std::cerr << __PRETTY_FUNC__ << "Illegal value " << imm << std::endl;
      abort();
   }

   std::int32_t imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   rd = rs1 & imm12;
}

//======================================
// Upper instructions
//======================================
void executeLui(std::int32_t& rd, std::int32_t imm20)
{
   // GCC compiler will throw an error if any of the 12 MSB is set. We'll do the same.
   if((imm20 & 0xfff00000) != 0)
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << imm20 << std::endl;
      abort();
   }

   ParseUtils::parseImmediate(20, imm20, imm20);
   rd = imm20 << 12;
}

void executeAuipc(std::int32_t& rd, std::int32_t imm20, std::int32_t pc)
{
   // GCC compiler will throw an error if any of the 12 MSB is set. We'll do the same.
   if((imm20 & 0xfff00000) != 0)
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << imm20 << std::endl;
      abort();
   }

   ParseUtils::parseImmediate(20, imm20, imm20);
   rd = (imm20 << 12) + pc;
}

//======================================
// Branch instructions
//======================================
const std::int32_t maxBranchOffset = std::pow(2, 12);
void executeBeq(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc, std::uint8_t instructionSize)
{
   if((offset >= maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   if(rs1 == rs2)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

void executeBne(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc, std::uint8_t instructionSize)
{
   if((offset >= maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   if(rs1 != rs2)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

void executeBlt(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc, std::uint8_t instructionSize)
{
   if((offset >= maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   if(rs1 < rs2)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

void executeBltu(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc, std::uint8_t instructionSize)
{
   if((offset >= maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   std::uint32_t rs1u = rs1;
   std::uint32_t rs2u = rs2;

   if(rs1u < rs2u)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

void executeBge(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc, std::uint8_t instructionSize)
{
   if((offset >= maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   if(rs1 >= rs2)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

void executeBgeu(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc, std::uint8_t instructionSize)
{
   if((offset >= maxBranchOffset) || (offset < -maxBranchOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   std::uint32_t rs1u = rs1;
   std::uint32_t rs2u = rs2;
   if(rs1u >= rs2u)
   {
      pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
   }
}

//======================================
// Jump instructions
//======================================
const std::int32_t maxJumpOffset = std::pow(2, 20);
void executeJal(std::int32_t& rd, std::int32_t offset, std::uint32_t& pc, std::uint8_t instructionSize)
{
   if((offset >= maxJumpOffset) || (offset < -maxJumpOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   ParseUtils::parseImmediate(21, offset, offset);
   rd = pc + instructionSize;
   pc = pc + offset - instructionSize; // Subtract instruction size since simulator automatically adds instruction size to PC after each instruction call
}

//======================================
// Jump register instructions
//======================================
const std::int32_t maxJalrOffset = std::pow(2, 11);
void executeJalr(std::int32_t& rd, std::int32_t offset, std::int32_t rs1, std::uint32_t& pc, std::uint8_t instructionSize)
{
   if((offset >= maxJalrOffset) || (offset < -maxJalrOffset))
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
const std::int32_t maxLoadOffset = std::pow(2, 11);
void executeLw(std::int32_t& rd, std::int32_t offset, std::int32_t rs1, Assemblers::RV32I::ExecutableProgram& program)
{
   if((offset >= maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   rd = program.loadFromHeap(offset + rs1, 4);
   ParseUtils::parseImmediate(32, rd, rd);
}

void executeLh(std::int32_t& rd, std::int32_t offset, std::int32_t rs1, Assemblers::RV32I::ExecutableProgram& program)
{
   if((offset >= maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   rd = program.loadFromHeap(offset + rs1, 2);
   ParseUtils::parseImmediate(16, rd, rd);
}

void executeLb(std::int32_t& rd, std::int32_t offset, std::int32_t rs1, Assemblers::RV32I::ExecutableProgram& program)
{
   if((offset >= maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   rd = program.loadFromHeap(offset + rs1, 1);
   ParseUtils::parseImmediate(8, rd, rd);
}

void executeLhu(std::int32_t& rd, std::int32_t offset, std::int32_t rs1, Assemblers::RV32I::ExecutableProgram& program)
{
   if((offset >= maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   rd = program.loadFromHeap(offset + rs1, 2);
}

void executeLbu(std::int32_t& rd, std::int32_t offset, std::int32_t rs1, Assemblers::RV32I::ExecutableProgram& program)
{
   if((offset >= maxLoadOffset) || (offset < -maxLoadOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   rd = program.loadFromHeap(offset + rs1, 1);
}

//======================================
// Store instructions
//======================================
const std::int32_t maxStoreOffset = std::pow(2, 11);
void executeSw(std::int32_t rs2, std::int32_t offset, std::int32_t rs1, Assemblers::RV32I::ExecutableProgram& program)
{
   if((offset >= maxStoreOffset) || (offset < -maxStoreOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   program.storeToHeap(offset + rs1, rs2, 4);
}

void executeSh(std::int32_t rs2, std::int32_t offset, std::int32_t rs1, Assemblers::RV32I::ExecutableProgram& program)
{
   if((offset >= maxStoreOffset) || (offset < -maxStoreOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   program.storeToHeap(offset + rs1, rs2, 2);
}

void executeSb(std::int32_t rs2, std::int32_t offset, std::int32_t rs1, Assemblers::RV32I::ExecutableProgram& program)
{
   if((offset >= maxStoreOffset) || (offset < -maxStoreOffset))
   {
      std::cerr << __PRETTY_FUNC__ << ": Illegal value " << offset << std::endl;
      abort();
   }

   program.storeToHeap(offset + rs1, rs2, 1);
}

}
}
}








