#pragma once

#include <string>

namespace Simulators
{
namespace REBEL6
{

class SimulatorUtils
{
 public:
   enum class InstructionType {BRANCH, IMMEDIATE, JUMP, JUMP_REGISTER, LOAD, LOAD_IMMEDIATE, REGISTER, STORE, SYSTEM, UNDEFINED};

   SimulatorUtils() = delete;

   static InstructionType getInstructionType(const std::string& instructionName);

};

}
}
