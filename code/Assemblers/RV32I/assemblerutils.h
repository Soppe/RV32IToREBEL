#pragma once

#include <string>

namespace Assemblers
{
namespace RV32I
{

class ExecutableProgram;

class AssemblerUtils
{
public:
   enum class InstructionType {BRANCH, IMMEDIATE, JUMP, JUMP_REGISTER, LOAD, REGISTER, STORE, UPPER, SYSTEM, UNDEFINED};


   AssemblerUtils() = delete;

   static InstructionType getInstructionType(const std::string& instructionName);
   static void generateAssemblyFileForMRCS(const ExecutableProgram& program, const std::string& fileName);
};

}
}