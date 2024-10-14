#pragma once

#include <string>

namespace Assemblers
{
namespace REBEL6
{

class AssemblerUtils
{
 public:
   enum class InstructionType {BRANCH, IMMEDIATE, JUMP, JUMP_REGISTER, LOAD, LOAD_IMMEDIATE, REGISTER, STORE, SYSTEM, UNDEFINED};

   AssemblerUtils() = delete;

   static InstructionType getInstructionType(const std::string& instructionName);

};

}
}
