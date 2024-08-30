#include "simulatorutils.h"

#include <unordered_map>

namespace
{
const std::unordered_map<std::string, const Simulators::REBEL6::SimulatorUtils::InstructionType> InstructionMap =
    {
       // One could argue the binary ones should be retrieved from the RV32I code, but doing it this way illustrates
       // which RV32I instructions we intend to support
       //------------ BINARY ----------
       // R - Register
       {"add", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"sub", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"sll", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"srl", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"sra", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"or", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"xor", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"and", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},

       // I - Immediate
       {"addi", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"slli", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"srli", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"srai", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"ori", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"xori", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"andi", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},

       // I - Load
       {"lw", Simulators::REBEL6::SimulatorUtils::InstructionType::LOAD},
       {"lh", Simulators::REBEL6::SimulatorUtils::InstructionType::LOAD},
       {"lb", Simulators::REBEL6::SimulatorUtils::InstructionType::LOAD},
       {"lhu", Simulators::REBEL6::SimulatorUtils::InstructionType::LOAD},
       {"lbu", Simulators::REBEL6::SimulatorUtils::InstructionType::LOAD},

       // S - Store
       {"sw", Simulators::REBEL6::SimulatorUtils::InstructionType::STORE},
       {"sh", Simulators::REBEL6::SimulatorUtils::InstructionType::STORE},
       {"sb", Simulators::REBEL6::SimulatorUtils::InstructionType::STORE},

       // SYSTEM
       {"ecall", Simulators::REBEL6::SimulatorUtils::InstructionType::SYSTEM},
       {"ebreak", Simulators::REBEL6::SimulatorUtils::InstructionType::SYSTEM},


       //------------ TERNARY ----------
       // R - Register
       {"add.t", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"sub.t", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"sl.t", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"sr.t", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"slt.t", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"or.t", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"xor.t", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},
       {"and.t", Simulators::REBEL6::SimulatorUtils::InstructionType::REGISTER},

       // I - Immediate
       {"addi.t", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"sli.t", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"sri.t", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"slti.t", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"ori.t", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"xori.t", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},
       {"andi.t", Simulators::REBEL6::SimulatorUtils::InstructionType::IMMEDIATE},

       // B - Branch
       {"beq.t", Simulators::REBEL6::SimulatorUtils::InstructionType::BRANCH},
       {"bne.t", Simulators::REBEL6::SimulatorUtils::InstructionType::BRANCH},
       {"blt.t", Simulators::REBEL6::SimulatorUtils::InstructionType::BRANCH},
       {"bge.t", Simulators::REBEL6::SimulatorUtils::InstructionType::BRANCH},

       // J - Jump
       {"jal.t", Simulators::REBEL6::SimulatorUtils::InstructionType::JUMP},

       // I - Jump Register
       {"jalr.t", Simulators::REBEL6::SimulatorUtils::InstructionType::JUMP_REGISTER},

       // I - Load
       {"lw.t", Simulators::REBEL6::SimulatorUtils::InstructionType::LOAD},
       {"lh.t", Simulators::REBEL6::SimulatorUtils::InstructionType::LOAD},
       {"lb.t", Simulators::REBEL6::SimulatorUtils::InstructionType::LOAD},

       // S - Store
       {"sw.t", Simulators::REBEL6::SimulatorUtils::InstructionType::STORE},
       {"sh.t", Simulators::REBEL6::SimulatorUtils::InstructionType::STORE},
       {"sb.t", Simulators::REBEL6::SimulatorUtils::InstructionType::STORE},

       // SYSTEM
       {"ecall.t", Simulators::REBEL6::SimulatorUtils::InstructionType::SYSTEM},
       {"ebreak.t", Simulators::REBEL6::SimulatorUtils::InstructionType::SYSTEM}
    };
}

namespace Simulators
{
namespace REBEL6
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
