#include "assemblerutils.h"

#include "executableprogram.h"
#include "simulatorutils.h"
#include "Cpu/registry.h"

#include <logger.h>
#include <Expressions/instruction.h>
#include <Parsers/parseutils.h>

#include <iostream>
#include <bitset>
#include <fstream>


namespace
{
std::unordered_map<std::string, int> Funct3Map =
    {
    // R - Register
    {"add", 0b000},
    {"sub", 0b000},
    {"sll", 0b001},
    {"srl", 0b101},
    {"sra", 0b101},
    {"slt", 0b010},
    {"sltu", 0b011},
    {"or", 0b110},
    {"xor", 0b100},
    {"and", 0b111},

    // I - Immediate
    {"addi", 0b000},
    {"slli", 0b001},
    {"srli", 0b101},
    {"srai", 0b101},
    {"slti", 0b010},
    {"sltiu", 0b011},
    {"ori", 0b110},
    {"xori", 0b100},
    {"andi", 0b111},

    // B - Branch
    {"beq", 0b000},
    {"bne", 0b001},
    {"blt", 0b100},
    {"bltu", 0b110},
    {"bge", 0b101},
    {"bgeu", 0b111},

    // I - Jump Register
    {"jalr", 0b000},

    // I - Load
    {"lw", 0b010},
    {"lh", 0b001},
    {"lb", 0b000},
    {"lhu", 0b101},
    {"lbu", 0b100},

    // S - Store
    {"sw", 0b010},
    {"sh", 0b001},
    {"sb", 0b000},

    // SYSTEM
    {"ecall", 0b000},
    {"ebreak", 0b000}
};

int getFunc3(const std::string& instructionName)
{
   int retVal;

   try
   {
      retVal = Funct3Map.at(instructionName);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Failed to find funct3 for instruction " << instructionName << std::endl;
      abort();
   }

   return retVal;
}

void machineFormatRegister(int& result, int opcode, int rd, int rs1, int rs2, int funct3, int funct7)
{
   result = opcode; // 7 bit opcode
   result |= (rd << 7); // 5 bit rd
   result |= (funct3 << 12); // 3 bit funct3
   result |= (rs1 << 15); // 5 bit rs1
   result |= (rs2 << 20); // 5 bit rs2
   result |= (funct7 << 25); // 7 bit funct7
}

void machineFormatImmediate(int& result, int opcode, int rd, int rs1, int imm, int funct3)
{
   result = opcode; // 7 bit opcode
   result |= (rd << 7); // 5 bit rd
   result |= (funct3 << 12); // 3 bit funct3
   result |= (rs1 << 15); // 5 bit rs1
   result |= (imm << 20); // 12 bit immediate [11:0]
}

void machineFormatStore(int& result, int opcode, int rs1, int rs2, int imm, int funct3)
{
   result = opcode; // 7 bit opcode
   result |= ((imm & 0x1F) << 7); // 5 bit - imm[4:0]
   result |= (funct3 << 12); // 3 bit funct3
   result |= (rs1 << 15); // 5 bit rs1
   result |= (rs2 << 20); // 5 bit rs2
   result |= (((imm & 0xFE0) >> 5) << 25); // 7 bit - imm[11:5]
}

void machineFormatBranch(int& result, int opcode, int rs1, int rs2, int imm, int funct3)
{
   result = opcode; // 7 bit opcode
   result |= (((imm & 0x800) >> 11) << 7); // 1 bit - imm[11]
   result |= (((imm & 0x1E) >> 1) << 8); // 4 bit - imm[4:1]
   result |= (funct3 << 12); // 3 bit funct3
   result |= (rs1 << 15); // 5 bit rs1
   result |= (rs2 << 20); // 5 bit rs2
   result |= (((imm & 0x7E0) >> 5) << 25); // 6 bit - imm[10:5]
   result |= (((imm & 0x1000) >> 12) << 31); // 1 bit - imm[12]
}

void machineFormatUpper(int& result, int opcode, int rd, int imm)
{
   result = opcode; // 7 bit opcode
   result |= (rd << 7); // 5 bit rd
   result |= (imm & 0xFFFFF000); // 20 bit immediate - imm[31:12]
}

void machineFormatJump(int& result, int opcode, int rd, int imm)
{
   result = opcode; // 7 bit opcode
   result |= (rd << 7); // 5 bit rd
   result |= (((imm & 0xFF000) >> 12) << 12); // 8 bit - imm[19:12]
   result |= (((imm & 0x800) >> 11) << 20); // 1 bit - imm[11]
   result |= (((imm & 0x7E) >> 1) << 21); // 10 bit - imm[10:1]
   result |= (((imm & 0x100000) >> 20) << 31); // 1 bit - imm[20]
}

}
namespace Simulators
{
namespace RV32I
{

void AssemblerUtils::generateAssemblyFileForMRCS(const ExecutableProgram& program, const std::string& fileName)
{
   int pc = 0;
   ushort instructionSize = 0;
   int binaryValue = 0;
   const Expressions::Instruction* instr = program.loadInstruction(pc, instructionSize);
   std::vector<std::bitset<32>> binaryRepresentedInstructions;

   SimulatorUtils::InstructionType type = SimulatorUtils::InstructionType::UNDEFINED;

   while(instr != nullptr)
   {
      const std::string& name = instr->getInstructionName();
      const std::vector<std::string>& operands = instr->getInstructionOperands();
      type = SimulatorUtils::getInstructionType(name);

      switch(type)
      {
      case SimulatorUtils::InstructionType::REGISTER:
      {
         int rd = Registry::getIntegerValue(operands[0]);
         int rs1 = Registry::getIntegerValue(operands[1]);
         int rs2 = Registry::getIntegerValue(operands[2]);
         int opcode = 0b0110011;
         int funct3 = getFunc3(name);
         int funct7 = ((name == "sub") || (name == "sra")) ? 0b0100000 : 0b0;

         machineFormatRegister(binaryValue, opcode, rd, rs1, rs2, funct3, funct7);
         break;
      }
      case SimulatorUtils::InstructionType::IMMEDIATE:
      {
         int rd = Registry::getIntegerValue(operands[0]);
         int rs = Registry::getIntegerValue(operands[1]);
         int imm = stoi(operands[2]);
         int opcode = 0b0010011;
         int funct3 = getFunc3(name);

         if((name == "slli") || (name == "srli")) // Use only the 5 LSB. The top 7 shall be 0 according to documentation
         {
            imm &= 0x1F;
         }
         else if(name == "srai") // Use only the 5 LSB. The top 7 shall be 0b0100000 according to documentation
         {
            imm &= 0x1f;
            imm |= (0b0100000 << 5);
         }

         machineFormatImmediate(binaryValue, opcode, rd, rs, imm, funct3);
         break;
      }
      case SimulatorUtils::InstructionType::UPPER:
      {
         int rd = Registry::getIntegerValue(operands[0]);
         int imm = stoi(operands[1]);
         int opcode = (name == "lui") ? 0b0110111 : 0b0010111;

         imm <<= 12; // Documentation states the Upper format takes bits [31:12] of the immediate, so we have to pad the immediate to put the relevant bits in the right place.

         machineFormatUpper(binaryValue, opcode, rd, imm);
         break;
      }
      case SimulatorUtils::InstructionType::BRANCH:
      {
         int rs1 = Registry::getIntegerValue(operands[0]);
         int rs2 = Registry::getIntegerValue(operands[1]);
         int offset = stoi(operands[2]);
         int opcode = 0b1100011;
         int funct3 = getFunc3(name);

         offset <<= 1; // Add back the lsb 0 that the assemblerandlinker shifts away

         machineFormatBranch(binaryValue, opcode, rs1, rs2, offset, funct3);
         break;
      }
      case SimulatorUtils::InstructionType::JUMP:
      {
         int rd = Registry::getIntegerValue(operands[0]);
         int offset = stoi(operands[1]);
         int opcode = 0b1101111;

         offset <<= 1; // Add back the lsb 0 that the assemblerandlinker shifts away

         machineFormatJump(binaryValue, opcode, rd, offset);
         break;
      }
      case SimulatorUtils::InstructionType::JUMP_REGISTER:
      {
         int rd = Registry::getIntegerValue(operands[0]);
         int rs1 = 0;
         int offset = 0;
         int opcode = 0b1100111;
         int funct3 = getFunc3(name);

         std::string off;
         std::string rs;
         ParseUtils::parseRegisterOffset(operands[1], off, rs);
         rs1 = Registry::getIntegerValue(rs);
         offset = stoi(off);

         machineFormatImmediate(binaryValue, opcode, rd, rs1, offset, funct3);
         break;
      }
      case SimulatorUtils::InstructionType::LOAD:
      {
         int rd = Registry::getIntegerValue(operands[0]);
         int rs1 = 0;
         int offset = 0;
         int opcode = 0b0000011;
         int funct3 = getFunc3(name);

         std::string off;
         std::string rs;
         ParseUtils::parseRegisterOffset(operands[1], off, rs);
         rs1 = Registry::getIntegerValue(rs);
         offset = stoi(off);

         machineFormatImmediate(binaryValue, opcode, rd, rs1, offset, funct3);
         break;
      }
      case SimulatorUtils::InstructionType::STORE:
      {
         int rs1 = 0;
         int rs2 = Registry::getIntegerValue(operands[0]);
         int offset = 0;
         int opcode = 0b0100011;
         int funct3 = getFunc3(name);

         std::string off;
         std::string rs;
         ParseUtils::parseRegisterOffset(operands[1], off, rs);
         rs1 = Registry::getIntegerValue(rs);
         offset = stoi(off);

         machineFormatStore(binaryValue, opcode, rs1, rs2, offset, funct3);
         break;
      }
      case SimulatorUtils::InstructionType::SYSTEM:
      {
         if(name == "ecall")
         {
            binaryValue = 0b1110011;
         }
         else if(name == "ebreak")
         {
            binaryValue = 0b100000000000001110011;
         }
         else
         {
            std::cerr << __PRETTY_FUNC__ << ": Unsupported system instruction: " << name << std::endl;
            abort();
         }
         break;
      }
      default:
         std::cerr << __PRETTY_FUNC__ << ": Unsupported instruction " << name << std::endl;
         break;
      }


      std::bitset<32> binaryRepresentation(binaryValue);
      /*std::cout << "Binary representation of ";
      instr->print();
      std::cout << " is \t" << binaryRepresentation.to_string() << std::endl;*/
      binaryRepresentedInstructions.push_back(binaryRepresentation);

      pc += instructionSize;
      instr = program.loadInstruction(pc, instructionSize);
   };

   std::ofstream file("BinaryInstructions.mbo");
   file << "isa: rv32i" << std::endl;
   for(const std::bitset<32>& binaryRepresentation: binaryRepresentedInstructions)
   {
      file << binaryRepresentation.to_string() << std::endl;
   }
   file.close();
   std::cout << "Finished writing binary represented instructions to BinaryInstructions.mbo" << std::endl;
}

}
}