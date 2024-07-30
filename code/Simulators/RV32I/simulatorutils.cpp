#include "simulatorutils.h"

#include "executableprogram.h"
#include "Cpu/registry.h"

#include <logger.h>
#include <Expressions/instruction.h>
#include <Parsers/parseutils.h>

#include <unordered_map>
#include <iostream>

namespace
{
const std::unordered_map<std::string, const Simulators::RV32I::SimulatorUtils::InstructionType> InstructionMap =
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

}
}
