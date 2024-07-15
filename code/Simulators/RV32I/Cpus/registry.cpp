#include "registry.h"

#include <logger.h>

#include <iostream>
#include <map>

namespace
{
int countBitFlips(int oldVal, int newVal)
{
   int num = (oldVal ^ newVal);
   unsigned int flips = 0;

   // Brian Kernighan’s algorithm
   while(num)
   {
      num = num & (num - 1); // Clear the rightmost set bit
      flips += 1;
   }

   return flips;
}

const std::map<std::string, std::string> RegisterNameXToABI = {
    {"x0", "zero"},
    {"x1", "ra"},
    {"x2", "sp"},
    {"x3", "gp"},
    {"x4", "tp"},
    {"x5", "t0"},
    {"x6", "t1"},
    {"x7", "t2"},
    {"x8", "s0"},
    {"x9", "s1"},
    {"x10", "a0"},
    {"x11", "a1"},
    {"x12", "a2"},
    {"x13", "a3"},
    {"x14", "a4"},
    {"x15", "a5"},
    {"x16", "a6"},
    {"x17", "a7"},
    {"x18", "s2"},
    {"x19", "s3"},
    {"x20", "s4"},
    {"x21", "s5"},
    {"x22", "s6"},
    {"x23", "s7"},
    {"x24", "s8"},
    {"x25", "s9"},
    {"x26", "s10"},
    {"x27", "s11"},
    {"x28", "t3"},
    {"x29", "t4"},
    {"x30", "t5"},
    {"x31", "t6"}
};

void convertToABI(const std::string& in, std::string& out)
{
   out.resize(in.length());
   std::transform(in.begin(), in.end(), out.begin(), [](unsigned char c){ return std::tolower(c); });

   if(out[0] == 'x')
   {
      try
      {
         out = RegisterNameXToABI.at(out);
      }
      catch(std::exception&)
      {
         std::cerr << __PRETTY_FUNC__ << ": Trying to convert unsupported x-named register " << in << std::endl;
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

Registry::Registry()
    : m_numBitFlips(0)
{
   std::string reg;
   std::string regABI;
   for(int i = 0; i < 32; ++i)
   {
      reg = "x" + std::to_string(i);
      convertToABI(reg, regABI);
      m_registry[regABI] = 0;
   }

}

void Registry::store(const std::string& regName, int regValue)
{
   std::string abiName;
   convertToABI(regName, abiName);

   // x0/zero is hardcoded to 0
   if(abiName == "zero")
   {
      return;
   }

   int oldVal = 0;

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

int Registry::load(const std::string& regName)
{
   std::string abiName;
   convertToABI(regName, abiName);

   int retVal = 0;

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

int Registry::getAccumulatedBitFlips() const
{
   return m_numBitFlips;
}

void Registry::printRegistry()
{
   std::string abiName;
   std::cout << "x\t" << "abi\t" << "hex\t\t" << "dec" << std::endl;
   std::cout << "------------------------------------" << std::endl;
   for(int i = 0; i < 32; ++i)
   {
      const std::string xName = "x" + std::to_string(i);
      convertToABI(xName, abiName);
      int val = load(abiName);

      std::cout << xName << "\t" << abiName << "\t0x" << std::hex << val << "\t\t" << std::dec << val << std::endl;
   }

   /*for(const auto& [key, value]: m_registry)
   {
      std::cout << key << ": " << value << std::endl;
   }*/
}

}
}
