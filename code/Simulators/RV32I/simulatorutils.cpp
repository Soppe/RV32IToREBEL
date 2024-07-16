#include "simulatorutils.h"

#include "executableprogram.h"
#include "Cpus/registry.h"

#include <logger.h>
#include <Expressions/instruction.h>

#include <unordered_map>
#include <iostream>

namespace
{
const std::unordered_map<std::string, const Simulators::RV32I::SimulatorUtils::InstructionType> InstructionMap =
    {
        {
            // R - Register
            {"add", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"sub", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"sll", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"srl", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"sra", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"slt", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"sltu", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"or", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"xor", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},
            {"and", Simulators::RV32I::SimulatorUtils::InstructionType::REGISTER},

            // I - Immediate
            {"addi", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},
            {"slli", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},
            {"srli", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},
            {"srai", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},
            {"slti", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},
            {"sltiu", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},
            {"ori", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},
            {"xori", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},
            {"andi", Simulators::RV32I::SimulatorUtils::InstructionType::IMMEDIATE},

            // U - Upper
            {"lui", Simulators::RV32I::SimulatorUtils::InstructionType::UPPER},
            {"auipc", Simulators::RV32I::SimulatorUtils::InstructionType::UPPER},

            // B - Branch
            {"beq", Simulators::RV32I::SimulatorUtils::InstructionType::BRANCH},
            {"bne", Simulators::RV32I::SimulatorUtils::InstructionType::BRANCH},
            {"blt", Simulators::RV32I::SimulatorUtils::InstructionType::BRANCH},
            {"bltu", Simulators::RV32I::SimulatorUtils::InstructionType::BRANCH},
            {"bge", Simulators::RV32I::SimulatorUtils::InstructionType::BRANCH},
            {"bgeu", Simulators::RV32I::SimulatorUtils::InstructionType::BRANCH},

            // J - Jump
            {"jal", Simulators::RV32I::SimulatorUtils::InstructionType::JUMP},

            // I - Jump Register
            {"jalr", Simulators::RV32I::SimulatorUtils::InstructionType::JUMP_REGISTER},

            // I - Load
            {"lw", Simulators::RV32I::SimulatorUtils::InstructionType::LOAD},
            {"lh", Simulators::RV32I::SimulatorUtils::InstructionType::LOAD},
            {"lb", Simulators::RV32I::SimulatorUtils::InstructionType::LOAD},
            {"lhu", Simulators::RV32I::SimulatorUtils::InstructionType::LOAD},
            {"lbu", Simulators::RV32I::SimulatorUtils::InstructionType::LOAD},

            // S - Store
            {"sw", Simulators::RV32I::SimulatorUtils::InstructionType::STORE},
            {"sh", Simulators::RV32I::SimulatorUtils::InstructionType::STORE},
            {"sb", Simulators::RV32I::SimulatorUtils::InstructionType::STORE},

            // SYSTEM
            {"ecall", Simulators::RV32I::SimulatorUtils::InstructionType::SYSTEM},
            {"ebreak", Simulators::RV32I::SimulatorUtils::InstructionType::SYSTEM}
        }
};

}

namespace Simulators
{
namespace RV32I
{
SimulatorUtils::InstructionType SimulatorUtils::getInstructionType(const std::string& instructionName)
{
   SimulatorUtils::InstructionType retVal;

   try
   {
      retVal = InstructionMap.at(instructionName);
   }
   catch(std::exception&)
   {
      retVal = SimulatorUtils::InstructionType::UNDEFINED;
   }

   return retVal;
}

void SimulatorUtils::generateAssemblyFileForMRCS(const ExecutableProgram& program, const std::string& fileName)
{
   int pc = 0;
   int instructionSize = 0;
   int binaryRepresentation = 0;
   const Expressions::Instruction* instr = program.loadInstruction(pc, instructionSize);

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

         std::cout << "rd = " << operands[0] << " = " << rd << std::endl;
         std::cout << "rs1 = " << operands[1] << " = " << rs1 << std::endl;
         std::cout << "rs2 = " << operands[2] << " = " << rs2 << std::endl;
         break;
      }
      case SimulatorUtils::InstructionType::IMMEDIATE:
         break;
      case SimulatorUtils::InstructionType::UPPER:
         break;
      case SimulatorUtils::InstructionType::BRANCH:
         break;
      case SimulatorUtils::InstructionType::JUMP:
         break;
      case SimulatorUtils::InstructionType::JUMP_REGISTER:
         break;
      case SimulatorUtils::InstructionType::LOAD:
         break;
      case SimulatorUtils::InstructionType::STORE:
         break;
      case SimulatorUtils::InstructionType::SYSTEM:
      {
         std::string systemInstructionName;
         systemInstructionName.resize(name.length());
         std::transform(name.begin(), name.end(), systemInstructionName.begin(), [](unsigned char c){ return std::tolower(c); });
         if(systemInstructionName == "fence")
         {
            // This is only the opcode. FENCE consists of multiple other fields, but since we don't support fence in the first place we won't add any more data
            binaryRepresentation = 0b1111;
         }
         else if(systemInstructionName == "ecall")
         {
            binaryRepresentation = 0b1110011;
         }
         else if(systemInstructionName == "ebreak")
         {
            binaryRepresentation = 0b100000000000001110011;
         }
         else
         {
            std::cerr << __PRETTY_FUNC__ << ": Unsupported system instruction: " << name << std::endl;
         }
         break;
      }
      default:
         std::cerr << __PRETTY_FUNC__ << ": Unsupported instruction " << name << std::endl;
      }

      /*
      std::cout << "Binary representation of ";
      instr->print();
      std::cout << " is " << binaryRepresentation << std::endl;*/

      pc += instructionSize;
      instr = program.loadInstruction(pc, instructionSize);
   };
}

}
}
