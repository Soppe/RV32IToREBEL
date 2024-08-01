#pragma once

#include "ternarylogic.h"

#include <string>
#include <map>
#include <vector>

namespace Expressions
{
class Instruction;
}

namespace Simulators
{
namespace REBEL6
{
class ExecutableProgram
{
public:
   ExecutableProgram();
   ~ExecutableProgram();

   void addInstruction(Expressions::Instruction* instruction, std::uint8_t instructionSizeTrits);
   void addToHeap(const Tint& value, std::uint8_t numTrits);
   void addSymbol(const std::string& symbolName, std::uint32_t address);

   Expressions::Instruction* loadInstruction(std::uint32_t programCounter, std::uint8_t& instructionSizeTrits) const;
   Tint loadFromHeap(std::uint32_t address, std::uint8_t numTrits) const;
   std::uint32_t loadSymbolAddress(const std::string& symbolName) const; // Can throw exception

   void storeToHeap(std::uint32_t address, const Tint& value, std::uint8_t numTrits);

   // Must be called after all instructions and initial heap values have been added, to recalculate heap size.
   void calculateHeapSize();

   std::uint32_t getProgramSizeTrits() const;
   std::uint32_t getInstructionsSizeTrits() const;

   void printInstructions() const;
   void printSymbols() const;

private:
   void doStoreToHeap(std::uint32_t index, const Tint& value, std::uint8_t numTrits);

   using InstructionMemoryMap = std::map<std::int32_t, Expressions::Instruction*>; // Address | Instruction
   using SymbolTableMap = std::map<std::string, std::int32_t>; // Label name | Start address of value
   using HeapVector = std::vector<Trit>; // Ineffective, but the simplest way to store any size ternary value

   InstructionMemoryMap m_instructions; // Address, Instruction
   std::uint32_t m_instructionsSizeTrits;
   std::uint32_t m_heapSizeTrits;

   HeapVector m_heap; // Starts at m_instructionsSize;
   SymbolTableMap m_symbolTable;
};
}
}
