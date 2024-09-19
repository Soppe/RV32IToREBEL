#include "cpu.h"

#include "instructionexecutor.h"

#include <Expressions/instruction.h>
#include <Parsers/parseutils.h>
#include <Simulators/RV32I/simulatorutils.h>
#include <Simulators/RV32I/executableprogram.h>
#include <logger.h>

#include <iostream>
#include <limits.h>

namespace
{

// Set ra to some irrational and easily detectable value. GCC, and possibly other compilers, adds a "jr ra" at the end of the main routine to return to some caller.
// This value is used to detect when that happens.
const std::int32_t ProgramEndRAValue = INT_MIN; // Set it to a value that most likely triggers the least amount of bit flips when the calue is changed
const std::int16_t RegisterCount = 32;

}

namespace Simulators
{
namespace RV32I
{

CPU::CPU()
    : m_registers(RegisterCount)
    , m_PC(0)
    , m_numberOfRanInstructions(0)
{
}

CPU::~CPU()
{

}

void CPU::executeProgram(ExecutableProgram& program)
{
   try
   {
      m_PC = program.loadSymbolAddress("main");
   }
   catch(const std::exception& e)
   {
      m_PC = 0;
      std::cout << __PRETTY_FUNC__ << "Found no maing function. Program will start from address 0" << std::endl;
   }

   m_numberOfRanInstructions = 0;

   initRegisters(program.getProgramSizeBytes());

   std::uint8_t instructionSize = 0;
   const Expressions::Instruction* instr = program.loadInstruction(m_PC, instructionSize);

   SimulatorUtils::InstructionType type = SimulatorUtils::InstructionType::UNDEFINED;

   while(instr != nullptr)
   {
      const std::string& name = instr->getInstructionName();
      const std::vector<std::string>& operands = instr->getInstructionOperands();
      type = SimulatorUtils::getInstructionType(name);
      // std::cout << "pc = " << m_PC << "\t" << *instr << std::endl;

      switch(type)
      {
      case SimulatorUtils::InstructionType::REGISTER:
         executeRegister(name, operands[0], operands[1], operands[2]);
         break;
      case SimulatorUtils::InstructionType::IMMEDIATE:
         executeImmediate(name, operands[0], operands[1], operands[2]);
         break;
      case SimulatorUtils::InstructionType::UPPER:
         executeUpper(name, operands[0], operands[1]);
         break;
      case SimulatorUtils::InstructionType::BRANCH:
         executeBranch(name, instructionSize, operands[0], operands[1], operands[2]);
         break;
      case SimulatorUtils::InstructionType::JUMP:
         executeJump(name, instructionSize, operands[0], operands[1]);
         break;
      case SimulatorUtils::InstructionType::JUMP_REGISTER:
         executeJumpRegister(name, instructionSize, operands[0], operands[1]);
         break;
      case SimulatorUtils::InstructionType::LOAD:
         executeLoad(name, operands[0], operands[1], program);
         break;
      case SimulatorUtils::InstructionType::STORE:
         executeStore(name, operands[0], operands[1], program);
         break;
      case SimulatorUtils::InstructionType::SYSTEM:
         executeSystem(name);
         break;
      default:
         std::cerr << __PRETTY_FUNC__ << ": Undefined instruction " << name << std::endl;
         break;
      }

      ++m_numberOfRanInstructions;
      m_PC += instructionSize;

      if(m_PC == ProgramEndRAValue)
      {
         break;
      }

      instr = program.loadInstruction(m_PC, instructionSize);
   }

   std::cout << "Finished executing program" << std::endl;
}

uint32_t CPU::getNumberOfRanInstructions() const
{
   return m_numberOfRanInstructions;
}

uint32_t CPU::getBitFlipCost() const
{
   return m_registers.getAccumulatedBitFlips();
}

void CPU::initRegisters(std::int32_t programSizeBytes)
{
   m_registers.reset(RegisterCount);
   m_registers.store("zero", 0);
   m_registers.store("sp", programSizeBytes);

   m_registers.store("ra", ProgramEndRAValue);
}

void CPU::executeRegister(const std::string& name, const std::string& rd, const std::string& rs1, const std::string& rs2)
{
   std::int32_t rs1i = m_registers.load(rs1);
   std::int32_t rs2i = m_registers.load(rs2);
   std::int32_t rdVal = 0;

   if     (name == "add")  InstructionExecutor::executeAdd(rdVal, rs1i, rs2i);
   else if(name == "sub")  InstructionExecutor::executeSub(rdVal, rs1i, rs2i);
   else if(name == "sll")  InstructionExecutor::executeSll(rdVal, rs1i, rs2i);
   else if(name == "srl")  InstructionExecutor::executeSrl(rdVal, rs1i, rs2i);
   else if(name == "sra")  InstructionExecutor::executeSra(rdVal, rs1i, rs2i);
   else if(name == "slt")  InstructionExecutor::executeSlt(rdVal, rs1i, rs2i);
   else if(name == "sltu") InstructionExecutor::executeSltu(rdVal, rs1i, rs2i);
   else if(name == "or")   InstructionExecutor::executeOr(rdVal, rs1i, rs2i);
   else if(name == "xor")  InstructionExecutor::executeXor(rdVal, rs1i, rs2i);
   else if(name == "and")  InstructionExecutor::executeAnd(rdVal, rs1i, rs2i);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported register instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeImmediate(const std::string& name, const std::string& rd, const std::string& rs1, const std::string& imm)
{
   std::int32_t rs1i = m_registers.load(rs1);
   std::int32_t rdVal = 0;
   std::int32_t immi = 0;

   try
   {
      immi = stoi(imm);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Unable to convert immediate with value " << imm << std::endl;
      abort();
   }

   if     (name == "addi")  InstructionExecutor::executeAddi(rdVal, rs1i, immi);
   else if(name == "slli")  InstructionExecutor::executeSlli(rdVal, rs1i, immi);
   else if(name == "srli")  InstructionExecutor::executeSrli(rdVal, rs1i, immi);
   else if(name == "srai")  InstructionExecutor::executeSrai(rdVal, rs1i, immi);
   else if(name == "slti")  InstructionExecutor::executeSlti(rdVal, rs1i, immi);
   else if(name == "sltiu") InstructionExecutor::executeSltiu(rdVal,rs1i, immi);
   else if(name == "ori")   InstructionExecutor::executeOri(rdVal, rs1i, immi);
   else if(name == "xori")  InstructionExecutor::executeXori(rdVal, rs1i, immi);
   else if(name == "andi")  InstructionExecutor::executeAndi(rdVal, rs1i, immi);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported immediate instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeUpper(const std::string& name, const std::string& rd, const std::string& imm)
{
   std::int32_t rdVal = 0;
   std::int32_t immi20 = 0;

   try
   {
      immi20 = stoi(imm);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Unable to convert immediate with value " << imm << std::endl;
      abort();
   }

   if     (name == "lui")   InstructionExecutor::executeLui(rdVal, immi20);
   else if(name == "auipc") InstructionExecutor::executeAuipc(rdVal, immi20, m_PC);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported upper instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeBranch(const std::string& name, std::uint8_t instructionSize, const std::string& rs1, const std::string& rs2, const std::string& offset)
{
   std::int32_t rs1i = m_registers.load(rs1);
   std::int32_t rs2i = m_registers.load(rs2);
   std::int32_t offi12 = 0;
   std::uint32_t pcVal = m_PC;

   try
   {
      offi12 = stoi(offset);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Unable to convert immediate with value " << offset << std::endl;
      abort();
   }

   // Bitshift the immediate left 1 bit to add the "hidden" 0-bit in the LSB that isn't part of the instruction, but is "added" by the CPU because any instruction is to be at least 2-byte aligned.
   // Not needing to add the last 0 in the immediate means there's instead space for an extra bit in the MSB, doubling the range of the offset.
   offi12 <<= 1;

   if     (name == "beq")  InstructionExecutor::executeBeq(rs1i, rs2i, offi12, pcVal, instructionSize);
   else if(name == "bne")  InstructionExecutor::executeBne(rs1i, rs2i, offi12, pcVal, instructionSize);
   else if(name == "blt")  InstructionExecutor::executeBlt(rs1i, rs2i, offi12, pcVal, instructionSize);
   else if(name == "bltu") InstructionExecutor::executeBltu(rs1i, rs2i, offi12, pcVal, instructionSize);
   else if(name == "bge")  InstructionExecutor::executeBge(rs1i, rs2i, offi12, pcVal, instructionSize);
   else if(name == "bgeu") InstructionExecutor::executeBgeu(rs1i, rs2i, offi12, pcVal, instructionSize);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported branch instruction " << name << std::endl;
      abort();
   }

   m_PC = pcVal;
}

void CPU::executeJump(const std::string& name, std::uint8_t instructionSize, const std::string& rd, const std::string& offset)
{
   std::int32_t offseti = 0;
   std::uint32_t pcVal = m_PC;
   std::int32_t rdVal = 0;

   try
   {
      offseti = stoi(offset);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Unable to convert offset with value " << offset << std::endl;
      abort();
   }

   // Bitshift the immediate left 1 bit to add the "hidden" 0-bit in the LSB that isn't part of the instruction, but is "added" by the CPU because any instruction is to be at least 2-byte aligned.
   // Not needing to add the last 0 in the immediate means there's instead space for an extra bit in the MSB, doubling the range of the offset.
   offseti <<= 1;

   if(name == "jal") InstructionExecutor::executeJal(rdVal, offseti, pcVal, instructionSize);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported jump instruction " << name << std::endl;
      abort();
   }

   m_PC = pcVal;
   m_registers.store(rd, rdVal);
}

void CPU::executeJumpRegister(const std::string& name, std::uint8_t instructionSize, const std::string& rd, const std::string& target)
{
   std::int32_t rs1 = 0;
   std::int32_t offset = 0;
   std::uint32_t pcVal = m_PC;
   std::int32_t rdVal = 0;

   resolveRsOffset(target, offset, rs1);

   if(name == "jalr") InstructionExecutor::executeJalr(rdVal, offset, rs1, pcVal, instructionSize);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported jump register instruction " << name << std::endl;
      abort();
   }

   m_PC = pcVal;
   m_registers.store(rd, rdVal);
}

void CPU::executeLoad(const std::string& name, const std::string& rd, const std::string& address, ExecutableProgram& program)
{
   std::int32_t rs1 = 0;
   std::int32_t offset = 0;
   std::int32_t rdVal = 0;

   resolveRsOffset(address, offset, rs1);

   if     (name == "lw")  InstructionExecutor::executeLw(rdVal, offset, rs1, program);
   else if(name == "lh")  InstructionExecutor::executeLh(rdVal, offset, rs1, program);
   else if(name == "lb")  InstructionExecutor::executeLb(rdVal, offset, rs1, program);
   else if(name == "lhu") InstructionExecutor::executeLhu(rdVal, offset, rs1, program);
   else if(name == "lbu") InstructionExecutor::executeLbu(rdVal, offset, rs1, program);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported load instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeStore(const std::string& name, const std::string& rs, const std::string& address, ExecutableProgram& program)
{
   std::int32_t offset = 0;
   std::int32_t rs1 = 0;
   std::int32_t rs2 = m_registers.load(rs);

   resolveRsOffset(address, offset, rs1);

   if     (name == "sw") InstructionExecutor::executeSw(rs2, offset, rs1, program);
   else if(name == "sh") InstructionExecutor::executeSh(rs2, offset, rs1, program);
   else if(name == "sb") InstructionExecutor::executeSb(rs2, offset, rs1, program);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported store instruction " << name << std::endl;
      abort();
   }
}

void CPU::executeSystem(const std::string& name)
{
   if(name == "ecall")
   {
      m_registers.printRegistry();
      std::cout << std::endl;

      std::int32_t statusVal = m_registers.load("a0");
      if(statusVal == 0)
      {
         std::cout << "RV32I TEST PASSED" << std::endl;
      }
      else
      {
         std::int32_t failedTestNum = m_registers.load("gp");
         std::cout << "RV32I TEST FAILED" << std::endl;
         std::cout << "Test number " << failedTestNum << " failed" << std::endl;
         abort(); // A bit excessive, but practicing what seems to be the similar behavior for the RISC-V "official" instruction tests
      }
   }
   //else if(name == "ebreak") ;
   //else if(name == "sb") ;
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported system instruction " << name << std::endl;
      abort();
   }
}

void CPU::resolveRsOffset(const std::string& offset, std::int32_t& offseti, std::int32_t& rs1)
{
   std::string offsetTemp;
   std::string rs;
   ParseUtils::parseRegisterOffset(offset, offsetTemp, rs);

   rs1 = m_registers.load(rs);
   try
   {
      offseti = stoi(offsetTemp);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Failed to parse offset = " + offset << std::endl;
      abort();
   }
}

}
}
