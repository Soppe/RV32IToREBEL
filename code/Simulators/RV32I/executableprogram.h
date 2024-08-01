#ifndef EXECUTABLEPROGRAM_H
#define EXECUTABLEPROGRAM_H

#include <string>
#include <map>
#include <vector>

namespace Expressions
{
class Instruction;
}

namespace Simulators
{
namespace RV32I
{
class ExecutableProgram
{
public:
   ExecutableProgram();
   ~ExecutableProgram();

   void addInstruction(Expressions::Instruction* instruction, std::uint8_t instructionSizeBytes);
   void addToHeap(std::int32_t value, std::uint8_t numBytes);
   void addSymbol(const std::string& symbolName, std::uint32_t address);

   Expressions::Instruction* loadInstruction(std::uint32_t programCounter, std::uint8_t& instructionSizeBytes) const;
   std::int32_t loadFromHeap(std::uint32_t address, std::uint8_t numBytes) const;
   std::uint32_t loadSymbolAddress(const std::string& symbolName) const; // Can throw exception

   void storeToHeap(std::uint32_t address, std::int32_t value, std::uint8_t numBytes);

   // Must be called after all instructions and initial heap values have been added, to recalculate heap size.
   void calculateHeapSize();

   std::uint32_t getProgramSizeBytes() const;
   std::uint32_t getInstructionsSizeBytes() const;

   void printInstructions() const;
   void printSymbols() const;

private:
   void doStoreToHeap(std::uint32_t index, std::int32_t value, std::uint8_t numBytes);

   using InstructionMemoryMap = std::map<std::int32_t, Expressions::Instruction*>; // Address | Instruction
   using SymbolTableMap = std::map<std::string, std::uint32_t>; // Label name | Start address of value
   using HeapVector = std::vector<std::uint8_t>;

   InstructionMemoryMap m_instructions; // Address, Instruction
   std::uint32_t m_instructionsSizeBytes;
   std::uint32_t m_heapSizeBytes;

   HeapVector m_heap; // Starts at m_instructionsSize;
   SymbolTableMap m_symbolTable;
};

}
}

#endif // EXECUTABLEPROGRAM_H
