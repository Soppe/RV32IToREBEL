#include "executableprogram.h"

#include <Expressions/instruction.h>
#include <logger.h>

#include <iostream>
#include <math.h>

namespace
{
const std::uint32_t PROGRAM_SIZE_TRITS = 1000000 * TRITS_PER_TRYTE;
}

namespace Simulators
{
namespace REBEL6
{

ExecutableProgram::ExecutableProgram()
    : m_instructionsSizeTrits(0)
    , m_heapSizeTrits(0)
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

void ExecutableProgram::addInstruction(Expressions::Instruction* instruction, std::uint8_t instructionSizeTrits)
{
   m_instructions.insert({m_instructionsSizeTrits, instruction});
   m_instructionsSizeTrits += instructionSizeTrits;
}

void ExecutableProgram::addToHeap(const Tint& value, uint8_t numTrits)
{
   if((numTrits > TRITS_PER_TWORD) || (numTrits < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to store data of unsupported size = " << numTrits << " trits" << std::endl;
      abort();
   }

   std::uint32_t heapSizeTrits = m_heap.size();
   m_heap.resize(heapSizeTrits + numTrits, 0);

   doStoreToHeap(heapSizeTrits, value, numTrits);
}

void ExecutableProgram::addSymbol(const std::string& symbolName, std::uint32_t address)
{
   m_symbolTable.insert({symbolName, address});
}

Expressions::Instruction* ExecutableProgram::loadInstruction(std::uint32_t programCounter, std::uint8_t& instructionSizeTrits) const
{
   Expressions::Instruction* instr = nullptr;
   InstructionMemoryMap::const_iterator it = m_instructions.find(programCounter);
   if(it != m_instructions.end())
   {
      instr = it->second;
      ++it;
      if(it == m_instructions.end())
      {
         instructionSizeTrits = m_instructionsSizeTrits - programCounter;
      }
      else
      {
         instructionSizeTrits = it->first - programCounter;
      }
   }
   return instr;
}

Tint ExecutableProgram::loadFromHeap(std::uint32_t address, std::uint8_t numTrits) const
{
   Tint retVal = 0;
   if((numTrits > TRITS_PER_TWORD) || (numTrits < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to load data of unsupported trit size " << numTrits << std::endl;
      abort();
   }

   std::uint32_t index = address - m_instructionsSizeTrits;
   for(std::uint8_t i = 0; i < numTrits; ++i, ++index)
   {
      if(m_heap[index] != 0) // Only care when the trit is either 1 or -1
      {
         retVal += m_heap[index] * std::pow(3, i);
      }
      std::cout << "Loading from memory = " << m_heap[index] << " at index " << index << std::endl;
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

void ExecutableProgram::storeToHeap(std::uint32_t address, const Tint& value, std::uint8_t numTrits)
{
   if((numTrits > TRITS_PER_TWORD) || (numTrits < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to store data of unsupported trit size " << numTrits << std::endl;
      abort();
   }

   std::uint32_t index = address - getInstructionsSizeTrits();

   doStoreToHeap(index, value, numTrits);
}

void ExecutableProgram::calculateHeapSize()
{
   m_heapSizeTrits = getProgramSizeTrits() - getInstructionsSizeTrits();
   if((m_heapSizeTrits < 0 ) || (m_heap.size() > m_heapSizeTrits))
   {
      std::cerr << __PRETTY_FUNC__ << ": Tried recalcuating heap size, but heap already overflowing" << std::endl;
      abort();
   }

   m_heap.resize(m_heapSizeTrits, 0);

   std::cout << "Heap size calculated to be " << m_heap.size() << " trits" << std::endl;
}

uint32_t ExecutableProgram::getProgramSizeTrits() const
{
   return PROGRAM_SIZE_TRITS;
}

uint32_t ExecutableProgram::getInstructionsSizeTrits() const
{
   return m_instructionsSizeTrits;
}

void ExecutableProgram::printInstructions() const
{
   std::uint32_t pc = 0;
   InstructionMemoryMap::const_iterator it = m_instructions.begin();
   while(it != m_instructions.end())
   {

      std::cout << pc << ": " << *(it->second) << std::endl;

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

void ExecutableProgram::doStoreToHeap(std::uint32_t index, const Tint& value, std::uint8_t numTrits)
{
   std::vector<Trit> trits(numTrits, 0);

   // TODO: TernaryLogic::DecimalToTrits(trits, value); // Could send m_heap directly, but lets keep it safe

   std::cout << "Initial value = " << std::hex << value << std::dec << std::endl;
   for(std::uint8_t i = 0; i < numTrits; ++i, ++index)
   {
      m_heap[index] = trits[i];
      std::cout << "Storing ternary value to heap = " << trits[i] << " at index " << index << std::endl;
   }
   std::cout << std::endl;
}

}
}
