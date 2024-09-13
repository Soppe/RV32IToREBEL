#include "cpu.h"

#include "instructionexecutor.h"

#include <logger.h>

#include <Expressions/instruction.h>
#include <Parsers/parseutils.h>
#include <Simulators/REBEL6/executableprogram.h>
#include <Simulators/REBEL6/simulatorutils.h>

#include <iostream>
#include <math.h>

namespace
{
// Set ra to some irrational and easily detectable value. GCC, and possibly other compilers, adds a "jr ra" at the end of the main routine to return to some caller.
// This value is used to detect when that happens.
const std::int32_t ProgramEndRAValue = -std::pow(3, 16); // Just some random number negative where only a single trit is set, minimizing the impact it has on tritflips
const std::int16_t RegisterCount = std::pow(3, 6); // 729
}

namespace Simulators
{
namespace REBEL6
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
   m_PC = 0;
   m_numberOfRanInstructions = 0;

   initRegisters(program.getProgramSizeTrytes());

   std::uint8_t instructionSize = 0;
   const Expressions::Instruction* instr = program.loadInstruction(m_PC, instructionSize);

   SimulatorUtils::InstructionType type = SimulatorUtils::InstructionType::UNDEFINED;

   while(instr != nullptr)
   {
      const std::string& name = instr->getInstructionName();
      const std::vector<std::string>& operands = instr->getInstructionOperands();
      type = SimulatorUtils::getInstructionType(name);
      bool isBinary = !name.ends_with(".t");
      // std::cout << "pc = " << m_PC << "\t" << *instr << std::endl;

      switch(type)
      {
      case SimulatorUtils::InstructionType::REGISTER:
         executeRegister(name, isBinary, operands[0], operands[1], operands[2]);
         break;
      case SimulatorUtils::InstructionType::IMMEDIATE:
         executeImmediate(name, isBinary, operands[0], operands[1], operands[2]);
         break;
      case SimulatorUtils::InstructionType::LOAD_IMMEDIATE:
         executeLoadImmediate(name, operands[0], operands[1]);
         break;
      case SimulatorUtils::InstructionType::BRANCH:
         executeBranch(name, isBinary, instructionSize, operands[0], operands[1], operands[2]);
         break;
      case SimulatorUtils::InstructionType::JUMP:
         executeJump(name, instructionSize, operands[0], operands[1]);
         break;
      case SimulatorUtils::InstructionType::JUMP_REGISTER:
         executeJumpRegister(name, instructionSize, operands[0], operands[1]);
         break;
      case SimulatorUtils::InstructionType::LOAD:
         executeLoad(name, isBinary, operands[0], operands[1], program);
         break;
      case SimulatorUtils::InstructionType::STORE:
         executeStore(name, isBinary, operands[0], operands[1], program);
         break;
      case SimulatorUtils::InstructionType::SYSTEM:
         executeSystem(name);
         break;
      default:
         std::cerr << __PRETTY_FUNC__ << ": Undefined instruction " << name << std::endl;
         abort();
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

uint32_t CPU::getTritFlipCost() const
{
   return m_registers.getAccumulatedTritFlips();
}

void CPU::initRegisters(std::int32_t programSizeTrytes)
{
   m_registers.reset(RegisterCount);
   m_registers.store("zero", 0);
   m_registers.store("sp", programSizeTrytes);

   m_registers.store("ra", ProgramEndRAValue);
}

void CPU::executeRegister(const std::string& name, bool isBinary, const std::string& rd, const std::string& rs1, const std::string& rs2)
{
   Tint rs1i = m_registers.load(rs1);
   Tint rs2i = m_registers.load(rs2);
   Tint rdVal = 0;

   if(isBinary)
   {
      if     (name == "add")  InstructionExecutor::executeAdd(rdVal, rs1i, rs2i);
      else if(name == "sub")  InstructionExecutor::executeSub(rdVal, rs1i, rs2i);
      else if(name == "sll")  InstructionExecutor::executeSll(rdVal, rs1i, rs2i);
      // else if(name == "srl")  InstructionExecutor::executeSrl(rdVal, rs1i, rs2i);
      // else if(name == "sra")  InstructionExecutor::executeSra(rdVal, rs1i, rs2i);
      // //else if(name == "slt")  InstructionExecutor::executeSlt(rdVal, rs1i, rs2i);
      else if(name == "sltu") InstructionExecutor::executeSltu(rdVal, rs1i, rs2i);
      else if(name == "or")   InstructionExecutor::executeOr(rdVal, rs1i, rs2i);
      // else if(name == "xor")  InstructionExecutor::executeXor(rdVal, rs1i, rs2i);
      else if(name == "and")  InstructionExecutor::executeAnd(rdVal, rs1i, rs2i);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported binary register instruction " << name << std::endl;
         abort();
      }
   }
   else
   {
      if(false){}
      // if     (name == "add.t")  InstructionExecutor::executeAdd_t(rdVal, rs1i, rs2i);
      // else if(name == "sub.t")  InstructionExecutor::executeSub_t(rdVal, rs1i, rs2i);
      // else if(name == "sl.t")  InstructionExecutor::executeSl_t(rdVal, rs1i, rs2i);
      // else if(name == "sr.t")  InstructionExecutor::executeSr_t(rdVal, rs1i, rs2i);
      else if(name == "slt.t")  InstructionExecutor::executeSlt_t(rdVal, rs1i, rs2i);
      // else if(name == "or.t")   InstructionExecutor::executeOr_t(rdVal, rs1i, rs2i);
      // else if(name == "xor.t")  InstructionExecutor::executeXor_t(rdVal, rs1i, rs2i);
      // else if(name == "and.t")  InstructionExecutor::executeAnd_t(rdVal, rs1i, rs2i);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported ternary register instruction " << name << std::endl;
         abort();
      }
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeImmediate(const std::string& name, bool isBinary, const std::string& rd, const std::string& rs1, const std::string& imm)
{
   Tint rs1i = m_registers.load(rs1);
   Tint rdVal = 0;
   Tint immi = 0;

   try
   {
      immi = stoll(imm);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Unable to convert immediate with value " << imm << std::endl;
      abort();
   }

   if(isBinary)
   {
      if     (name == "addi")  InstructionExecutor::executeAddi(rdVal, rs1i, immi);
      else if(name == "slli")  InstructionExecutor::executeSlli(rdVal, rs1i, immi);
      // else if(name == "srli")  InstructionExecutor::executeSrli(rdVal, rs1i, immi);
      // else if(name == "srai")  InstructionExecutor::executeSrai(rdVal, rs1i, immi);
      // //else if(name == "slti")  InstructionExecutor::executeSlti(rdVal, rs1i, immi);
      else if(name == "sltiu") InstructionExecutor::executeSltiu(rdVal,rs1i, immi);
      else if(name == "ori")   InstructionExecutor::executeOri(rdVal, rs1i, immi);
      // else if(name == "xori")  InstructionExecutor::executeXori(rdVal, rs1i, immi);
      else if(name == "andi")  InstructionExecutor::executeAndi(rdVal, rs1i, immi);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported binary immediate instruction " << name << std::endl;
         abort();
      }
   }
   else
   {
      if     (name == "addi.t")  InstructionExecutor::executeAddi_t(rdVal, rs1i, immi);
      // else if(name == "sli.t")  InstructionExecutor::executeSli_t(rdVal, rs1i, immi);
      // else if(name == "sri.t")  InstructionExecutor::executeSri_t(rdVal, rs1i, immi);
      else if(name == "slti.t")  InstructionExecutor::executeSlti_t(rdVal, rs1i, immi);
      // else if(name == "ori.t")   InstructionExecutor::executeOri_t(rdVal, rs1i, immi);
      // else if(name == "xori.t")  InstructionExecutor::executeXori_t(rdVal, rs1i, immi);
      // else if(name == "andi.t")  InstructionExecutor::executeAndi_t(rdVal, rs1i, immi);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported ternary immediate instruction " << name << std::endl;
         abort();
      }
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeLoadImmediate(const std::string& name, const std::string& rd, const std::string& imm)
{
   Tint rdVal = 0;
   Tint immi = 0;

   try
   {
      immi = stoll(imm);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Unable to convert immediate with value " << imm << std::endl;
      abort();
   }

   if     (name == "li.t")   InstructionExecutor::executeLi_t(rdVal, immi);
   else if(name == "aipc.t") InstructionExecutor::executeAipc_t(rdVal, immi, m_PC);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported ternary immediate instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeBranch(const std::string& name, bool isBinary, std::uint8_t instructionSize, const std::string& rs1, const std::string& rs2, const std::string& offset)
{
   Tint rs1i = m_registers.load(rs1);
   Tint rs2i = m_registers.load(rs2);
   std::int32_t offi16 = 0;
   std::int32_t pcVal = m_PC;

   try
   {
      offi16 = stoi(offset);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Unable to convert immediate with value " << offset << std::endl;
      abort();
   }

   if(isBinary)
   {
      if     (name == "bgeu")  InstructionExecutor::executeBgeu(rs1i, rs2i, offi16, pcVal, instructionSize);
      else if(name == "bltu")  InstructionExecutor::executeBltu(rs1i, rs2i, offi16, pcVal, instructionSize);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported binary immediate instruction " << name << std::endl;
         abort();
      }
   }
   else
   {
      if     (name == "beq.t")  InstructionExecutor::executeBeq_t(rs1i, rs2i, offi16, pcVal, instructionSize);
      else if(name == "bne.t")  InstructionExecutor::executeBne_t(rs1i, rs2i, offi16, pcVal, instructionSize);
      else if(name == "blt.t")  InstructionExecutor::executeBlt_t(rs1i, rs2i, offi16, pcVal, instructionSize);
      else if(name == "bge.t")  InstructionExecutor::executeBge_t(rs1i, rs2i, offi16, pcVal, instructionSize);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported branch instruction " << name << std::endl;
         abort();
      }
   }

   m_PC = pcVal;
}

void CPU::executeJump(const std::string& name, std::uint8_t instructionSize, const std::string& rd, const std::string& offset)
{

   std::int32_t offseti = 0;
   std::int32_t pcVal = m_PC;
   Tint rdVal = 0;

   try
   {
      offseti = stoi(offset);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Unable to convert offset with value " << offset << std::endl;
      abort();
   }

   if(name == "jal.t") InstructionExecutor::executeJal_t(rdVal, offseti, pcVal, instructionSize);
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
   Tint rs1 = 0;
   Tint offset = 0;
   std::int32_t pcVal = m_PC;
   Tint rdVal = 0;

   resolveRsOffset(target, offset, rs1);

   if(name == "jalr.t") InstructionExecutor::executeJalr_t(rdVal, offset, rs1, pcVal, instructionSize);
   else
   {
      std::cerr << __PRETTY_FUNC__ << ": Unsupported jump register instruction " << name << std::endl;
      abort();
   }

   m_PC = pcVal;
   m_registers.store(rd, rdVal);
}

void CPU::executeLoad(const std::string& name, bool isBinary, const std::string& rd, const std::string& address, ExecutableProgram& program)
{
   Tint rs1 = 0;
   Tint offset = 0;
   Tint rdVal = 0;

   resolveRsOffset(address, offset, rs1);

   if(isBinary)
   {
      if(false){}
      // if(name == "lw") InstructionExecutor::executeLw(rdVal, offset, rs1, program);
      else if(name == "lh") InstructionExecutor::executeLh(rdVal, offset, rs1, program);
      else if(name == "lb") InstructionExecutor::executeLb(rdVal, offset, rs1, program);
      // else if(name == "lhu") InstructionExecutor::executeLhu(rdVal, offset, rs1, program);
      else if(name == "lbu") InstructionExecutor::executeLbu(rdVal, offset, rs1, program);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported binary load instruction " << name << std::endl;
         abort();
      }
   }
   else
   {
      if(false){}
      // if     (name == "lw.t")  InstructionExecutor::executeLw_t(rdVal, offset, rs1, program);
      // else if(name == "lh.t")  InstructionExecutor::executeLh_t(rdVal, offset, rs1, program);
      // else if(name == "lt.t")  InstructionExecutor::executeLt_t(rdVal, offset, rs1, program);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported ternary load instruction " << name << std::endl;
         abort();
      }
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeStore(const std::string& name, bool isBinary, const std::string& rs, const std::string& address, ExecutableProgram& program)
{
   Tint offset = 0;
   Tint rs1 = 0;
   Tint rs2 = m_registers.load(rs);

   resolveRsOffset(address, offset, rs1);

   if(isBinary)
   {
      if(false){}
      // if     (name == "sw") InstructionExecutor::executeSw(rsi, addressi, program);
      // else if(name == "sh") InstructionExecutor::executeSh(rsi, addressi, program);
      // else if(name == "sb") InstructionExecutor::executeSb(rsi, addressi, program);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported binary store instruction " << name << std::endl;
         abort();
      }
   }
   else
   {
      if(false){}
      // if     (name == "sw.t") InstructionExecutor::executeSw_t(rsi, addressi, program);
      // else if(name == "sh.t") InstructionExecutor::executeSh_t(rsi, addressi, program);
      // else if(name == "st.t") InstructionExecutor::executeSb_t(rsi, addressi, program);
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported ternary store instruction " << name << std::endl;
         abort();
      }
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
         std::cout << "REBEL-6 TEST PASSED" << std::endl;
      }
      else
      {
         std::int32_t failedTestNum = m_registers.load("gp");
         std::cout << "REBEL-6 TEST FAILED" << std::endl;
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

void CPU::resolveRsOffset(const std::string& offset, Tint& offseti, Tint& rs1)
{
   std::string offsetTemp;
   std::string rs;
   ParseUtils::parseRegisterOffset(offset, offsetTemp, rs);

   rs1 = m_registers.load(rs);

   try
   {
      offseti = stoll(offsetTemp);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Failed to parse offset = " + offset << std::endl;
      abort();
   }
}

}
}

