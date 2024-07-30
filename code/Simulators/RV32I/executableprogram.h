#ifndef EXECUTABLEPROGRAM_H
#define EXECUTABLEPROGRAM_H

#include <vector>
#include <map>
#include <string>

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

   void addInstruction(Expressions::Instruction* instruction, uint instructionSize);
   void addToHeap(int value, ushort numBytes);
   void addSymbol(const std::string& symbolName, int value);

   Expressions::Instruction* loadInstruction(int programCounter, ushort& instructionSize) const;
   int loadFromHeap(int address, ushort numBytes) const;
   int loadSymbolValue(const std::string& symbolName) const;

   void storeToHeap(int address, int value, ushort numBytes);

   // Should be called after all instructions and initial heap values have been added, to recalculate heap size.
   void calculateHeapSize();

   int getProgramSize() const;
   int getInstructionsSize();

   void printInstructions() const;
   void printSymbols() const;

private:
   void doStoreToHeap(int index, int value, ushort numBytes);

   using InstructionMemoryMap = std::map<int, Expressions::Instruction*>; // Address | Instruction
   using SymbolTableMap = std::map<std::string, int>; // Label name | Start address of value
   using HeapVector = std::vector<unsigned char>;

   InstructionMemoryMap m_instructions; // Address, Instruction
   int m_instructionsSize;
   int m_heapSize;

   // Might have to use SymbolValues or char class since the size of each negative binary value is relevant for number of bitflips.
   HeapVector m_heap; // Starts at m_instructionsSize;
   SymbolTableMap m_symbolTable;
};

}
}

#endif // EXECUTABLEPROGRAM_H
