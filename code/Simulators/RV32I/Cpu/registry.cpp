#include "registry.h"

#include <logger.h>

#include <iostream>
#include <vector>

namespace
{
int countBitFlips(int oldVal, int newVal)
{
   int num = (oldVal ^ newVal);
   uint flips = 0;

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
   std::transform(in.begin(), in.end(), out.begin(), [](unsigned char c){ return std::tolower(c); });

   if(out[0] == 'x')
   {
      try
      {
         int index = stoi(out.substr(1, out.length()));
         out = RegisterABINames[index];
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

int Registry::getIntegerValue(const std::string& regName)
{
   std::string name;
   name.resize(regName.length());
   std::transform(regName.begin(), regName.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });

   int value = 0;
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
