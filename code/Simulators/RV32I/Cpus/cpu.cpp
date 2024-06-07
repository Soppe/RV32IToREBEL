#include "cpu.h"

#include <parseutils.h>
#include <Simulators/RV32I/simulatorutils.h>
#include <Simulators/RV32I/executableprogram.h>
#include <Expressions/instruction.h>

#include <iostream>

namespace
{
//======================================
// Register instructions
//======================================
void executeAdd(int& rd, int rs1, int rs2)
{
   rd = rs1 + rs2;
}

void executeSub(int& rd, int rs1, int rs2)
{
   rd = rs1 - rs2;
}

void executeSll(int& rd, int rs1, int rs2)
{
   rd = rs1 << rs2;
}

void executeSrl(int& rd, int rs1, int rs2)
{
   rs2 = rs2 & 0x1f; // Use only the 5 lowest bits

   if(rs2 == 0)
   {
      rd = rs1;
   }
   else
   {
      rd = rs1 >> rs2;

      // Since the >> operand in c++ functions as sra we have to simulate shifting 0's into the upper bits
      int mask = 0x7fffffff;
      mask = mask >> (rs2 - 1);
      rd = rd & mask;
   }
}

void executeSra(int& rd, int rs1, int rs2)
{
   rd = rs1 >> rs2;
}

void executeSlt(int& rd, int rs1, int rs2)
{
   if(rs1 < rs2)
   {
      rd = 1;
   }
}

void executeSltu(int& rd, int rs1, int rs2)
{
   uint rs1u = rs1;
   uint rs2u = rs2;

   if(rs1u < rs2u)
   {
      rd = 1;
   }
}

void executeOr(int& rd, int rs1, int rs2)
{
   rd = rs1 | rs2;
}

void executeXor(int& rd, int rs1, int rs2)
{
   rd = rs1 ^ rs2;
}

void executeAnd(int& rd, int rs1, int rs2)
{
   rd = rs1 & rs2;
}

//======================================
// Immediate instructions
//======================================
void executeAddi(int& rd, int rs1, int imm)
{

   if((imm > 0x7ff) || (imm < (int)0xfffff800))
   {
      std::cerr << __PRETTY_FUNCTION__ << "Illegal value " << imm << std::endl;
      abort();
   }

   int imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   rd = rs1 + imm12;
}


void executeSlli(int& rd, int rs1, int imm)
{
   // This is based on tests with the GCC assembler using values represented with more than 5 bits, and negative numbers, where errors related to negative numbers inferred they had been converted
   // to signed values first - a low negative number resulted in an error referring to close to the max value of a 64 bit unsigned integer
   uint immu = imm;
   if(immu > 0x1f)
   {
      std::cerr << __PRETTY_FUNCTION__ << "Value too large: " << immu << std::endl;
      abort();
   }

   rd = rs1 << immu;
}

void executeSrli(int& rd, int rs1, int imm)
{
   // This is based on tests with the GCC assembler using values represented with more than 5 bits, and negative numbers, where errors related to negative numbers inferred they had been converted
   // to signed values first - a low negative number resulted in an error referring to close to the max value of a 64 bit unsigned integer
   uint immu = imm;
   if(immu > 0x1f)
   {
      std::cerr << __PRETTY_FUNCTION__ << "Value too large: " << immu << std::endl;
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
      int mask = 0x7fffffff;
      mask = mask >> (immu - 1);
      rd = rd & mask;
   }
}

void executeSrai(int& rd, int rs1, int imm)
{
   // This is based on tests with the GCC assembler using values represented with more than 5 bits, and negative numbers, where errors related to negative numbers inferred they had been converted
   // to signed values first - a low negative number resulted in an error referring to close to the max value of a 64 bit unsigned integer
   uint immu = imm;
   if(immu > 0x1f)
   {
      std::cerr << __PRETTY_FUNCTION__ << "Value too large: " << immu << std::endl;
      abort();
   }

   rd = rs1 >> immu;
}

void executeSlti(int& rd, int rs1, int imm)
{
   if((imm > 0x7ff) || (imm < (int)0xfffff800))
   {
      std::cerr << __PRETTY_FUNCTION__ << "Illegal value " << imm << std::endl;
      abort();
   }

   int imm12;
   ParseUtils::parseImmediate(12, imm, imm12);


   if(rs1 < imm12)
   {
      rd = 1;
   }
}

// The standard says the imm value is to first be sign-extended, then converted to an unsigned value. Ergo, the limits for sltiu are the same as for slti.
void executeSltiu(int& rd, int rs1, int imm)
{
   if((imm > 0x7ff) || (imm < (int)0xfffff800))
   {
      std::cerr << __PRETTY_FUNCTION__ << "Illegal value " << imm << std::endl;
      abort();
   }

   int imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   uint rs1u = rs1;
   uint immu12 = imm12;

   if(rs1u < immu12)
   {
      rd = 1;
   }
}

void executeOri(int& rd, int rs1, int imm)
{
   if((imm > 0x7ff) || (imm < (int)0xfffff800))
   {
      std::cerr << __PRETTY_FUNCTION__ << "Illegal value " << imm << std::endl;
      abort();
   }

   int imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   rd = rs1 | imm12;
}

void executeXori(int& rd, int rs1, int imm)
{
   if((imm > 0x7ff) || (imm < (int)0xfffff800))
   {
      std::cerr << __PRETTY_FUNCTION__ << "Illegal value " << imm << std::endl;
      abort();
   }

   int imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   rd = rs1 ^ imm12;
}

void executeAndi(int& rd, int rs1, int imm)
{
   if((imm > 0x7ff) || (imm < (int)0xfffff800))
   {
      std::cerr << __PRETTY_FUNCTION__ << "Illegal value " << imm << std::endl;
      abort();
   }

   int imm12;
   ParseUtils::parseImmediate(12, imm, imm12);

   rd = rs1 & imm12;
}

//======================================
// Upper instructions
//======================================
void executeLui(int& rd, int imm20)
{
   ParseUtils::parseImmediate(20, imm20, imm20);
   rd = imm20 << 12;
}

void executeAuipc(int& rd, int imm20, int pc)
{
   ParseUtils::parseImmediate(20, imm20, imm20);
   rd = (imm20 << 12) + pc;
}

//======================================
// Branch instructions
//======================================
void executeBeq(int rs1, int rs2, int offset, int& pc)
{
   if(rs1 == rs2)
   {
      pc = pc + offset - 4; // Subtract 4 since simulator automatically adds 4 to PC after each instruction call
   }
}

void executeBne(int rs1, int rs2, int offset, int& pc)
{
   if(rs1 != rs2)
   {
      pc = pc + offset - 4; // Subtract 4 since simulator automatically adds 4 to PC after each instruction call
   }
}

void executeBlt(int rs1, int rs2, int offset, int& pc)
{
   if(rs1 < rs2)
   {
      pc = pc + offset - 4; // Subtract 4 since simulator automatically adds 4 to PC after each instruction call
   }
}

void executeBltu(int rs1, int rs2, int offset, int& pc)
{
   uint rs1u = rs1;
   uint rs2u = rs2;

   if(rs1u < rs2u)
   {
      pc = pc + offset - 4; // Subtract 4 since simulator automatically adds 4 to PC after each instruction call
   }
}

void executeBge(int rs1, int rs2, int offset, int& pc)
{
   if(rs1 >= rs2)
   {
      pc = pc + offset - 4; // Subtract 4 since simulator automatically adds 4 to PC after each instruction call
   }
}

void executeBgeu(int rs1, int rs2, int offset, int& pc)
{
   uint rs1u = rs1;
   uint rs2u = rs2;
   if(rs1u >= rs2u)
   {
      pc = pc + offset - 4; // Subtract 4 since simulator automatically adds 4 to PC after each instruction call
   }
}

//======================================
// Jump instructions
//======================================
void executeJal(int& rd, int offset, int& pc)
{
   ParseUtils::parseImmediate(12, offset, offset);
   rd = pc + 4;
   pc = pc + offset - 4; // Subtract 4 since simulator automatically adds 4 to PC after each instruction call
}

//======================================
// Jump register instructions
//======================================
void executeJalr(int& rd, int target, int& pc)
{
   rd = pc + 4;
   pc = target - 4; // Subtract 4 since simulator automatically adds 4 to PC after each instruction call
}

//======================================
// Load instructions
//======================================
void executeLw(int& rd, int srcAddress, Simulators::RV32I::ExecutableProgram& program)
{
   std::cout << "Wanting to load something from source = " << srcAddress << std::endl;
   rd = program.loadFromHeap(srcAddress, 4);
   ParseUtils::parseImmediate(32, rd, rd);
   std::cout << "Value read from memory is " << rd << std::endl;
}

void executeLh(int& rd, int srcAddress, Simulators::RV32I::ExecutableProgram& program)
{
   rd = program.loadFromHeap(srcAddress, 2);
   ParseUtils::parseImmediate(16, rd, rd);
}

void executeLb(int& rd, int srcAddress, Simulators::RV32I::ExecutableProgram& program)
{
   rd = program.loadFromHeap(srcAddress, 1);
   ParseUtils::parseImmediate(8, rd, rd);
}

void executeLhu(int& rd, int srcAddress, Simulators::RV32I::ExecutableProgram& program)
{
   rd = program.loadFromHeap(srcAddress, 2);
}

void executeLbu(int& rd, int srcAddress, Simulators::RV32I::ExecutableProgram& program)
{
   rd = program.loadFromHeap(srcAddress, 1);
}

//======================================
// Store instructions
//======================================
void executeSw(int rs, int targetAddress, Simulators::RV32I::ExecutableProgram& program)
{
   program.storeToHeap(targetAddress, rs, 4);
}

void executeSh(int rs, int targetAddress, Simulators::RV32I::ExecutableProgram& program)
{
   program.storeToHeap(targetAddress, rs, 2);
}

void executeSb(int rs, int targetAddress, Simulators::RV32I::ExecutableProgram& program)
{
   program.storeToHeap(targetAddress, rs, 1);
}
}

namespace Simulators
{
namespace RV32I
{

CPU::CPU()
    : m_PC(0)
    , m_accumulatedCost(0)
{
}

CPU::~CPU()
{

}

void CPU::executeProgram(ExecutableProgram& program)
{
   initRegisters(program.getProgramSize());

   int instructionSize = 0;
   const Expressions::Instruction* instr = program.loadInstruction(m_PC, instructionSize);

   SimulatorUtils::InstructionType type = SimulatorUtils::InstructionType::UNDEFINED;

   while(instr != nullptr)
   {
      const std::string& name = instr->getInstructionName();
      const std::vector<std::string>& operands = instr->getInstructionOperands();
      type = SimulatorUtils::getInstructionType(name);
      std::cout << "pc = " << m_PC << "\t";
      instr->print();
      std::cout << std::endl;

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
         executeBranch(name, operands[0], operands[1], operands[2]);
         break;
      case SimulatorUtils::InstructionType::JUMP:
         executeJump(name, operands[0], operands[1]);
         break;
      case SimulatorUtils::InstructionType::JUMP_REGISTER:
         executeJumpRegister(name, operands[0], operands[1]);
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
         std::cerr << __PRETTY_FUNCTION__ << ": Undefined instruction " << name << std::endl;
      }
      m_PC += instructionSize;
      instr = program.loadInstruction(m_PC, instructionSize);
   }

   std::cout << "Finished executing program" << std::endl;
}

void CPU::executeRegister(const std::string& name, const std::string& rd, const std::string& rs1, const std::string& rs2)
{
   int rs1i = m_registers.load(rs1);
   int rs2i = m_registers.load(rs2);
   int rdVal = 0;

   if     (name == "add")  executeAdd(rdVal, rs1i, rs2i);
   else if(name == "sub")  executeSub(rdVal, rs1i, rs2i);
   else if(name == "sll")  executeSll(rdVal, rs1i, rs2i);
   else if(name == "srl")  executeSrl(rdVal, rs1i, rs2i);
   else if(name == "sra")  executeSra(rdVal, rs1i, rs2i);
   else if(name == "slt")  executeSlt(rdVal, rs1i, rs2i);
   else if(name == "sltu") executeSltu(rdVal, rs1i, rs2i);
   else if(name == "or")   executeOr(rdVal, rs1i, rs2i);
   else if(name == "xor")  executeXor(rdVal, rs1i, rs2i);
   else if(name == "and")  executeAnd(rdVal, rs1i, rs2i);
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported register instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeImmediate(const std::string& name, const std::string& rd, const std::string& rs1, const std::string& imm)
{
   int rs1i = m_registers.load(rs1);
   int rdVal = 0;
   int immi = 0;

   try
   {
      immi = stoi(imm);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unable to convert immediate with value " << imm << std::endl;
      abort();
   }

   if     (name == "addi")  executeAddi(rdVal, rs1i, immi);
   else if(name == "slli")  executeSlli(rdVal, rs1i, immi);
   else if(name == "srli")  executeSrli(rdVal, rs1i, immi);
   else if(name == "srai")  executeSrai(rdVal, rs1i, immi);
   else if(name == "slti")  executeSlti(rdVal, rs1i, immi);
   else if(name == "sltiu") executeSltiu(rdVal,rs1i, immi);
   else if(name == "ori")   executeOri(rdVal, rs1i, immi);
   else if(name == "xori")  executeXori(rdVal, rs1i, immi);
   else if(name == "andi")  executeAndi(rdVal, rs1i, immi);
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported immediate instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeUpper(const std::string& name, const std::string& rd, const std::string& imm)
{
   int rdVal = 0;
   int immi20 = 0;

   try
   {
      immi20 = stoi(imm);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unable to convert immediate with value " << imm << std::endl;
      abort();
   }

   // GCC compiler will throw an error if any of the 12 MSB is set. We'll do the same.
   if((immi20 & 0xfff00000) != 0)
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Illegal value " << immi20 << std::endl;
      abort();
   }

   if     (name == "lui") executeLui(rdVal, immi20);
   else if(name == "auipc") executeAuipc(rdVal, immi20, m_PC);
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported upper instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);

}

void CPU::executeBranch(const std::string& name, const std::string& rs1, const std::string& rs2, const std::string& offset)
{
   int rs1i = m_registers.load(rs1);
   int rs2i = m_registers.load(rs2);
   int offi12 = 0;
   int pcVal = m_PC;

   try
   {
      offi12 = stoi(offset);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unable to convert immediate with value " << offset << std::endl;
      abort();
   }

   // Bitshift the immediate left 1 bit to compensate for how there is a "hidden" 0-bit in the LSB that isn't part of the instruction per the branch documentation
   if((offi12 > (0x7ff << 1)) || (offi12 < (int)(0xfffff800 << 1)))
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Illegal value " << offi12 << std::endl;
      abort();
   }

   if     (name == "beq")  executeBeq(rs1i, rs2i, offi12, pcVal);
   else if(name == "bne")  executeBne(rs1i, rs2i, offi12, pcVal);
   else if(name == "blt")  executeBlt(rs1i, rs2i, offi12, pcVal);
   else if(name == "bltu") executeBltu(rs1i, rs2i, offi12, pcVal);
   else if(name == "bge")  executeBge(rs1i, rs2i, offi12, pcVal);
   else if(name == "bgeu") executeBgeu(rs1i, rs2i, offi12, pcVal);
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported branch instruction " << name << std::endl;
      return;
   }

   m_PC = pcVal;
}

void CPU::executeJump(const std::string& name, const std::string& rd, const std::string& offset)
{
   int offseti = 0;
   int pcVal = m_PC;
   int rdVal = 0;

   try
   {
      offseti = stoi(offset);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unable to convert offset with value " << offset << std::endl;
      abort();
   }

   // Bitshift the immediate left 1 bit to compensate for how there is a "hidden" 0-bit in the LSB that isn't part of the instruction per the jump documentation
   if((offseti > (0x0007ffff << 1)) || (offseti < (int)(0xfff80000 << 1)))
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Illegal value " << offseti << std::endl;
      abort();
   }

   if(name == "jal") executeJal(rdVal, offseti, pcVal);
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported jump instruction " << name << std::endl;
      abort();
   }

   m_PC = pcVal;
   m_registers.store(rd, rdVal);
}

void CPU::executeJumpRegister(const std::string& name, const std::string& rd, const std::string& target)
{
   int targeti = 0;
   int pcVal = m_PC;
   int rdVal = 0;

   resolve12ImmOffset(target, targeti);

   if(name == "jalr") executeJalr(rdVal, targeti, pcVal);
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported jump register instruction " << name << std::endl;
      abort();
   }

   m_PC = pcVal;
   m_registers.store(rd, rdVal);
}

void CPU::executeLoad(const std::string& name, const std::string& rd, const std::string& address, ExecutableProgram& program)
{
   int addressi = 0;
   int rdVal = 0;

   resolve12ImmOffset(address, addressi);

   if     (name == "lw")  executeLw(rdVal, addressi, program);
   else if(name == "lh")  executeLh(rdVal, addressi, program);
   else if(name == "lb")  executeLb(rdVal, addressi, program);
   else if(name == "lhu") executeLhu(rdVal, addressi, program);
   else if(name == "lbu") executeLbu(rdVal, addressi, program);
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported load instruction " << name << std::endl;
      abort();
   }

   m_registers.store(rd, rdVal);
}

void CPU::executeStore(const std::string& name, const std::string& rs, const std::string& address, ExecutableProgram& program)
{
   int addressi = 0;
   int rsi = m_registers.load(rs);

   resolve12ImmOffset(address, addressi);

   if     (name == "sw") executeSw(rsi, addressi, program);
   else if(name == "sh") executeSh(rsi, addressi, program);
   else if(name == "sb") executeSb(rsi, addressi, program);
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported store instruction " << name << std::endl;
      abort();
   }
}

void CPU::executeSystem(const std::string& name)
{
   if(name == "ecall")
   {
      m_registers.printRegistry();
      std::cout << std::endl;

      int statusVal = m_registers.load("a0");
      if(statusVal == 0)
      {
         std::cout << "TEST PASSED" << std::endl;
      }
      else
      {
         int failedTestNum = m_registers.load("gp");
         std::cout << "TEST FAILED" << std::endl;
         std::cout << "Test number " << failedTestNum << " failed" << std::endl;
         abort(); // A bit excessive, but practicing what seems to be the similar behavior for the RISC-V "official" instruction tests
      }
   }
   //else if(name == "ebreak") ;
   //else if(name == "sb") ;
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Unsupported system instruction " << name << std::endl;
      abort();
   }
}

void CPU::resolve12ImmOffset(const std::string& offset, int& value)
{
   std::string off12;
   std::string rs;
   ParseUtils::parseRegisterOffset(offset, off12, rs);

   int regVal = m_registers.load(rs);
   int offi12 = 0;
   try
   {
      offi12 = stoi(off12);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Failed to parse offset = " + off12 << std::endl;
      abort();
   }

   if((offi12 > 0x7ff) || (offi12 < (int)0xfffff800))
   {
      std::cerr << __PRETTY_FUNCTION__ << ": Illegal value " << offi12 << std::endl;
      abort();
   }

   value = regVal + offi12;
}

int CPU::getAccumulatedCost() const
{
   return m_accumulatedCost;
}

void CPU::initRegisters(int programSize)
{
   m_registers.store("zero", 0);
   m_registers.store("sp", programSize);
}

}
}
