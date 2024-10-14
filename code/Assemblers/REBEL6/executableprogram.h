#pragma once

#include <Parsers/ternarylogic.h>

#include <string>
#include <map>
#include <vector>

namespace Expressions
{
class Instruction;
}

namespace Assemblers
{
namespace REBEL6
{
class ExecutableProgram
{
public:
   ExecutableProgram();
   ~ExecutableProgram();

   void addInstruction(Expressions::Instruction* instruction, std::uint8_t instructionSize);
   void addToHeap(const Tint& value, std::uint8_t numTrytes);
   void addSymbol(const std::string& symbolName, std::int32_t address);

   Expressions::Instruction* loadInstruction(std::int32_t programCounter, std::uint8_t& instructionSize) const;
   void loadFromHeap(std::int32_t address, std::uint8_t numTrytes, Trytes& out) const;
   std::int32_t loadSymbolAddress(const std::string& symbolName) const; // Can throw exception

   void storeToHeap(std::int32_t address, const Tint& value, std::uint8_t numTrytes);

   // Must be called after all instructions and initial heap values have been added, to recalculate heap size.
   void calculateHeapSize();

   std::uint32_t getProgramSizeTrytes() const;
   std::uint32_t getInstructionsSizeTrytes() const;

   void printInstructions() const;
   void printSymbols() const;

private:
   void doStoreToHeap(std::uint32_t heapAddress, const Tint& value, std::uint8_t numTrytes);

   using InstructionMemoryMap = std::map<std::int32_t, Expressions::Instruction*>; // Address | Instruction
   using SymbolTableMap = std::map<std::string, std::int32_t>; // Label name | Start address of value
   using HeapVector = Trytes;

   InstructionMemoryMap m_instructions; // Address, Instruction
   std::uint32_t m_instructionsSize;
   std::uint32_t m_heapSizeTrytes;

   HeapVector m_heap; // Starts at m_instructionsSize;
   SymbolTableMap m_symbolTable;
};
}
}
