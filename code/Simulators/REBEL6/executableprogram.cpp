#include "executableprogram.h"

#include <Converters/rv32itorebel6.h>
#include <Expressions/instruction.h>
#include <logger.h>

#include <iostream>
#include <math.h>

namespace
{
const std::uint32_t PROGRAM_SIZE_TRYTES = std::pow(3, 13); // About 1.6m trytes
}

namespace Simulators
{
namespace REBEL6
{

ExecutableProgram::ExecutableProgram()
    : m_instructionsSize(0)
    , m_heapSizeTrytes(0)
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

void ExecutableProgram::addInstruction(Expressions::Instruction* instruction, std::uint8_t instructionSize)
{
   m_instructions.insert({m_instructionsSize, instruction});
   m_instructionsSize += instructionSize;
}

void ExecutableProgram::addToHeap(const Tint& value, uint8_t numTrytes)
{
   if((numTrytes > 4) || (numTrytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to store data of unsupported size = " << numTrytes << " trits" << std::endl;
      abort();
   }

   std::uint32_t heapSizeTrytes = m_heap.size();
   m_heap.resize(heapSizeTrytes + numTrytes, 0);

   doStoreToHeap(heapSizeTrytes, value, numTrytes);
}

void ExecutableProgram::addSymbol(const std::string& symbolName, std::int32_t address)
{
   m_symbolTable.insert({symbolName, address});
}

Expressions::Instruction* ExecutableProgram::loadInstruction(std::int32_t programCounter, std::uint8_t& instructionSize) const
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

Tint ExecutableProgram::loadFromHeap(std::int32_t address, std::uint8_t numTrytes) const
{
   Tint retVal = 0;
   if((numTrytes > 4) || (numTrytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to load data of unsupported tryte size " << numTrytes << std::endl;
      abort();
   }

   std::uint32_t heapAddress = address - getInstructionsSizeTrytes() /* +/- program start address if it's other than 0 */;
   Trits trits;
   for(std::uint8_t i = 0; i < numTrytes; ++i, ++heapAddress)
   {
      trits.clear();
      TernaryLogic::TintToTrits(m_heap[heapAddress], trits);
      for(std::uint8_t j = 0; j < trits.size(); ++j)
      {
         retVal += trits[j] * std::pow(3, j + (i * REBEL6_TRITS_PER_TRYTE));
      }
      std::cout << "Loading from memory = " << m_heap[heapAddress] << " at index " << heapAddress << std::endl;
   }

   return retVal;
}

std::int32_t ExecutableProgram::loadSymbolAddress(const std::string& symbolName) const
{
   std::int32_t retVal = 0;
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

void ExecutableProgram::storeToHeap(std::int32_t address, const Tint& value, std::uint8_t numTrytes)
{
   if((numTrytes > 4) || (numTrytes < 0))
   {
      std::cerr << __PRETTY_FUNC__ << ": Trying to store data of unsupported tryte size " << numTrytes << std::endl;
      abort();
   }

   std::uint32_t heapAddress = address - getInstructionsSizeTrytes() /* +/- some value if program starts at address other than 0 */;

   doStoreToHeap(heapAddress, value, numTrytes);
}

void ExecutableProgram::calculateHeapSize()
{
   m_heapSizeTrytes = getProgramSizeTrytes() - getInstructionsSizeTrytes();
   if((m_heapSizeTrytes < 0 ) || (m_heap.size() > m_heapSizeTrytes))
   {
      std::cerr << __PRETTY_FUNC__ << ": Tried recalcuating heap size, but heap already overflowing" << std::endl;
      abort();
   }

   m_heap.resize(m_heapSizeTrytes, 0);

   std::cout << "Program size: " << getProgramSizeTrytes() << " trytes; Instruction size: " << getInstructionsSizeTrytes() << " trytes; Heap size: " << m_heap.size() << " trytes" << std::endl;
}

std::uint32_t ExecutableProgram::getProgramSizeTrytes() const
{
   return PROGRAM_SIZE_TRYTES;
}

std::uint32_t ExecutableProgram::getInstructionsSizeTrytes() const
{
   return std::ceil((m_instructionsSize * REBEL6_TRITS_PER_INSTRUCTION) / REBEL6_TRITS_PER_TRYTE);
}

void ExecutableProgram::printInstructions() const
{
   std::int32_t pc = 0;
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

void ExecutableProgram::doStoreToHeap(std::uint32_t heapAddress, const Tint& value, std::uint8_t numTrytes)
{
   Trits trits;
   TernaryLogic::TintToTrits(value, trits); // Could send m_heap directly, but lets keep it safe
   std::uint8_t numTrits = numTrytes * REBEL6_TRITS_PER_TRYTE;

   if(trits.size() > numTrits)
   {
      std::cout << "Storing an unintended number of trits to heap - Intended = " << (int)numTrits << "; actual = " << trits.size() << std::endl;
   }

   trits.resize(numTrits, 0); // Fill the rest with zeroes so we ensure the memory allocated for this value has all trits properly reset

   std::cout << "Initial value = " << value << std::endl;
   for(std::uint8_t i = 0; i < numTrytes; ++i)
   {
      Tryte val = 0;
      for(std::uint8_t j = 0; j < REBEL6_TRITS_PER_TRYTE; ++j)
      {
         val += (trits[j + (i * REBEL6_TRITS_PER_TRYTE)] * std::pow(3, j));
      }

      m_heap[heapAddress] = val;
      std::cout << "Storing ternary value to heap = " << val << " at index " << heapAddress << std::endl;
      ++heapAddress;
   }
   std::cout << std::endl;
}

}
}
