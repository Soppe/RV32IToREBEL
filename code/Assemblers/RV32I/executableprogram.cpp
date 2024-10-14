#include "executableprogram.h"

#include <Expressions/instruction.h>
#include <logger.h>

#include <iostream>

namespace
{
const std::uint32_t PROGRAM_SIZE_BYTES = 1024 * 1024; // 1 MB
}

namespace Assemblers
{
namespace RV32I
{

ExecutableProgram::ExecutableProgram()
    : m_instructionsSizeBytes(0)
    , m_heapSizeBytes(0)
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

void ExecutableProgram::addInstruction(Expressions::Instruction* instruction, std::uint8_t instructionSizeBytes)
{
   m_instructions.insert({m_instructionsSizeBytes, instruction});
   m_instructionsSizeBytes += instructionSizeBytes;
}

void ExecutableProgram::addToHeap(std::int32_t value, std::uint8_t numBytes)
{
   if((numBytes > 4) || (numBytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to store data of unsupported size = " << numBytes << " bytes" << std::endl;
      abort();
   }

   std::uint32_t heapSizeBytes = m_heap.size();
   m_heap.resize(heapSizeBytes + numBytes, 0);

   doStoreToHeap(heapSizeBytes, value, numBytes);
}

void ExecutableProgram::addSymbol(const std::string& symbolName, std::uint32_t address)
{
   m_symbolTable.insert({symbolName, address});
}

Expressions::Instruction* ExecutableProgram::loadInstruction(std::uint32_t programCounter, std::uint8_t& instructionSizeBytes) const
{
   Expressions::Instruction* instr = nullptr;
   InstructionMemoryMap::const_iterator it = m_instructions.find(programCounter);
   if(it != m_instructions.end())
   {
      instr = it->second;
      ++it;
      if(it == m_instructions.end())
      {
         instructionSizeBytes = m_instructionsSizeBytes - programCounter;
      }
      else
      {
         instructionSizeBytes = it->first - programCounter;
      }
   }
   return instr;
}

std::int32_t ExecutableProgram::loadFromHeap(std::uint32_t address, std::uint8_t numBytes) const
{
   std::int32_t retVal = 0;
   if((numBytes > 4) || (numBytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to load data of unsupported byte size " << numBytes << std::endl;
      abort();
   }

   std::uint8_t shiftCounter = 0;
   std::uint32_t index = address - m_instructionsSizeBytes;
   for(std::uint8_t i = 0; i < numBytes; ++i, ++index)
   {
      retVal = retVal | (m_heap[index] << shiftCounter);
      // std::cout << "Loading from memory = " << (int)m_heap[index] << " at index " << index << std::endl;
      shiftCounter += 8;
   }

   return retVal;
}

std::uint32_t ExecutableProgram::loadSymbolAddress(const std::string& symbolName) const
{
   std::uint32_t retVal = 0;
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

void ExecutableProgram::storeToHeap(std::uint32_t address, std::int32_t value, std::uint8_t numBytes)
{
   if((numBytes > 4) || (numBytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to store data of unsupported byte size " << numBytes << std::endl;
      abort();
   }

   std::uint32_t heapAddress = address - getInstructionsSizeBytes();

   doStoreToHeap(heapAddress, value, numBytes);

}

void ExecutableProgram::calculateHeapSize()
{
   m_heapSizeBytes = getProgramSizeBytes() - getInstructionsSizeBytes();
   if((m_heapSizeBytes < 0 ) || (m_heap.size() > m_heapSizeBytes))
   {
      std::cerr << __PRETTY_FUNC__ << ": Tried recalcuating heap size, but heap already overflowing" << std::endl;
      abort();
   }

   m_heap.resize(m_heapSizeBytes, 0);

   std::cout << "Program size: " << getProgramSizeBytes() << " bytes; Instructions size: " << getInstructionsSizeBytes() << " bytes; Heap size: " << m_heap.size() << " bytes" << std::endl;
}

std::uint32_t ExecutableProgram::getProgramSizeBytes() const
{
   return PROGRAM_SIZE_BYTES;
}

std::uint32_t ExecutableProgram::getInstructionsSizeBytes() const
{
   return m_instructionsSizeBytes;
}

void ExecutableProgram::printInstructions() const
{
   std::uint32_t pc = 0;
   InstructionMemoryMap::const_iterator it = m_instructions.begin();
   while(it != m_instructions.end())
   {

      std::cout << std::hex << pc << ": " << *(it->second) << std::endl;

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

void ExecutableProgram::doStoreToHeap(std::uint32_t heapAddress, std::int32_t value, std::uint8_t numBytes)
{
   for(std::uint8_t i = 0; i < numBytes; ++i)
   {
      m_heap[heapAddress] = (value & 0xff);
      value = value >> 8;
      ++heapAddress;
   }
}

}
}
