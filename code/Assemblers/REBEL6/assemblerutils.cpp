#include "assemblerutils.h"

#include <unordered_map>

namespace
{
const std::unordered_map<std::string, const Assemblers::REBEL6::AssemblerUtils::InstructionType> InstructionMap =
   {
      // One could argue the binary ones should be retrieved from the RV32I code, but doing it this way illustrates
      // which RV32I instructions we intend to support
      //------------ BINARY ----------
      // R - Register
      {"add", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"sub", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"sll", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"srl", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"sra", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"sltu", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"or", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"xor", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"and", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},

      // I - Immediate
      {"addi", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      {"slli", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      {"srli", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      {"srai", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      {"sltiu", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      {"ori", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      {"xori", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      {"andi", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},

      // B - Branch
      {"bgeu", Assemblers::REBEL6::AssemblerUtils::InstructionType::BRANCH},
      {"bltu", Assemblers::REBEL6::AssemblerUtils::InstructionType::BRANCH},

      // I - Load
      {"lw", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD},
      {"lh", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD},
      {"lb", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD},
      {"lhu", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD},
      {"lbu", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD},

      // S - Store
      {"sw", Assemblers::REBEL6::AssemblerUtils::InstructionType::STORE},
      {"sh", Assemblers::REBEL6::AssemblerUtils::InstructionType::STORE},
      {"sb", Assemblers::REBEL6::AssemblerUtils::InstructionType::STORE},

      // SYSTEM
      {"ecall", Assemblers::REBEL6::AssemblerUtils::InstructionType::SYSTEM},
      // {"ebreak", Assemblers::REBEL6::AssemblerUtils::InstructionType::SYSTEM},


      //------------ TERNARY ----------
      // R - Register
      // {"add.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      // {"sub.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      // {"sl.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      // {"sr.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      {"slt.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      // {"or.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      // {"xor.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},
      // {"and.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::REGISTER},

      // I - Immediate
      {"addi.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      // {"sli.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      // {"sri.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      {"slti.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      // {"ori.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      // {"xori.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},
      // {"andi.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::IMMEDIATE},

      // LI - Load Immediate
      {"li.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD_IMMEDIATE},
      {"aipc.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD_IMMEDIATE},

      // B - Branch
      {"beq.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::BRANCH},
      {"bne.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::BRANCH},
      {"blt.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::BRANCH},
      {"bge.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::BRANCH},

      // J - Jump
      {"jal.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::JUMP},

      // I - Jump Register
      {"jalr.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::JUMP_REGISTER},

      // I - Load
      // {"lw.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD},
      // {"lh.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD},
      // {"lt.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::LOAD},

      // S - Store
      // {"sw.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::STORE},
      // {"sh.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::STORE},
      // {"st.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::STORE},

      // SYSTEM
      // {"ecall.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::SYSTEM},
      // {"ebreak.t", Assemblers::REBEL6::AssemblerUtils::InstructionType::SYSTEM}
   };
}

namespace Assemblers
{
namespace REBEL6
{
AssemblerUtils::InstructionType AssemblerUtils::getInstructionType(const std::string& instructionName)
{
   InstructionType retVal;

   try
   {
      retVal = InstructionMap.at(instructionName);
   }
   catch(std::exception&)
   {
      retVal = InstructionType::UNDEFINED;
   }

   return retVal;
}

}
}
