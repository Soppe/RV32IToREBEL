#pragma once

#include <string>


namespace Simulators
{
namespace RV32I
{

class SimulatorUtils
{
public:
   enum class InstructionType {BRANCH, IMMEDIATE, JUMP, JUMP_REGISTER, LOAD, REGISTER, STORE, UPPER, SYSTEM, UNDEFINED};

   SimulatorUtils() = delete;

   static InstructionType getInstructionType(const std::string& instructionName);

};

}
}
