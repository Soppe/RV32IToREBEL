#include "executableprogram.h"

#include <Expressions/instruction.h>
#include <logger.h>

#include <iostream>

namespace
{
const int PROGRAM_SIZE = 1024 * 1024; // 1 MB
}

namespace Simulators
{
namespace RV32I
{

ExecutableProgram::ExecutableProgram()
    : m_instructionsSize(0)
    , m_heapSize(0)
{

}

ExecutableProgram::~ExecutableProgram()
{
   InstructionMemoryMap::iterator iter = m_instructions.begin();
   for(; iter != m_instructions.end(); )
   {
      if (iter->second != nullptr)
      {
         delete iter->second;
         iter->second == nullptr;
      }
      iter = m_instructions.erase(iter);
   }
}

void ExecutableProgram::addInstruction(Expressions::Instruction* instruction, uint instructionSize)
{
   m_instructions.insert({m_instructionsSize, instruction});
   m_instructionsSize += instructionSize;
}

void ExecutableProgram::addToHeap(int value, ushort numBytes)
{
   if((numBytes > 4) || (numBytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to store data of unsupported size = " << numBytes << std::endl;
      abort();
   }

   int heapSize = m_heap.size();
   m_heap.resize(heapSize + numBytes, 0);

   doStoreToHeap(heapSize, value, numBytes);
}

void ExecutableProgram::addSymbol(const std::string& symbolName, int value)
{
   m_symbolTable.insert({symbolName, value});
}

Expressions::Instruction* ExecutableProgram::loadInstruction(int programCounter, ushort& instructionSize) const
{
   Expressions::Instruction* instr = nullptr;
   InstructionMemoryMap::const_iterator it = m_instructions.find(programCounter);
   if(it != m_instructions.end())
   {
      instr = it->second;
      ++it;
      if(it == m_instructions.end())
      {
         instructionSize = m_instructionsSize - programCounter;
      }
      else
      {
         instructionSize = it->first - programCounter;
      }
   }
   return instr;
}

int ExecutableProgram::loadFromHeap(int address, ushort numBytes) const
{
   int retVal = 0;
   if((numBytes > 4) || (numBytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to load data of unsupported byte size " << numBytes << std::endl;
      abort();
   }

   int shiftCounter = 0;
   int index = address - m_instructionsSize;
   for(ushort i = 0; i < numBytes; ++i)
   {
      retVal = retVal | (m_heap[index] << shiftCounter);
      std::cout << "Loading from memory = " << static_cast<int>(m_heap[index]) << " at index " << index << std::endl;
      ++index;
      shiftCounter += 8;
   }

   return retVal;
}

int ExecutableProgram::loadSymbolValue(const std::string& symbolName) const
{
   int retVal = -1;
   try
   {
      retVal = m_symbolTable.at(symbolName);
   }
   catch(std::exception&)
   {
      throw;
   }

   return retVal;
}

void ExecutableProgram::storeToHeap(int address, int value, ushort numBytes)
{
   if((numBytes > 4) || (numBytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to store data of unsupported byte size " << numBytes << std::endl;
      abort();
   }

   int index = address - getInstructionsSize();

   doStoreToHeap(index, value, numBytes);

}

void ExecutableProgram::calculateHeapSize()
{
   m_heapSize = getProgramSize() - getInstructionsSize();
   if((m_heapSize < 0 ) || (m_heap.size() > m_heapSize))
   {
      std::cerr << __PRETTY_FUNC__ << ": Tried recalcuating heap, but heap already overflowing" << std::endl;
      abort();
   }

   m_heap.resize(m_heapSize, 0);
}

int ExecutableProgram::getProgramSize() const
{
   return PROGRAM_SIZE;
}

int ExecutableProgram::getInstructionsSize()
{
   return m_instructionsSize;
}

void ExecutableProgram::printInstructions() const
{
   int pc = 0;
   InstructionMemoryMap::const_iterator it = m_instructions.begin();
   while(it != m_instructions.end())
   {

      std::cout << std::hex << pc << ": ";
      it->second->print();
      std::cout << std::endl;

      pc += it->first;
      ++it;
   }
}

void ExecutableProgram::printSymbols() const
{
   for(const auto& [key, value]: m_symbolTable)
   {
      std::cout << key << ": " << value << std::endl;
   }
}

void ExecutableProgram::doStoreToHeap(int index, int value, ushort numBytes)
{
   std::cout << "Initial value = " << std::hex << value << std::dec << std::endl;
   for(ushort i = 0; i < numBytes; ++i)
   {
      m_heap[index + i] = (value & 0xff);
      std::cout << "Storing value to heap = " << std::hex << (value & 0xff) << std::dec << " at index " << (index + i) << std::endl;
      value = value >> 8;
   }
   std::cout << std::endl;
}

}
}
