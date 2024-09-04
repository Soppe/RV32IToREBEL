#include "registry.h"

#include <logger.h>

#include <iostream>
#include <vector>

namespace
{
std::uint32_t countBitFlips(std::int32_t oldVal, std::int32_t newVal)
{
   std::int32_t num = (oldVal ^ newVal);
   std::uint32_t flips = 0;

   // Brian Kernighan’s algorithm
   while(num)
   {
      num = num & (num - 1); // Clear the rightmost set bit
      flips += 1;
   }

   return flips;
}

const std::vector<std::string> RegisterABINames = {"zero", "ra", "sp", "gp", "tp",
                                                   "t0", "t1", "t2",
                                                   "s0", "s1",
                                                   "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
                                                   "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
                                                   "t3", "t4", "t5", "t6"};

void convertToABI(const std::string& in, std::string& out)
{
   out.resize(in.length());
   std::transform(in.begin(), in.end(), out.begin(), [](std::uint8_t c){ return std::tolower(c); });

   if(out[0] == 'x')
   {
      std::int32_t index = stoi(out.substr(1, out.length()));
      if((index >= 0) && (index < RegisterABINames.size()) )
      {
         out = RegisterABINames[index];
      }
      else
      {
         std::cout << __PRETTY_FUNC__ << ": Trying to convert unconvertable x-named register " << in << std::endl;
      }
   }
   else
   {
      // Already happening in the transform
      //out = in;
   }
}
}

namespace Simulators
{
namespace RV32I
{

Registry::Registry(int size)
{
   reset(size);
}

void Registry::reset(int size)
{
   m_numBitFlips = 0;
   m_registry.clear();

   std::string reg;
   std::string regABI;
   for(std::int32_t i = 0; i < size; ++i)
   {
      reg = "x" + std::to_string(i);
      convertToABI(reg, regABI);
      m_registry[regABI] = 0;
   }
}

void Registry::store(const std::string& regName, std::int32_t regValue)
{
   std::string abiName;
   convertToABI(regName, abiName);

   // x0/zero is hardcoded to 0
   if(abiName == "zero")
   {
      return;
   }

   std::int32_t oldVal = 0;

   auto it = m_registry.find(abiName);
   if(it != m_registry.end())
   {
      oldVal = it->second;
      it->second = regValue;
   }
   else
   {
      std::cerr << "Tried to store value " << regValue << " to unsupported registry named " << regName << std::endl;
      abort();
   }

   m_numBitFlips += countBitFlips(oldVal, regValue);
}

std::int32_t Registry::load(const std::string& regName)
{
   std::string abiName;
   convertToABI(regName, abiName);

   std::int32_t retVal = 0;

   auto it = m_registry.find(abiName);
   if(it != m_registry.end())
   {
      retVal = it->second;
   }
   else
   {
      std::cerr << "Tried to load value from unsupported registry named " << regName << std::endl;
      abort();
   }

   return retVal;
}

std::uint8_t Registry::getIntegerValue(const std::string& regName)
{
   std::string name;
   name.resize(regName.length());
   std::transform(regName.begin(), regName.end(), name.begin(), [](std::uint8_t c){ return std::tolower(c); });

   std::uint8_t value = 0;
   if(name[0] == 'x')
   {
      try
      {
         value = stoi(name.substr(1, name.length()));
      }
      catch(const std::exception&)
      {
         std::cerr << __PRETTY_FUNC__ << ": Unable to convert register " << name << " to integer" << std::endl;
         abort();
      }
   }
   else
   {
      auto it = std::find(RegisterABINames.begin(), RegisterABINames.end(), name);
      if(it == RegisterABINames.end())
      {
         std::cerr << __PRETTY_FUNC__ << ": Unable to find register " << name << std::endl;
         abort();
      }
      value = std::distance(RegisterABINames.begin(), it);
   }

   return value;
}

std::int32_t Registry::getAccumulatedBitFlips() const
{
   return m_numBitFlips;
}

void Registry::printRegistry()
{
   std::string abiName;
   std::cout << "x\t" << "abi\t" << "hex\t\t" << "dec" << std::endl;
   std::cout << "------------------------------------" << std::endl;
   for(std::int32_t i = 0; i < 32; ++i)
   {
      const std::string xName = "x" + std::to_string(i);
      convertToABI(xName, abiName);
      std::int32_t val = load(abiName);

      std::cout << xName << "\t" << abiName << "\t0x" << std::hex << val << "\t\t" << std::dec << val << std::endl;
   }

   /*for(const auto& [key, value]: m_registry)
   {
      std::cout << key << ": " << value << std::endl;
   }*/
}

}
}
